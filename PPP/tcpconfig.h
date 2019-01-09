#ifndef TCPCONFIG_H
#define TCPCONFIG_H

#include <QDialog>

namespace Ui {
class TcpConfig;
}

class TcpConfig : public QDialog
{
    Q_OBJECT
public slots:
    void  btnclicked();
public:
    explicit TcpConfig(QWidget *parent = 0);
    ~TcpConfig();
    QString Path;

private:
    Ui::TcpConfig *ui;
    void setLabelAddr();

};

#endif // TCPCONFIG_H
