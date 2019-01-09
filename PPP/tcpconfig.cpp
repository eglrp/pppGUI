#include "tcpconfig.h"
#include "ui_tcpconfig.h"
#include<QDebug>

extern int tcpType;
TcpConfig::TcpConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TcpConfig)
{
    qDebug()<<"构造TcpConfig";
    ui->setupUi(this);
    this->show();
    setLabelAddr();

    connect(ui->BtnOk,SIGNAL(clicked(bool)),this,SLOT(btnclicked()));
    connect(ui->BtnCancel,SIGNAL(clicked(bool)),this,SLOT(reject()));
}

TcpConfig::~TcpConfig()
{
    qDebug()<<"析构TcpConfig";
    delete ui;
}
void TcpConfig::setLabelAddr()
{
    //------------设置默认值----------
    if(tcpType==0)
    {
        ui->LabelAddr->setText("ETRIP Caster Host");
        ui->Addr->addItem("ntrip.gnsslab.cn");
        ui->Port->setText("2101");
        ui->Mountpoint->setText("IGS01");
        ui->User->setText("lei1101");
        ui->Passwd->setText("199391");
    }
    if(tcpType==1)
    {
        ui->LabelAddr->setText("TCP Client Address");
        ui->Mountpoint->setEnabled(false);
        ui->User->setEnabled(false);
        ui->Passwd->setEnabled(false);
        ui->BtnNtrip->setEnabled(false);
    }
    if(tcpType==2)
    {
        ui->LabelAddr->setText("TCP Server Address");
        ui->Addr->setEnabled(false);
        ui->Mountpoint->setEnabled(false);
        ui->User->setEnabled(false);
        ui->Passwd->setEnabled(false);
        ui->BtnNtrip->setEnabled(false);
    }
}

void  TcpConfig::btnclicked()
{

    if(tcpType==0)
        Path=QString("%1:%2:%3").arg(ui->User->text()).
                arg(ui->Passwd->text()+'@'+ui->Addr->currentText()).
                arg(ui->Port->text()+'/'+ui->Mountpoint->text());
    if(tcpType==1)
        Path=QString("%1:%2").arg(ui->Addr->currentText()).arg(ui->Port->text());
    if(tcpType==2)
        Path=QString("%1:%2").arg("").arg(ui->Port->text());

    accept();
}

