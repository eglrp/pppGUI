#include "dialogconfig.h"
#include "ui_dialogconfig.h"

extern prcopt_t prcopt1;
int tcpType;
int fileType;
int button;
extern int StreamType[8];
DialogConfig::DialogConfig(QWidget *parent) : QDialog(parent),
    ui(new Ui::DialogConfig)
{
    qDebug()<<"构造DialogConfig";
    ui->setupUi(this);
    this->show();
    connect(ui->OK,&QPushButton::clicked,this,&DialogConfig::ConfigOk);


    connect(ui->StreamRover,SIGNAL(stateChanged(int)),this,SLOT(CheckRawData()));
    connect(ui->StreamSSR,SIGNAL(stateChanged(int)),this,SLOT(CheckSsrData()));
    connect(ui->StreamOutput,SIGNAL(stateChanged(int)),this,SLOT(CheckOutPutRaw()));
    connect(ui->OutEventC,SIGNAL(stateChanged(int)),this,SLOT(CheckOutPutResult()));

    connect(ui->StreamRaw, SIGNAL(currentIndexChanged(int)), this, SLOT(Stream1BoxChange(int)));
    connect(ui->StreamEtrip, SIGNAL(currentIndexChanged(int)), this, SLOT(Stream2BoxChange(int)));
    connect(ui->StreamOutputRaw, SIGNAL(currentIndexChanged(int)), this, SLOT(Stream3BoxChange(int)));

    connect(ui->BtnStr1, SIGNAL(clicked()), this, SLOT(Btn1Click()));
    connect(ui->BtnStr2, SIGNAL(clicked()), this, SLOT(Btn2Click()));
    connect(ui->BtnStr3, SIGNAL(clicked()), this, SLOT(Btn3Click()));
    connect(ui->BtnCancel,SIGNAL(clicked(bool)),this,SLOT(reject()));

    connect(ui->BtnFile3,SIGNAL(clicked()),this,SLOT(outResultPath()));
    connect(ui->BtnFile1, SIGNAL(clicked()), this, SLOT(FileRawOpt()));
    connect(ui->BtnFile2, SIGNAL(clicked()), this, SLOT(SaveFileRawOpt()));
    connect(ui->Orbit, SIGNAL(clicked()), this, SLOT(FileOrbitOpt()));
    connect(ui->Clock, SIGNAL(clicked()), this, SLOT(FileClockOpt()));
    connect(ui->BtnSatPcvFile_2,SIGNAL(clicked()),this,SLOT(FileSatPcvOpt()));
    connect(ui->BtnSatPcvFile_2,SIGNAL(clicked()),this,SLOT(FileSatPcvOpt()));

    connect(ui->PosOpt1,SIGNAL(stateChanged(int)),this,SLOT(CheckSatpcvData()));


    connect(ui->TideCorr, SIGNAL(currentIndexChanged(int)), this, SLOT(TideCorrBoxChange(int)));

    ui->StreamRaw->setEnabled(false);
    ui->StreamEtrip->setEnabled(false);
    ui->StreamOutputRaw->setEnabled(false);
    ui->FilePath1->setEnabled(false);
    ui->FilePath2->setEnabled(false);
    ui->FilePath3->setEnabled(false);
    ui->BtnFile1->setEnabled(false);
    ui->BtnFile2->setEnabled(false);
    ui->BtnFile3->setEnabled(false);
    ui->BtnStr1->setEnabled(false);
    ui->BtnStr2->setEnabled(false);
    ui->BtnStr3->setEnabled(false);
    ui->FormatRover->setEnabled(false);
    ui->FormatSSR->setEnabled(false);

    ui->Orbit->setEnabled(false);
    ui->Clock->setEnabled(false);
    ui->OrbitFile->setEnabled(false);
    ui->ClockFile->setEnabled(false);
    ui->SatPcvFile_2->setEnabled(false);
    ui->BtnSatPcvFile_2->setEnabled(false);
    ui->AntPcvFile_2->setEnabled(false);
    ui->BtnAntPcvFile_2->setEnabled(false);
    ui->GeoidDataFile_2->setEnabled(false);
    ui->BtnGeoidDataFile_2->setEnabled(false);
   // ui->DCBFile_2->setEnabled(false);
   // ui->BtnDCBFile_2->setEnabled(false);
   // ui->EOPFile_2->setEnabled(false);
   // ui->BtnEOPFile_2->setEnabled(false);
   // ui->OLFile_2->setEnabled(false);
   // ui->BtnOLFile_2->setEnabled(false);
}

