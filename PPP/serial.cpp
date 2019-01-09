#include "serial.h"
#include "ui_serial.h"
#include<QDebug>
#include<QFileDialog>
serial::serial(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::serial)
{
    qDebug()<<"构造serial";
    ui->setupUi(this);
    this->show();
    connect(ui->BtnOk,SIGNAL(clicked(bool)),this,SLOT(btnclicked()));
    connect(ui->BtnCancel,SIGNAL(clicked(bool)),this,SLOT(reject()));
}

serial::~serial()
{
    qDebug()<<"析构serial";
    delete ui;
}

//---------------------------------------------------------------------------
void  serial::showEvent(QShowEvent *event)
{
    if (event->spontaneous()) return;

    UpdatePortList();
    QStringList tokens=Path.split(':');

    ui->Port->setCurrentIndex(ui->Port->findText(tokens.first()));

    if (tokens.size()<2) return;
    ui->BitRate->setCurrentIndex(ui->BitRate->findText(tokens.at(1)));

    if (tokens.size()<3) return;
    ui->ByteSize->setCurrentIndex(tokens.at(2)=="7"?0:1);

    if (tokens.size()<4) return;
    ui->Parity->setCurrentIndex(tokens.at(3)=="n"?0:tokens.at(3)=="e"?1:2);

    if (tokens.size()<5) return;
    ui->StopBits->setCurrentIndex(tokens.at(4)=="1"?0:1);

    if (tokens.size()<6) return;
    ui->FlowCtr->setCurrentIndex(tokens.at(5)=="off"?0:tokens.at(5)=="rts"?1:2);

}

void  serial::btnclicked()
{
    char const *parity[]={"n","e","o"},*fctr[]={"off","rts","xon"};
    QString Port_Text=ui->Port->currentText(),BitRate_Text=ui->BitRate->currentText();
    Path=QString("%1:%2:%3:%4:%5:%6").arg(Port_Text).arg(BitRate_Text)
            .arg(ui->ByteSize->currentIndex()?8:7).arg(parity[ui->Parity->currentIndex()])
            .arg(ui->StopBits->currentIndex()?2:1).arg(fctr[ui->FlowCtr->currentIndex()]);
    accept();
}

//---------获取电脑可能串口信息----------
void  serial::UpdatePortList(void)
{
    ui->Port->clear();
#ifdef QEXTSERIALPORT
    QList<QextPortInfo>  ports=QextSerialEnumerator::getPorts();

    for (int i=0;i<ports.size();i++)
    {
        ui->Port->addItem(ports.at(i).portName);
    }
#else
    QList<QSerialPortInfo>  ports=QSerialPortInfo::availablePorts();

    for (int i=0;i<ports.size();i++)
    {
        ui->Port->addItem(ports.at(i).portName());
    }
#endif
}
