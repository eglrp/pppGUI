/********************************************************************************
** Form generated from reading UI file 'tcpconfig.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TCPCONFIG_H
#define UI_TCPCONFIG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TcpConfig
{
public:
    QLabel *LabelAddr;
    QComboBox *Addr;
    QLabel *LabelPort;
    QLineEdit *User;
    QLabel *LabelUser;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *BtnNtrip;
    QPushButton *BtnOk;
    QPushButton *BtnCancel;
    QLabel *LabelMntPnt;
    QLineEdit *Port;
    QLineEdit *Passwd;
    QLabel *LabelPasswd;
    QLineEdit *Mountpoint;

    void setupUi(QDialog *TcpConfig)
    {
        if (TcpConfig->objectName().isEmpty())
            TcpConfig->setObjectName(QStringLiteral("TcpConfig"));
        TcpConfig->resize(504, 242);
        LabelAddr = new QLabel(TcpConfig);
        LabelAddr->setObjectName(QStringLiteral("LabelAddr"));
        LabelAddr->setEnabled(true);
        LabelAddr->setGeometry(QRect(46, 24, 248, 34));
        Addr = new QComboBox(TcpConfig);
        Addr->setObjectName(QStringLiteral("Addr"));
        Addr->setEnabled(true);
        Addr->setGeometry(QRect(46, 64, 248, 20));
        Addr->setEditable(true);
        LabelPort = new QLabel(TcpConfig);
        LabelPort->setObjectName(QStringLiteral("LabelPort"));
        LabelPort->setEnabled(true);
        LabelPort->setGeometry(QRect(300, 24, 166, 34));
        User = new QLineEdit(TcpConfig);
        User->setObjectName(QStringLiteral("User"));
        User->setEnabled(true);
        User->setGeometry(QRect(127, 130, 167, 20));
        LabelUser = new QLabel(TcpConfig);
        LabelUser->setObjectName(QStringLiteral("LabelUser"));
        LabelUser->setEnabled(true);
        LabelUser->setGeometry(QRect(127, 90, 167, 34));
        widget = new QWidget(TcpConfig);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(46, 182, 420, 34));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        BtnNtrip = new QPushButton(widget);
        BtnNtrip->setObjectName(QStringLiteral("BtnNtrip"));
        BtnNtrip->setEnabled(true);

        horizontalLayout->addWidget(BtnNtrip);

        BtnOk = new QPushButton(widget);
        BtnOk->setObjectName(QStringLiteral("BtnOk"));
        BtnOk->setEnabled(true);
        BtnOk->setAutoDefault(true);

        horizontalLayout->addWidget(BtnOk);

        BtnCancel = new QPushButton(widget);
        BtnCancel->setObjectName(QStringLiteral("BtnCancel"));
        BtnCancel->setEnabled(true);

        horizontalLayout->addWidget(BtnCancel);

        LabelMntPnt = new QLabel(TcpConfig);
        LabelMntPnt->setObjectName(QStringLiteral("LabelMntPnt"));
        LabelMntPnt->setEnabled(true);
        LabelMntPnt->setGeometry(QRect(46, 90, 69, 34));
        Port = new QLineEdit(TcpConfig);
        Port->setObjectName(QStringLiteral("Port"));
        Port->setEnabled(true);
        Port->setGeometry(QRect(300, 64, 166, 20));
        Passwd = new QLineEdit(TcpConfig);
        Passwd->setObjectName(QStringLiteral("Passwd"));
        Passwd->setEnabled(true);
        Passwd->setGeometry(QRect(300, 130, 166, 20));
        Passwd->setEchoMode(QLineEdit::Password);
        LabelPasswd = new QLabel(TcpConfig);
        LabelPasswd->setObjectName(QStringLiteral("LabelPasswd"));
        LabelPasswd->setEnabled(true);
        LabelPasswd->setGeometry(QRect(300, 90, 166, 34));
        Mountpoint = new QLineEdit(TcpConfig);
        Mountpoint->setObjectName(QStringLiteral("Mountpoint"));
        Mountpoint->setEnabled(true);
        Mountpoint->setGeometry(QRect(40, 130, 71, 20));

        retranslateUi(TcpConfig);

        BtnOk->setDefault(true);


        QMetaObject::connectSlotsByName(TcpConfig);
    } // setupUi

    void retranslateUi(QDialog *TcpConfig)
    {
        TcpConfig->setWindowTitle(QApplication::translate("TcpConfig", "Dialog", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        LabelAddr->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        LabelAddr->setText(QApplication::translate("TcpConfig", "TCP Client Address", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        Addr->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        LabelPort->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        LabelPort->setText(QApplication::translate("TcpConfig", "Port", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        User->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        User->setText(QString());
#ifndef QT_NO_TOOLTIP
        LabelUser->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        LabelUser->setText(QApplication::translate("TcpConfig", "User-ID", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        BtnNtrip->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        BtnNtrip->setText(QApplication::translate("TcpConfig", "&Ntrip...", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        BtnOk->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        BtnOk->setText(QApplication::translate("TcpConfig", "&OK", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        BtnCancel->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        BtnCancel->setText(QApplication::translate("TcpConfig", "&Cancel", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        LabelMntPnt->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        LabelMntPnt->setText(QApplication::translate("TcpConfig", "Mountpoint", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        Port->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        Port->setText(QString());
#ifndef QT_NO_TOOLTIP
        Passwd->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        Passwd->setText(QString());
#ifndef QT_NO_TOOLTIP
        LabelPasswd->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        LabelPasswd->setText(QApplication::translate("TcpConfig", "Password", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        Mountpoint->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        Mountpoint->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class TcpConfig: public Ui_TcpConfig {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TCPCONFIG_H
