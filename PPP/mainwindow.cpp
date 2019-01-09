#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#define PRGNAME     "RTKNAVI-QT"           // program name
#define TRACEFILE   "rtknavi_%Y%m%d%h%M.trace" // debug trace file
#define STATFILE    "rtknavi_%Y%m%d%h%M.stat"  // solution status file
//const QChar degreeChar(0260);           // character code of degree (UTF-8)
#define SATSIZE     20                  // satellite circle size in skyplot
#define MINSNR      10                  // minimum snr
#define MAXSNR      60                  // maximum snr

#define SQRT(x)     ((x)<0.0?0.0:sqrt(x))
#define MIN(x,y)    ((x)<(y)?(x):(y))
prcopt_t prcopt1;
int StreamType[8];
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    this->setWindowTitle("PPP");
    setWindowState(Qt::WindowMaximized);
    setLablSsize();
    prcopt1=prcopt_default;
    //--------------初始化流路径个流类型-----------
    for(int i=0;i<8;i++)
    {
       strcpy(streamPaths[i],"");
       StreamType[i]=STR_NONE;
    }
    strcpy(OutResultPath,"");
    for(int i=0;i<2;i++)
    {
        strcpy(inputFilePaths[i],"");
    }

    PppConfig =new DialogConfig(this);

    //----------配置参数完成显示主窗口，同时隐藏配置对话框---------
    connect(PppConfig,&DialogConfig::sigConfigOk,this,[=]()
    {
        PppConfig->hide();
        this->show();

        strcpy(streamPaths[0], PppConfig->RawPaths.toLocal8Bit().data());
        strcpy(streamPaths[2], PppConfig->NetCorrPaths.toLocal8Bit().data());
        strcpy(streamPaths[5], PppConfig->OutputRawPath.toLocal8Bit().data());

        qDebug()<<PppConfig->RawPaths;
        qDebug()<<PppConfig->NetCorrPaths;
        qDebug()<<PppConfig->OutputRawPath;
        strcpy(inputFilePaths[0], PppConfig->OrbitPath.toLocal8Bit().data());
        strcpy(inputFilePaths[1], PppConfig->ClockPath.toLocal8Bit().data());
        strcpy(OutResultPath, PppConfig->OutputResultPath.toLocal8Bit().data());

        startppp(StreamType,streamPaths,inputFilePaths,OutResultPath);
        id = startTimer(1000);
    });

    FreqType1 = 0;
    //dataTimer.start(1000);
    connect(ui->actionupdate,&QAction::triggered,this,[=]()
    {
        for(int i=0;i<6;i++)
            initial_solution[i]=solution_ppp[i];
    });
    connect(ui->actionFreq,&QAction::triggered,this,[=]()
    {
       changeplot();
    });

   //--------------初始化画图窗口------------------
    setupRealtimeDataDemo_xyz(ui->customPlot_xyz);
    setupRealtimeDataDemo_Nstas(ui->customPlot_nstas);
    setupRealtimeDataDemo_Dop(ui->customPlot_dop);
    setupRealtimeDataDemo_Res(ui->customPlot_res);

//----------------config dialog-------------------------
    connect(ui->actionsConfig,&QAction::triggered,this,[=]()
    {
        PppConfig->show();
        this->hide();
    });

}

void MainWindow::setLablSsize()
{
    QFont ft;
    ft.setPointSize(14);
    ui->longitude->setFont(ft);
    ui->latitude->setFont(ft);
    ui->height->setFont(ft);
    ui->speed->setFont(ft);
    ui->UTC->setFont(ft);
    ui->GPS->setFont(ft);
    ui->system->setFont(ft);
    ui->positon_mode->setFont(ft);
    ui->nstas->setFont(ft);
    ui->GDOP->setFont(ft);
    ui->PDOP->setFont(ft);
    ui->HDOP->setFont(ft);
    ui->VDOP->setFont(ft);
    ui->acceleration->setFont(ft);

    ui->label_longitude->setFont(ft);
    ui->label_latitude->setFont(ft);
    ui->label_height->setFont(ft);
    ui->label_speed->setFont(ft);
    ui->label_UTC->setFont(ft);
    ui->label_GPS->setFont(ft);
    ui->label_system->setFont(ft);
    ui->label_positon_mode->setFont(ft);
    ui->label_nstas->setFont(ft);
    ui->label_GDOP->setFont(ft);
    ui->label_PDOP->setFont(ft);
    ui->label_HDOP->setFont(ft);
    ui->label_VDOP->setFont(ft);
    ui->label_acceleration->setFont(ft);

    ui->label_xyz_plot->setFont(ft);
    ui->label_nstas_plot->setFont(ft);
    ui->label_cno_plot->setFont(ft);
    ui->label_sky_plot->setFont(ft);
    ui->label_res_plot->setFont(ft);

}
MainWindow::~MainWindow()
{
    delete ui;
}

