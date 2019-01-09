#ifndef DIALOGCONFIG_H
#define DIALOGCONFIG_H

#include <QDialog>
#include <QShowEvent>
#include<serial.h>
#include<tcpconfig.h>
#include<QDebug>
#include <QFileDialog>
#include "src/rtklib.h"

namespace Ui {
class DialogConfig;
}

class DialogConfig : public QDialog
{
    Q_OBJECT
signals:
    void sigConfigOk();
public slots:
    void ConfigOk();
    void CheckRawData();
    void CheckSsrData();
    void CheckOutPutRaw();
    void CheckOutPutResult();
    void Btn1Click();
    void Btn2Click();
    void Btn3Click();
    void TideCorrBoxChange(int index);
    void Stream1BoxChange(int index);
    void Stream2BoxChange(int index);
    void Stream3BoxChange(int index);
    void FileRawOpt();
    void FileOrbitOpt();
    void FileClockOpt();
    void CheckSatpcvData();
    void FileSatPcvOpt();
    void CheckRecpcvData();
    void outResultPath();
    void SaveFileRawOpt();
public:
    explicit DialogConfig(QWidget *parent = 0);
    ~DialogConfig();
    QString RawPaths;
    QString OrbitPath;
    QString ClockPath;
    QString NetCorrPaths;
    QString OutputRawPath;
    QString OutputResultPath;

private:
    Ui::DialogConfig *ui;
    serial *myserial;
    TcpConfig *tcpDialog;
    QString FileFilter;
    void SerialOpt();
    void NetOpt();

};


#endif // DIALOGCONFIG_H