DialogConfig::~DialogConfig()
{
    qDebug()<<"析构DialogConfig";
    delete ui;
}

//-----配置完发送信号，主窗口接收到信号隐藏配置窗口---------
void DialogConfig::ConfigOk()
{
    int mode=ui->PosMode->currentIndex();
    switch (mode) {
    case 0:  prcopt1.mode=PMODE_SINGLE;     break;
    case 1:  prcopt1.mode=PMODE_PPP_KINEMA; break;
    case 2:  prcopt1.mode=PMODE_PPP_STATIC; break;
    default: prcopt1.mode=PMODE_PPP_KINEMA; break;
    }
    int freq=ui->Freq->currentIndex();
    switch (freq) {
    case 0:  prcopt1.nf=1; break;
    case 1:  prcopt1.nf=2; break;
    default: prcopt1.nf=1; break;
    }
    int elmin=ui->ElMask->currentIndex();
    switch (elmin) {
    case 0:  prcopt1.elmin=0.0*D2R;   break;
    case 1:  prcopt1.elmin=5.0*D2R;   break;
    case 2:  prcopt1.elmin=10.0*D2R;  break;
    case 3:  prcopt1.elmin=15.0*D2R;  break;
    case 4:  prcopt1.elmin=20.0*D2R;  break;
    case 5:  prcopt1.elmin=25.0*D2R;  break;
    case 6:  prcopt1.elmin=30.0*D2R;  break;
    case 7:  prcopt1.elmin=35.0*D2R;  break;
    case 8:  prcopt1.elmin=40.0*D2R;  break;
    case 9:  prcopt1.elmin=45.0*D2R;  break;
    case 10:  prcopt1.elmin=50.0*D2R; break;
    case 11:  prcopt1.elmin=55.0*D2R; break;
    case 12:  prcopt1.elmin=60.0*D2R; break;
    case 13:  prcopt1.elmin=65.0*D2R; break;
     case 14: prcopt1.elmin=70.0*D2R; break;
    default: prcopt1.elmin=15.5*D2R; break;
    }

    int tide=ui->TideCorr->currentIndex();
    switch (tide) {
    case 0:  prcopt1.tidecorr=0;  break;
    case 1:  prcopt1.tidecorr=1;  break;
    case 2:  prcopt1.tidecorr=2;  break;
    default: prcopt1.tidecorr=1;  break;
    }
    int ionoOpt=ui->IonoOpt->currentIndex();
    switch (ionoOpt) {
    case 0:  prcopt1.ionoopt=IONOOPT_OFF;  break;
    case 1:  prcopt1.ionoopt=IONOOPT_BRDC; break;
    case 2:  prcopt1.ionoopt=IONOOPT_IFLC; break;
    case 3:  prcopt1.ionoopt=IONOOPT_EST;  break;
    case 4:  prcopt1.ionoopt=IONOOPT_TEC;  break;
    default: prcopt1.ionoopt=IONOOPT_BRDC; break;
    }
    int tropOpt=ui->TropOpt->currentIndex();
    switch (tropOpt) {
    case 0:  prcopt1.tropopt=TROPOPT_OFF;  break;
    case 1:  prcopt1.tropopt=TROPOPT_SAAS; break;
    case 2:  prcopt1.tropopt=TROPOPT_EST;  break;
    case 3:  prcopt1.tropopt=TROPOPT_ESTG; break;
    default: prcopt1.tropopt=TROPOPT_SAAS; break;
    }
    int sateph=ui->SatEphem->currentIndex();
    switch (sateph) {
    case 0:  prcopt1.sateph=EPHOPT_BRDC;  break;
    case 1:  prcopt1.sateph=EPHOPT_PREC;  break;
    case 2:  prcopt1.sateph=EPHOPT_SSRAPC;  break;
    case 3:  prcopt1.sateph=EPHOPT_SSRCOM;  break;
    default: prcopt1.tropopt=EPHOPT_BRDC;  break;
    }

    int gps=(ui->NavSys1->checkState())?SYS_GPS:SYS_NONE;
    int glonass=(ui->NavSys2->checkState())?SYS_GLO:SYS_NONE;
    int galileo=(ui->NavSys3->checkState())?SYS_GAL:SYS_NONE;
    int bds=(ui->NavSys4->checkState())?SYS_CMP:SYS_NONE;
    prcopt1.navsys=gps|glonass|galileo|bds;

    // 发送信号
    emit sigConfigOk();
}