void  MainWindow::UpdatePlot(void)
{
    DrawPlot(ui->DispSnr,ui->DispSky,FreqType1);
}


void  MainWindow::DrawPlot(QLabel *plot,QLabel *plot_sky, int freq)
{
 //   qDebug() << "into DrawPlot";
    QString s1,s2;
    gtime_t time;

    if (!plot) return;

    QPixmap buffer(plot->size());
    QPixmap buffer2(plot_sky->size());

    if (buffer.isNull()) return;

    buffer.fill(Qt::red);

    QPainter *c= new QPainter(&buffer);
    QPainter *c2= new QPainter(&buffer2);

    QFont font;
    font.setPixelSize(12);
    c->setFont(font);
    c2->setFont(font);

    QString fstr[]={"","L1 ","L2 ","L5 ","L6 ","L7 ","L8 ","","","",""};
    int w=buffer.size().width()-2,h=buffer.height()-2;
    int w2=buffer.size().width()-2,h2=buffer.height()-2;
    int i,j,x,sat[MAXSAT],ns,vsat[MAXSAT];
    int *snr0[MAXSAT];
    int snr[MAXSAT][NFREQ];
    char name[16];
    double az[MAXSAT],el[MAXSAT],rr[3],rs[6],e[3],pos[3],azel[2];

    trace(4,"DrawPlot\n");

    fstr[NFREQ+1]="SYS ";

    for (i=0;i<MAXSAT;i++)
    {
        snr0[i]=snr[i];
    }
    ns=rtksvrostat(&svr1,0,&time,sat,az,el,snr0,vsat);

    rtksvrlock(&svr1);
    matcpy(rr,svr1.rtk.sol.rr,3,1);
    ecef2pos(rr,pos);
    rtksvrunlock(&svr1);

// get obs date to val
    for (i=0;i<1;i++)
    {
        for (j=0;j<ns;j++)
        {
            if (az[j]!=0.0||el[j]!=0.0) continue;
            satno2id(sat[j],name);
        //    if (!tle_pos(time,name,"","",&TLEData,NULL,rs)) continue;
            if (geodist(rs,rr,e)>0.0)
            {
                satazel(pos,e,azel);
                az[j]=azel[0];
                el[j]=azel[1];
            }
        }
        if (ns>0)
        {
            Nsat=ns;
            for (int j=0;j<ns;j++)
            {
                Sat [j]=sat [j];
                Az  [j]=az  [j];
                El  [j]=el  [j];
                for (int k=0;k<NFREQ;k++)
                {
                    Snr[j][k]=snr0[j][k];
                }
                Vsat[j]=vsat[j];
            }
        }
        else
        {
            Nsat=0;
            for (j=0;j<Nsat;j++)
            {
                Vsat[j]=0;
                for (int k=0;k<NFREQ;k++)
                {
                    Snr[j][k]=0;
                }
            }
        }
    }
/*******************************************************/
    c->setBrush(Qt::white);
    c->fillRect(buffer.rect(),QBrush(Qt::white));
    c2->setBrush(Qt::white);
    c2->fillRect(buffer.rect(),QBrush(Qt::white));
    x=4;

    DrawSnr(c,w,h-15,0,15,0,freq);
    s1=QString(tr("%1 SNR (dBHz)")).arg(fstr[freq]);
    DrawText(c,x,1,s1,Qt::gray,0);

    DrawSat(c2,w2,h2,0,0,0,freq);
    s1=QString("%1").arg(fstr[!freq?1:freq]);
    DrawText(c2,x,1,s1,Qt::gray,0);

    plot->setPixmap(buffer);//load buff to label / buff can see a picture
    plot_sky->setPixmap(buffer2);//load buff to label / buff can see a picture

    delete c;
    delete c2;
}
void  MainWindow::DrawSnr(QPainter *c, int w, int h, int x0, int y0,
    int index, int freq)
{
  //  qDebug() << "into DrawSne";
    static const QColor color[]=
    {
        QColor(0,128,0),QColor(0,128,128),QColor(0xA0,0,0xA0),
        QColor(128,0,0),QColor(0,0,128),QColor(128,128,128)
    };
    static const QColor color_sys[]={
        Qt::green,QColor(0,0xAA,0xFF),QColor(255,0,255),Qt::blue,Qt::red,Qt::gray
    };
    QString s;
    int i,j,k,l,n,x1,y1,y2,y3,hh=h-15,ww,www,snr[NFREQ+1],mask[6]={0};
    char id[16],sys[]="GREJCS",*q;

    trace(4,"DrawSnr: w=%d h=%d x0=%d y0=%d index=%d freq=%d\n",w,h,x0,y0,index,freq);
    for (snr[0]=MINSNR+10;snr[0]<MAXSNR;snr[0]+=10)
    {
        y1=y0+hh-(snr[0]-MINSNR)*hh/(MAXSNR-MINSNR);
        c->setPen(QColor(0x0c,0x0c,0x0c));
        c->drawLine(x0+3,y1,x0+w-13,y1);
        DrawText(c,x0+10,y1,QString::number(snr[0]),Qt::gray,1);
    }
    //-----------plan a rect------no brush-----------
    y1=y0+hh;
    QRect b(x0+1,y0,x0+w-2,y1);
    c->setBrush(Qt::NoBrush);
    c->setPen(Qt::gray);
    c->drawRect(b);
   //--------------------------------------------------
    for (i=0;i<Nsat&&i<MAXSAT;i++)
    {

        ww=(w-10)/Nsat;
        www=ww-2<8?ww-2:8;
        x1=x0+i*(w-10)/Nsat+ww/2;
        satno2id(Sat[i],id);
        l=(q=strchr(sys,id[0]))?(int)(q-sys):5;

        for (j=snr[0]=0;j<NFREQ;j++)
        {
            snr[j+1]=Snr[i][j];
            if ((freq&&freq==j+1)||((!freq||freq>NFREQ)&&snr[j+1]>snr[0]))
            {
                snr[0]=snr[j+1];
              //  qDebug()<<QString::number(snr[0]);
            }
        }
        for (j=0;j<NFREQ+2;j++)
        {
            k=j<NFREQ+1?j:0;
            y3=j<NFREQ+1?0:2;
            y2=y1-y3;
            if (snr[k]>0) y2-=(snr[k]-MINSNR)*hh/(MAXSNR-MINSNR)-y3;
            y2=y2<2?2:(y1<y2?y1:y2);

            QRect r1(x1,y1,www,y2-y1);
            if (j==0)
            {
                c->setBrush(QBrush(freq<NFREQ?SnrColor(snr[k]):color_sys[l],Qt::SolidPattern));
                if (!Vsat[i]) c->setBrush(QBrush(QColor(0x0c,0x0c,0x0c),Qt::SolidPattern));
                c->drawRect(r1);
            }
            else
            {
                c->setPen(j<NFREQ+1?QColor(0x0c,0x0c,0x0c):Qt::gray);
                c->setBrush(Qt::NoBrush);
                c->drawRect(r1);
            }
        }
        DrawText(c,x1+www/2,y1+6,(s=id+1),color[l],1);
        mask[l]=1;
    }
    for (i=n=0;i<6;i++) if (mask[i]) n++;
    for (i=j=0;i<6;i++)
    {
        if (!mask[i]) continue;
        sprintf(id,"%c",sys[i]);
       DrawText(c,x0+w-15+8*(-n+j++),y0+3,(s=id),color[i],0);
    }
}
void  MainWindow::DrawText(QPainter *c, int x, int y, const QString &s,const QColor &color, int align)
{
    int flags=0;
    switch (align) {
    case 0: flags|=Qt::AlignLeft;break;
    case 1: flags|=Qt::AlignHCenter;break;
    case 2: flags|=Qt::AlignRight;break;
    }

    QRectF off=c->boundingRect(QRectF(),flags,s);

    c->setPen(color);

    c->translate(x,y);
    c->drawText(off,s);
    c->translate(-x,-y);

}

