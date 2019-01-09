/********************************************************************************
** Form generated from reading UI file 'serial.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERIAL_H
#define UI_SERIAL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_serial
{
public:
    QComboBox *BitRate;
    QComboBox *Parity;
    QLabel *Label3;
    QLabel *Label1;
    QComboBox *ByteSize;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *BtnOk;
    QPushButton *BtnCancel;
    QSpacerItem *horizontalSpacer_2;
    QLabel *Label4;
    QComboBox *FlowCtr;
    QComboBox *StopBits;
    QLabel *Label2;
    QLabel *Label5;
    QComboBox *Port;
    QLabel *Label8;

    void setupUi(QDialog *serial)
    {
        if (serial->objectName().isEmpty())
            serial->setObjectName(QStringLiteral("serial"));
        serial->resize(546, 261);
        BitRate = new QComboBox(serial);
        BitRate->setObjectName(QStringLiteral("BitRate"));
        BitRate->setEnabled(true);
        BitRate->setGeometry(QRect(130, 95, 126, 20));
        Parity = new QComboBox(serial);
        Parity->setObjectName(QStringLiteral("Parity"));
        Parity->setEnabled(true);
        Parity->setGeometry(QRect(390, 53, 126, 20));
        Label3 = new QLabel(serial);
        Label3->setObjectName(QStringLiteral("Label3"));
        Label3->setEnabled(true);
        Label3->setGeometry(QRect(26, 53, 126, 20));
        Label1 = new QLabel(serial);
        Label1->setObjectName(QStringLiteral("Label1"));
        Label1->setEnabled(true);
        Label1->setGeometry(QRect(26, 98, 126, 20));
        ByteSize = new QComboBox(serial);
        ByteSize->setObjectName(QStringLiteral("ByteSize"));
        ByteSize->setEnabled(true);
        ByteSize->setGeometry(QRect(130, 130, 126, 20));
        widget = new QWidget(serial);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(130, 190, 308, 41));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(58, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        BtnOk = new QPushButton(widget);
        BtnOk->setObjectName(QStringLiteral("BtnOk"));
        BtnOk->setEnabled(true);
        BtnOk->setAutoDefault(true);

        horizontalLayout_2->addWidget(BtnOk);

        BtnCancel = new QPushButton(widget);
        BtnCancel->setObjectName(QStringLiteral("BtnCancel"));
        BtnCancel->setEnabled(true);

        horizontalLayout_2->addWidget(BtnCancel);

        horizontalSpacer_2 = new QSpacerItem(58, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        Label4 = new QLabel(serial);
        Label4->setObjectName(QStringLiteral("Label4"));
        Label4->setEnabled(true);
        Label4->setGeometry(QRect(290, 53, 126, 20));
        FlowCtr = new QComboBox(serial);
        FlowCtr->setObjectName(QStringLiteral("FlowCtr"));
        FlowCtr->setEnabled(true);
        FlowCtr->setGeometry(QRect(390, 130, 126, 20));
        StopBits = new QComboBox(serial);
        StopBits->setObjectName(QStringLiteral("StopBits"));
        StopBits->setEnabled(true);
        StopBits->setGeometry(QRect(390, 98, 126, 20));
        Label2 = new QLabel(serial);
        Label2->setObjectName(QStringLiteral("Label2"));
        Label2->setEnabled(true);
        Label2->setGeometry(QRect(26, 130, 126, 20));
        Label5 = new QLabel(serial);
        Label5->setObjectName(QStringLiteral("Label5"));
        Label5->setEnabled(true);
        Label5->setGeometry(QRect(290, 98, 126, 20));
        Port = new QComboBox(serial);
        Port->setObjectName(QStringLiteral("Port"));
        Port->setEnabled(true);
        Port->setGeometry(QRect(130, 50, 126, 20));
        Label8 = new QLabel(serial);
        Label8->setObjectName(QStringLiteral("Label8"));
        Label8->setEnabled(true);
        Label8->setGeometry(QRect(290, 130, 126, 20));

        retranslateUi(serial);

        BitRate->setCurrentIndex(9);
        ByteSize->setCurrentIndex(1);
        BtnOk->setDefault(true);


        QMetaObject::connectSlotsByName(serial);
    } // setupUi

    void retranslateUi(QDialog *serial)
    {
        serial->setWindowTitle(QApplication::translate("serial", "Dialog", Q_NULLPTR));
        BitRate->clear();
        BitRate->insertItems(0, QStringList()
         << QApplication::translate("serial", "300", Q_NULLPTR)
         << QApplication::translate("serial", "600", Q_NULLPTR)
         << QApplication::translate("serial", "1200", Q_NULLPTR)
         << QApplication::translate("serial", "2400", Q_NULLPTR)
         << QApplication::translate("serial", "4800", Q_NULLPTR)
         << QApplication::translate("serial", "9600", Q_NULLPTR)
         << QApplication::translate("serial", "19200", Q_NULLPTR)
         << QApplication::translate("serial", "38400", Q_NULLPTR)
         << QApplication::translate("serial", "57600", Q_NULLPTR)
         << QApplication::translate("serial", "115200", Q_NULLPTR)
         << QApplication::translate("serial", "230400", Q_NULLPTR)
        );
#ifndef QT_NO_TOOLTIP
        BitRate->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        Parity->clear();
        Parity->insertItems(0, QStringList()
         << QApplication::translate("serial", "None", Q_NULLPTR)
         << QApplication::translate("serial", "Even", Q_NULLPTR)
         << QApplication::translate("serial", "Odd", Q_NULLPTR)
        );
#ifndef QT_NO_TOOLTIP
        Parity->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        Label3->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        Label3->setText(QApplication::translate("serial", "Port", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        Label1->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        Label1->setText(QApplication::translate("serial", "Bitrate (bps)", Q_NULLPTR));
        ByteSize->clear();
        ByteSize->insertItems(0, QStringList()
         << QApplication::translate("serial", "7 bits", Q_NULLPTR)
         << QApplication::translate("serial", "8 bits", Q_NULLPTR)
        );
#ifndef QT_NO_TOOLTIP
        ByteSize->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        BtnOk->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        BtnOk->setText(QApplication::translate("serial", "&OK", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        BtnCancel->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        BtnCancel->setText(QApplication::translate("serial", "&Cancel", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        Label4->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        Label4->setText(QApplication::translate("serial", "Parity", Q_NULLPTR));
        FlowCtr->clear();
        FlowCtr->insertItems(0, QStringList()
         << QApplication::translate("serial", "None", Q_NULLPTR)
         << QApplication::translate("serial", "RTS/CTS", Q_NULLPTR)
         << QApplication::translate("serial", "XON/XOFF", Q_NULLPTR)
        );
#ifndef QT_NO_TOOLTIP
        FlowCtr->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        StopBits->clear();
        StopBits->insertItems(0, QStringList()
         << QApplication::translate("serial", "1 bit", Q_NULLPTR)
         << QApplication::translate("serial", "2 bits", Q_NULLPTR)
        );
#ifndef QT_NO_TOOLTIP
        StopBits->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        Label2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        Label2->setText(QApplication::translate("serial", "Byte Size", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        Label5->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        Label5->setText(QApplication::translate("serial", "Stop Bits", Q_NULLPTR));
        Port->clear();
        Port->insertItems(0, QStringList()
         << QApplication::translate("serial", "COM1", Q_NULLPTR)
        );
#ifndef QT_NO_TOOLTIP
        Port->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        Label8->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        Label8->setText(QApplication::translate("serial", "Flow Control", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class serial: public Ui_serial {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERIAL_H
