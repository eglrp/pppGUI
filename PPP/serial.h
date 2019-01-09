#ifndef SERIAL_H
#define SERIAL_H

#include <QDialog>
#include<QWidget>
#include <QCoreApplication>
#include <QSerialPort>
#include<QSerialPortInfo>
#include <qevent.h>

namespace Ui {
class serial;
}

class serial : public QDialog
{
    Q_OBJECT
protected:
    void  showEvent(QShowEvent *);
    void  UpdatePortList(void);
public slots:
    void  btnclicked();
public:

    explicit serial(QWidget *parent = 0);
    ~serial();
    QString Path;

private:
    Ui::serial *ui;
};



#endif // SERIAL_H