QColor  MainWindow::SnrColor(int snr)
{
   // QColor color[]={Qt::green,QColor(255,128,0,255),QColor(0,255,128,255),Qt::blue,Qt::red,Qt::gray};
    QColor color[]={QColor(0,120,40,255),QColor(255,128,0,255),QColor(0,255,128,255),Qt::blue,Qt::red,Qt::gray};
    unsigned int r1,g1,b1;
    QColor c1,c2;
    double a;
    int i;

    if (snr<25) return color[5];
    if (snr<27) return color[4];
    if (snr>47) return color[0];
    a=(snr-27.5)/5.0;
    i=static_cast<int>(a);
    a-=i;
    c1=color[3-i];
    c2=color[4-i];
    r1=static_cast<unsigned int>(a*c1.red()  +(1.0-a)*c2.red())&0xFF;
    g1=static_cast<unsigned int>(a*c1.green()+(1.0-a)*c2.green())&0xFF;
    b1=static_cast<unsigned int>(a*c1.blue() +(1.0-a)*c2.blue())&0xFF;

    return QColor(r1,g1,b1);
}

void MainWindow::dateUpdate()
{
    const char *sep = " ";
    int week;
    double sow, ep[6], pos[3], denu[3], dxyz[3] = {0.0};
    time2epoch(svr1.rtcm[0].obs.data[0].time, ep);
//    const char *sep = " ";
//    int week;
//    double sow, ep[6], pos[3], denu[3], dxyz[3] = {0.0};
//    time2epoch(svr1.rtcm[0].obs.data[0].time, ep);
//    sow = time2gpst(svr1.rtcm[0].obs.data[0].time, &week);

//    double ture_xyz[3] = { -2005029.8275 ,5411164.7129 ,2707856.6051 };
//         ecef2pos(ture_xyz, pos);
//    for (int i = 0; i<3; i++)
//        denu[i] = dxyz[i] = svr1.rtk.sol.rr[i] - ture_xyz[i];
//    ecef2enu(pos, dxyz, denu);
//    //double time_dif = timediff(svr1->obs[0].time, ssr_time);
//    ecef2pos(svr1.rtk.sol.rr, pos);
   // printf("%04d%s%02d%s%02d%s%02d%s%02d%s%02d%s%04d%s%9.2f%s%14.7f%s%14.7f%s%14.7f%s%7.4f%s%7.4f%s%7.4f%s%7.4f%s%02d%s%02d%s%02d%s%4.2f%s%3.2f\n",
       // (int)ep[0], sep, (int)ep[1], sep, (int)ep[2], sep, (int)ep[3], sep, (int)ep[4], sep, (int)ep[5], sep, week, sep, sow, sep, svr1.rtk.sol.rr[0], sep, svr1.rtk.sol.rr[1], sep, svr1.rtk.sol.rr[2], sep, denu[0], sep, denu[1], sep, denu[2], sep, norm(denu, 3), sep,stat,sep ,0, sep,0,sep, svr1.rtk.sol.dop[1],sep, 0);
//    if(svr1.stream[0].inr > 0)
//          ui->signal_in_b->setStyleSheet("background-color:green;");
//     else
//          ui->signal_in_b->setStyleSheet("background-color:yellow;");
//    ui->stat->setText("PPP");

    if(svr1.rtk.sol.stat==SOLQ_PPP)
   {
        double pos[3],dms1[3],dms2[3];
         ecef2pos(svr1.rtk.sol.rr, pos);
         deg2dms(pos[0]*R2D,dms1);
         deg2dms(pos[1]*R2D,dms2);


        double speed=sqrt(pow(svr1.rtk.sol.rr[3],2)+pow(svr1.rtk.sol.rr[4],2)+pow(svr1.rtk.sol.rr[5],2));
        ui->longitude->setText(QString::number(dms1[0])+"°"+QString::number(dms1[1],'f',2)+"'"+QString::number(dms1[2],'f',2)+"''");
        ui->latitude->setText(QString::number(dms2[0])+"°"+QString::number(dms2[1],'f',2)+"'"+QString::number(dms2[2],'f',2)+"''");
        ui->height->setText(QString::number(pos[2],'f',2)+"m");
        ui->speed->setText(QString::number(speed,'f',2));
        ui->UTC->setText(QString::number(ep[3])+":"+QString::number(ep[4])+":"+QString::number(ep[5]));
        ui->GPS->setText(QString::number(ep[3]+8)+":"+QString::number(ep[4])+":"+QString::number(ep[5]));
        ui->system->setText(QString::number(NSYS_USED));
        ui->positon_mode->setText("PPP");
        ui->nstas->setText(QString::number( Nsat));

        ui->GDOP->setText(QString::number( svr1.rtk.sol.dop[0], 'f', 2));
        ui->PDOP->setText(QString::number( svr1.rtk.sol.dop[1], 'f', 2));
        ui->HDOP->setText(QString::number( svr1.rtk.sol.dop[2], 'f', 2));
        ui->VDOP->setText(QString::number( svr1.rtk.sol.dop[3], 'f', 2));
   }
   UpdatePlot();
   realtimeDataSlot_xyz();
   realtimeDataSlot_Nstas();
   realtimeDataSlot_Dop();
   realtimeDataSlot_Res();
}