void DialogConfig::CheckSatpcvData()
{
    ui->SatPcvFile_2->setEnabled(ui->PosOpt1->isChecked());
    ui->BtnSatPcvFile_2->setEnabled(ui->PosOpt1->isChecked());
}
void DialogConfig::CheckRecpcvData()
{
    ui->AntPcvFile_2->setEnabled(ui->PosOpt2->isChecked());
    ui->BtnAntPcvFile_2->setEnabled(ui->PosOpt2->isChecked());
}

//--------------根据CheckBox将配置相关按钮置灰或高亮-------------
void DialogConfig::CheckRawData()
{
    ui->StreamRaw->setEnabled(ui->StreamRover->isChecked());
    ui->BtnStr1->setEnabled(ui->StreamRover->isChecked()&&ui->StreamRaw->currentIndex()!=STR_FILE);

    ui->FilePath1->setEnabled(ui->StreamRover->isChecked()&&ui->StreamRaw->currentIndex()==STR_FILE);
    ui->BtnFile1->setEnabled(ui->StreamRover->isChecked()&&ui->StreamRaw->currentIndex()==STR_FILE);

    ui->FormatRover->setEnabled(ui->StreamRover->isChecked());

}

void DialogConfig::CheckSsrData()
{
    ui->StreamEtrip->setEnabled(ui->StreamSSR->isChecked());
    ui->BtnStr2->setEnabled(ui->StreamSSR->isChecked());
    ui->FormatSSR->setEnabled(ui->StreamSSR->isChecked());
}

void DialogConfig::CheckOutPutRaw()
{
    ui->StreamOutputRaw->setEnabled(ui->StreamOutput->isChecked());
    ui->BtnStr3->setEnabled(ui->StreamOutput->isChecked()&&ui->StreamOutputRaw->currentIndex()!=STR_FILE);

    ui->FilePath2->setEnabled(ui->StreamOutput->isChecked()&&ui->StreamOutputRaw->currentIndex()==STR_FILE);
    ui->BtnFile2->setEnabled(ui->StreamOutput->isChecked()&&ui->StreamOutputRaw->currentIndex()==STR_FILE);
}
void DialogConfig::CheckOutPutResult()
{
    ui->FilePath3->setEnabled(ui->OutEventC->isChecked());
    ui->BtnFile3->setEnabled(ui->OutEventC->isChecked());
}

//-----------ComboBox值法改变对应的操作-------------
void DialogConfig::TideCorrBoxChange(int index)
{
    if(index!=0)
    {
        ui->GeoidDataFile_2->setEnabled(ui->PosOpt2->isChecked());
        ui->BtnGeoidDataFile_2->setEnabled(ui->PosOpt2->isChecked());
    }
}

void DialogConfig::Stream1BoxChange(int index)
{
    if(index==STR_FILE)  StreamType[0]=STR_FILE;
    ui->BtnStr1->setEnabled(ui->StreamRover->isChecked()&&index!=STR_FILE);
    ui->FilePath1->setEnabled(ui->StreamRover->isChecked()&&index==STR_FILE);
    ui->BtnFile1->setEnabled(ui->StreamRover->isChecked()&&index==STR_FILE);
}

void DialogConfig::Stream2BoxChange(int index)
{
    if(index==STR_FILE) StreamType[2]=STR_FILE;
    ui->BtnStr2->setEnabled(ui->StreamSSR->isChecked()&&index!=STR_FILE);
    ui->Orbit->setEnabled(ui->StreamSSR->isChecked()&&index==STR_FILE);
    ui->Clock->setEnabled(ui->StreamSSR->isChecked()&&index==STR_FILE);
    ui->OrbitFile->setEnabled(ui->StreamSSR->isChecked()&&index==STR_FILE);
    ui->ClockFile->setEnabled(ui->StreamSSR->isChecked()&&index==STR_FILE);
}