void  MainWindow::DrawSky(QPainter *c, int w, int h, int x0, int y0)
{
    QString label[]={tr("N"),tr("E"),tr("S"),tr("W")};
    QPoint p(x0+w/2,y0+h/2);
    double r=MIN(w*0.95,h*0.95)/2;
    int a,e,d,x,y;

    c->setBrush(Qt::white);
    for (e=0;e<90;e+=30)
    {
        d=static_cast<int>(r*(90-e)/90);
        c->setPen(e==0?Qt::gray:QColor(0xc0,0xc0,0xc0));
        c->drawEllipse(p.x()-d,p.y()-d,2*d+1,2*d+1);
    }
    for (a=0;a<360;a+=45)
    {
        x=static_cast<int>(r*sin(a*D2R));
        y=static_cast<int>(r*cos(a*D2R));
        c->setPen(QColor(0xc0,0xc0,0xc0));
        c->drawLine(p.x(),p.y(),p.x()+x,p.y()-y);  //this is + becasue O(0,0) in  top left corner, x is relative the center length
        if (a%90==0) DrawText(c,p.x()+x,p.y()-y,label[a/90],Qt::gray,1);
    }
}

// draw satellites in skyplot -----------------------------------------------
void  MainWindow::DrawSat(QPainter *c, int w, int h, int x0, int y0, int index, int freq)
{
    static const QColor color_sys[]={
        Qt::green,QColor(0x00,0xAA,0xFF),QColor(0xff,0x00,0xff),Qt::blue,Qt::red,Qt::gray
    };
    QString s;
    QPoint p(w/2,h/2);
    double r=MIN(w*0.95,h*0.95)/2,azel[MAXSAT*2],dop[4];
    int i,k,l,d,x[MAXSAT],y[MAXSAT],ns=0,f=!freq?0:freq-1;
    char id[16],sys[]="GREJCS",*q;

    trace(4,"DrawSat: w=%d h=%d index=%d freq=%d\n",w,h,index,freq);

    DrawSky(c,w,h,x0,y0);

    for (i=0,k=Nsat-1;i<Nsat&&i<MAXSAT;i++,k--)
    {
        if (El[k]<=0.0) continue;
        if (Vsat[k])
        {
            azel[ns*2]=Az[k]; azel[1+ns*2]=El[k];
            ns++;
        }
        satno2id(Sat[k],id);
        l=(q=strchr(sys,id[0]))?(int)(q-sys):5;
        x[i]=static_cast<int>(p.x()+r*(90-El[k]*R2D)/90*sin(Az[k]))+x0;
        y[i]=static_cast<int>(p.y()-r*(90-El[k]*R2D)/90*cos(Az[k]))+y0;
        c->setPen(Qt::gray);
        d=SATSIZE/2;
        c->setBrush(!Vsat[k]?QColor(0xc0,0xc0,0xc0): (freq<NFREQ?SnrColor(Snr[k][f]):color_sys[l]));
        c->drawEllipse(x[i]-d,y[i]-d,2*d+1,2*d+1);
        DrawText(c,x[i],y[i],s=id,Qt::white,1);
    }
   // dops(ns,azel,0.0,dop);
    DrawText(c,x0+3,y0+h-15,QString("# Sat:%1").arg(Nsat,2),Qt::gray,0);
    DrawText(c,x0+w-3,y0+h-15,QString("GDOP:%1").arg(svr1.rtk.sol.dop[0],0,'f',1),Qt::gray,2);
}

//-----------------------------------画xyz图初始化--------------------------
void MainWindow::setupRealtimeDataDemo_xyz(QCustomPlot *customPlot)
{

  customPlot->addGraph(); // green line
  customPlot->graph(0)->setPen(QPen(Qt::green));
  customPlot->graph(0)->setName("x");
  //customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
  customPlot->graph(0)->setLineStyle((QCPGraph::LineStyle)1);//设置线性

  customPlot->addGraph(); // blue line
  customPlot->graph(1)->setPen(QPen(Qt::blue));
  customPlot->graph(1)->setName("y");
 // customPlot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
  customPlot->graph(1)->setLineStyle((QCPGraph::LineStyle)1);//设置线性

  customPlot->addGraph(); // red line
  customPlot->graph(2)->setPen(QPen(Qt::red));
  customPlot->graph(2)->setName("z");
 // customPlot->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
  customPlot->graph(2)->setLineStyle((QCPGraph::LineStyle)1);//设置线性

  customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
  customPlot->xAxis->setAutoTickStep(false);
  customPlot->xAxis->setTickStep(600);
  customPlot->axisRect()->setupFullAxesBox();


  // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
 // connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot_xyz()));
  //dataTimer.start(1000); // Interval 0 means to refresh as fast as possible
  customPlot->legend->setVisible(true);
  customPlot->axisRect()->insetLayout()->setInsetAlignment(0,Qt::AlignTop|Qt::AlignLeft);
}