void DialogConfig::Stream3BoxChange(int index)
{
    if(index==STR_FILE) StreamType[4]=STR_FILE;
    ui->BtnStr3->setEnabled(ui->StreamOutput->isChecked()&&index!=STR_FILE);
    ui->FilePath2->setEnabled(ui->StreamOutput->isChecked()&&index==STR_FILE);
    ui->BtnFile2->setEnabled(ui->StreamOutput->isChecked()&&index==STR_FILE);
}

//---------------------------------------------------------------------------
void DialogConfig::outResultPath()
{
    OutputResultPath = QFileDialog::getExistingDirectory( this, QString("Load file"));
    ui->FilePath3->setText(OutputResultPath);
}

void DialogConfig::FileSatPcvOpt()
{

}
void DialogConfig::FileRawOpt()
{
    RawPaths = QFileDialog::getOpenFileName(this, QString("Load file"));
    ui->FilePath1->setText(RawPaths);
}
void DialogConfig::SaveFileRawOpt()
{
    OutputRawPath = QFileDialog::getOpenFileName(this, QString("Load file"));
    ui->FilePath2->setText(OutputRawPath);
}
void DialogConfig::FileOrbitOpt()
{
    OrbitPath = QFileDialog::getOpenFileName( this, QString("Load file"));
    ui->OrbitFile->setText(OrbitPath);
}
void DialogConfig::FileClockOpt()
{
    ClockPath = QFileDialog::getOpenFileName( this, QString("Load file"));
    ui->ClockFile->setText(ClockPath);
}

void DialogConfig::SerialOpt()
{
    myserial =new serial(this);
    myserial->exec();
    if(myserial->result() == QDialog::Accepted)
    {
        if(button==1)
            RawPaths = myserial->Path;
         else if(button==2)
            NetCorrPaths = myserial->Path;
        else
            OutputRawPath = myserial->Path;
    }
    if(myserial->result() == QDialog::Accepted||myserial->result()==QDialog::Rejected)
        delete myserial;
   // qDebug()<<RawPaths;
}

void DialogConfig::NetOpt()
{
    tcpDialog = new TcpConfig(this);
    tcpDialog->exec();
    if(tcpDialog->result() == QDialog::Accepted)
    {
        if(button==1)
            RawPaths = tcpDialog->Path;
         else if(button==2)
            NetCorrPaths = tcpDialog->Path;
        else
            OutputRawPath = tcpDialog->Path;
    }
    if(tcpDialog->result() == QDialog::Accepted||tcpDialog->result()==QDialog::Rejected)
        delete tcpDialog;
   //qDebug()<<NetCorrPaths;
}

//----------raw data path--------------
void DialogConfig::Btn1Click()
{
    button=1;
    int index=ui->StreamRaw->currentIndex();
    switch ( index )
    {
    case STR_SERIAL:
        { 
            StreamType[0]=STR_SERIAL;
            SerialOpt();
            break;
        }
    case STR_TCPCLI:
        {
            StreamType[0]=STR_TCPCLI;
            tcpType=1;
            NetOpt();
            break;
        }
        break;
    default:break;
    }
}

//----------correction data path--------------
void DialogConfig::Btn2Click()
{
    button=2;
    int index=ui->StreamEtrip->currentIndex();
    switch ( index )
    {
    case STR_NTRIPCLI:
        {
            StreamType[2]=STR_NTRIPCLI;
            tcpType=0;
            NetOpt();
            break;
        }
    case STR_FILE:
        break;
    default:break;
    }
}

void DialogConfig::Btn3Click()
{
    button=3;
    int index=ui->StreamOutputRaw->currentIndex();
    switch ( index )
    {
    case STR_TCPCLI:
        {
            StreamType[5]=STR_TCPCLI;
            tcpType=1;
            NetOpt();
            break;
        }
    case STR_TCPSVR:
    {
        StreamType[5]=STR_TCPSVR;
        tcpType=2;
        NetOpt();
        break;
    }
    default:break;
    }
}