void MainWindow::realtimeDataSlot_xyz()
{
    //key的单位是秒
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    if(svr1.rtk.sol.stat==SOLQ_PPP)
    {
        #if 0
        double delt_rr[6];
        for(int i=0;i<6;i++)
        {
            solution_ppp[i]=svr1.rtk.sol.rr[i];
            delt_rr[i]=svr1.rtk.sol.rr[i]-initial_solution[i];
        }
        #endif
        double delt_rr[6]={0};
        double ture_xyz[3] = { -2005044.4023382 ,5411159.3009611 ,2707856.7667605 };
        for(int i=0;i<3;i++)
         delt_rr[i]=svr1.rtk.sol.rr[i]-ture_xyz[i];

        ui->customPlot_xyz->graph(0)->addData(key, delt_rr[0]);//添加数据1到曲线1
        ui->customPlot_xyz->graph(1)->addData(key, delt_rr[1]);//添加数据2到曲线2
        ui->customPlot_xyz->graph(2)->addData(key, delt_rr[2]);//添加数据2到曲线2
        //删除8秒之前的数据。这里的8要和下面设置横坐标宽度的8配合起来
        //才能起到想要的效果，可以调整这两个值，观察显示的效果。
        ui->customPlot_xyz->graph(0)->removeDataBefore(key-3600);
        ui->customPlot_xyz->graph(1)->removeDataBefore(key-3600);
        ui->customPlot_xyz->graph(2)->removeDataBefore(key-3600);
          //自动设定graph(1)曲线y轴的范围，如果不设定，有可能看不到图像
        //也可以用ui->customPlot->yAxis->setRange(up,low)手动设定y轴范围
        ui->customPlot_xyz->graph(0)->rescaleValueAxis();
        ui->customPlot_xyz->graph(1)->rescaleValueAxis();
        ui->customPlot_xyz->graph(2)->rescaleValueAxis(true);

        //这里的8，是指横坐标时间宽度为8秒，如果想要横坐标显示更多的时间
        //就把8调整为比较大到值，比如要显示60秒，那就改成60。
        //这时removeDataBefore(key-8)中的8也要改成60，否则曲线显示不完整。
        ui->customPlot_xyz->xAxis->setRange(key+0.25, 3600, Qt::AlignRight);//设定x轴的范围
        ui->customPlot_xyz->graph()->rescaleAxes(true);//坐标轴自适应

        ui->customPlot_xyz->yAxis->setRange(-10,10);
        ui->customPlot_xyz->xAxis2->setVisible(true);
        ui->customPlot_xyz->xAxis2->setTickLabels(false);
        connect(ui->customPlot_xyz->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot_xyz->xAxis2, SLOT(setRange(QCPRange)));

        ui->customPlot_xyz->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
//        connect(ui->customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress(QMouseEvent*)));
//        connect(ui->customPlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(mouseMove(QMouseEvent*)));
//        connect(ui->customPlot, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(mouseRelease(QMouseEvent*)));
        ui->customPlot_xyz->axisRect()->setupFullAxesBox();
        ui->customPlot_xyz->replot();
    }
}

//------------------------------------------------画Nstas图初始化--------------------------------------------------
void MainWindow::setupRealtimeDataDemo_Nstas(QCustomPlot *customPlot)
{

  customPlot->addGraph(); // blue line
  customPlot->graph(0)->setPen(QPen(Qt::red));
  customPlot->graph(0)->setName("NGPS");
 // customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
  customPlot->graph(0)->setLineStyle((QCPGraph::LineStyle)2);//设置线性

  customPlot->addGraph(); // blue line
  customPlot->graph(1)->setPen(QPen(Qt::darkGreen));
  customPlot->graph(1)->setName("NCMP");
 // customPlot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
  customPlot->graph(1)->setLineStyle((QCPGraph::LineStyle)2);//设置线性

//  customPlot->addGraph(); // blue line
//  customPlot->graph(2)->setPen(QPen(Qt::blue));
//  customPlot->graph(2)->setName("NCMP");
// // customPlot->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
//  customPlot->graph(2)->setLineStyle((QCPGraph::LineStyle)2);//设置线性

//  customPlot->addGraph(); // blue line
//  customPlot->graph(3)->setPen(QPen(Qt::yellow));
//  customPlot->graph(3)->setName("NGAL");
// // customPlot->graph(3)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
//  customPlot->graph(3)->setLineStyle((QCPGraph::LineStyle)2);//设置线性

  customPlot->addGraph(); // blue line
  customPlot->graph(2)->setPen(QPen(Qt::darkMagenta));
  customPlot->graph(2)->setName("NSUM");
 // customPlot->graph(3)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
  customPlot->graph(2)->setLineStyle((QCPGraph::LineStyle)2);//设置线性

  customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
  customPlot->xAxis->setAutoTickStep(false);
  customPlot->xAxis->setTickStep(600);
  customPlot->axisRect()->setupFullAxesBox();


  // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
 // connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot_Nstas()));
 //dataTimer.start(1000); // Interval 0 means to refresh as fast as possible
  customPlot->legend->setVisible(true);
  customPlot->axisRect()->insetLayout()->setInsetAlignment(0,Qt::AlignTop|Qt::AlignLeft);
}

void MainWindow::realtimeDataSlot_Nstas()
{
    //key的单位是秒
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    int nstas=0;
    for(int i=0;i<NSYS;i++)
    {
        nstas+=svr1.rtk.sol.nsats[i];
        ui->customPlot_nstas->graph(i)->addData(key, svr1.rtk.sol.nsats[i]);//添加数据1到曲线1
        ui->customPlot_nstas->graph(i)->removeDataBefore(key-3600);
    }
    ui->customPlot_nstas->graph(2)->addData(key, nstas);
    //删除8秒之前的数据。这里的8要和下面设置横坐标宽度的8配合起来
    ui->customPlot_nstas->graph(2)->removeDataBefore(key-3600);

      //自动设定graph(1)曲线y轴的范围，如果不设定，有可能看不到图像
    //也可以用ui->customPlot->yAxis->setRange(up,low)手动设定y轴范围
//    ui->customPlot_nstas->graph(0)->rescaleValueAxis();
//    ui->customPlot_nstas->graph(1)->rescaleValueAxis();
//    ui->customPlot_nstas->graph(2)->rescaleValueAxis(true);

    ui->customPlot_nstas->xAxis->setRange(key+0.25, 3600, Qt::AlignRight);//设定x轴的范围
    ui->customPlot_nstas->graph()->rescaleAxes(true);//坐标轴自适应

   ui->customPlot_nstas->xAxis2->setVisible(true);
   ui->customPlot_nstas->xAxis2->setTickLabels(false);
    connect(ui->customPlot_nstas->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot_nstas->xAxis2, SLOT(setRange(QCPRange)));
  //  connect(ui->customPlot_nstas->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot_nstas->yAxis2, SLOT(setRange(QCPRange)));
    ui->customPlot_nstas->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
//        connect(ui->customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress(QMouseEvent*)));
//        connect(ui->customPlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(mouseMove(QMouseEvent*)));
//        connect(ui->customPlot, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(mouseRelease(QMouseEvent*)));

    ui->customPlot_nstas->axisRect()->setupFullAxesBox();
    ui->customPlot_nstas->replot();

}

//------------------------------------------------Nstas图----------------------------------------------------------

//------------------------------------------------dop图初始化--------------------------------------------------
void MainWindow::setupRealtimeDataDemo_Dop(QCustomPlot *customPlot)
{

  customPlot->addGraph(); // blue line
  customPlot->graph(0)->setPen(QPen(Qt::red));
  customPlot->graph(0)->setName("GDOP");
  //customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
  customPlot->graph(0)->setLineStyle((QCPGraph::LineStyle)2);//设置线性

  customPlot->addGraph(); // blue line
  customPlot->graph(1)->setPen(QPen(Qt::blue));
  customPlot->graph(1)->setName("PDOP");
 // customPlot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
  customPlot->graph(1)->setLineStyle((QCPGraph::LineStyle)2);//设置线性

  customPlot->addGraph(); // blue line
  customPlot->graph(2)->setPen(QPen(Qt::green));
  customPlot->graph(2)->setName("HDOP");
 // customPlot->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
  customPlot->graph(2)->setLineStyle((QCPGraph::LineStyle)2);//设置线性

  customPlot->addGraph(); // blue line
  customPlot->graph(3)->setPen(QPen(Qt::yellow));
  customPlot->graph(3)->setName("VDOP");
 // customPlot->graph(3)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
  customPlot->graph(3)->setLineStyle((QCPGraph::LineStyle)2);//设置线性

  customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
  customPlot->xAxis->setAutoTickStep(false);
  customPlot->xAxis->setTickStep(600);
  customPlot->axisRect()->setupFullAxesBox();


  // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
 // connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot_Dop()));
 // dataTimer.start(1000); // Interval 0 means to refresh as fast as possible
  customPlot->legend->setVisible(true);
  customPlot->axisRect()->insetLayout()->setInsetAlignment(0,Qt::AlignTop|Qt::AlignLeft);
}

void MainWindow::realtimeDataSlot_Dop()
{
    //key的单位是秒
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

    ui->customPlot_dop->graph(0)->addData(key, svr1.rtk.sol.dop[0]);//添加数据1到曲线1
    ui->customPlot_dop->graph(1)->addData(key, svr1.rtk.sol.dop[1]);//添加数据1到曲线1
    ui->customPlot_dop->graph(2)->addData(key, svr1.rtk.sol.dop[2]);//添加数据1到曲线1
    ui->customPlot_dop->graph(3)->addData(key, svr1.rtk.sol.dop[3]);//添加数据1到曲线1
    //删除8秒之前的数据。这里的8要和下面设置横坐标宽度的8配合起来

    //这里的8，是指横坐标时间宽度为8秒，如果想要横坐标显示更多的时间
    //就把8调整为比较大到值，比如要显示60秒，那就改成60。
    ui->customPlot_dop->graph(0)->removeDataBefore(key-3600);
    ui->customPlot_dop->graph(1)->removeDataBefore(key-3600);
    ui->customPlot_dop->graph(2)->removeDataBefore(key-3600);
    ui->customPlot_dop->graph(3)->removeDataBefore(key-3600);
    //这时removeDataBefore(key-8)中的8也要改成60，否则曲线显示不完整。
    ui->customPlot_dop->xAxis->setRange(key+0.25, 3600, Qt::AlignRight);//设定x轴的范围
    ui->customPlot_dop->graph()->rescaleAxes(true);//坐标轴自适应

   // ui->customPlot->yAxis->setRange(-1,1);
   ui->customPlot_dop->xAxis2->setVisible(true);
   ui->customPlot_dop->xAxis2->setTickLabels(false);
   ui->customPlot_dop->yAxis2->setVisible(true);
   ui->customPlot_dop->yAxis2->setTickLabels(false);
    connect(ui->customPlot_dop->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot_dop->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot_dop->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot_dop->yAxis2, SLOT(setRange(QCPRange)));
    ui->customPlot_dop->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
//        connect(ui->customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress(QMouseEvent*)));
//        connect(ui->customPlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(mouseMove(QMouseEvent*)));
//        connect(ui->customPlot, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(mouseRelease(QMouseEvent*)));
    ui->customPlot_dop->axisRect()->setupFullAxesBox();
    ui->customPlot_dop->replot();

}

//------------------------------------------------Res图初始化--------------------------------------------------
void MainWindow::setupRealtimeDataDemo_Res(QCustomPlot *customPlot)
{

  for(int i=0;i<MAXSAT;i++)
  {
      customPlot->addGraph(); // blue line
      customPlot->graph(i)->setPen(QPen(PrnColor(i)));
     // customPlot->graph(i)->setName("GDOP");
     // customPlot->graph(i)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));
      customPlot->graph(i)->setLineStyle((QCPGraph::LineStyle)1);//设置线性
    }
    customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    customPlot->xAxis->setAutoTickStep(false);
    customPlot->xAxis->setTickStep(600);
    customPlot->axisRect()->setupFullAxesBox();

  // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
  // connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot_Res()));
  //dataTimer.start(1000); // Interval 0 means to refresh as fast as possible
  //customPlot->legend->setVisible(true);
}

void MainWindow::realtimeDataSlot_Res()
{
    //key的单位是秒
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    double resc, resp;
    if(svr1.rtk.sol.stat==SOLQ_PPP)
    {
    for(int i=0;i<MAXSAT;i++)
    {
        if (svr1.rtk.ssat[i].vsat[0] == 0)
        {
            resc = 0.0;
            resp = 0.0;
        }
        if (svr1.rtk.ssat[i].vsat[0] == 1 && (svr1.rtk.ssat[i].P[0] != 0 || svr1.rtk.ssat[i].P[1]!= 0 ||svr1.rtk.ssat[i].L[0] != 0 || svr1.rtk.ssat[i].L[1] != 0)) {
            resc = svr1.rtk.ssat[i].resc[0];
            resp = svr1.rtk.ssat[i].resp[0];

        }
        ui->customPlot_res->graph(i)->addData(key, resc);//添加数据1到曲线1
        ui->customPlot_res->graph(i)->removeDataBefore(key-3600);
    }

    //删除8秒之前的数据。这里的8要和下面设置横坐标宽度的8配合起来

    //这里的8，是指横坐标时间宽度为8秒，如果想要横坐标显示更多的时间
    //就把8调整为比较大到值，比如要显示60秒，那就改成60。
    //这时removeDataBefore(key-8)中的8也要改成60，否则曲线显示不完整。
    ui->customPlot_res->xAxis->setRange(key+0.25, 3600, Qt::AlignRight);//设定x轴的范围
    ui->customPlot_res->graph()->rescaleAxes(true);//坐标轴自适应

   ui->customPlot_res->yAxis->setRange(-1,1);
   ui->customPlot_res->xAxis2->setVisible(true);
   ui->customPlot_res->xAxis2->setTickLabels(false);
   ui->customPlot_res->yAxis2->setVisible(true);
   ui->customPlot_res->yAxis2->setTickLabels(false);
    connect(ui->customPlot_res->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot_res->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot_res->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot_res->yAxis2, SLOT(setRange(QCPRange)));
    ui->customPlot_res->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
//        connect(ui->customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress(QMouseEvent*)));
//        connect(ui->customPlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(mouseMove(QMouseEvent*)));
//        connect(ui->customPlot, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(mouseRelease(QMouseEvent*)));
    ui->customPlot_res->axisRect()->setupFullAxesBox();
    ui->customPlot_res->replot();
    }
}

//------------------------------------------------res----------------------------------------------------------
QColor  MainWindow::PrnColor(int prn)
{
   // QColor color[]={Qt::green,QColor(255,128,0,255),QColor(0,255,128,255),Qt::blue,Qt::red,Qt::gray};
    QColor color[]={QColor(0,120,40,255),QColor(255,128,0,255),QColor(0,255,128,255),Qt::blue,Qt::red,Qt::gray};
    unsigned int r1,g1,b1;
    QColor c1,c2;
    double a;
    int i;

    a=(prn-1)/10.0;
    i=static_cast<int>(a);
    a-=i;
    c1=color[3-i];
    c2=color[4-i];
    r1=static_cast<unsigned int>(a*c1.red()  +(1.0-a)*c2.red())&0xFF;
    g1=static_cast<unsigned int>(a*c1.green()+(1.0-a)*c2.green())&0xFF;
    b1=static_cast<unsigned int>(a*c1.blue() +(1.0-a)*c2.blue())&0xFF;

    return QColor(r1,g1,b1);
}


void MainWindow::changeplot()
{
    if (FreqType1++>NFREQ)
    {
        FreqType1=1;
        UpdatePlot();
    }
}

void MainWindow::timerEvent(QTimerEvent *)
{
    dateUpdate();
}


