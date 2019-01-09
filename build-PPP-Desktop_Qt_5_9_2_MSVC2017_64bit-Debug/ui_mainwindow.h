/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionopen;
    QAction *actionupdate;
    QAction *actionFreq;
    QAction *actionsConfig;
    QAction *actionsPost;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_9;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout_6;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_7;
    QWidget *widget_9;
    QHBoxLayout *horizontalLayout_8;
    QWidget *widget_4;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_longitude;
    QLabel *label_latitude;
    QLabel *label_height;
    QLabel *label_speed;
    QLabel *label_acceleration;
    QLabel *label_UTC;
    QLabel *label_GPS;
    QWidget *widget_6;
    QVBoxLayout *verticalLayout_10;
    QLabel *longitude;
    QLabel *latitude;
    QLabel *height;
    QLabel *speed;
    QLabel *acceleration;
    QLabel *UTC;
    QLabel *GPS;
    QWidget *widget_8;
    QHBoxLayout *horizontalLayout_4;
    QWidget *widget_5;
    QVBoxLayout *verticalLayout_12;
    QLabel *label_system;
    QLabel *label_nstas;
    QLabel *label_positon_mode;
    QLabel *label_GDOP;
    QLabel *label_PDOP;
    QLabel *label_HDOP;
    QLabel *label_VDOP;
    QWidget *widget_7;
    QVBoxLayout *verticalLayout_11;
    QLabel *system;
    QLabel *nstas;
    QLabel *positon_mode;
    QLabel *GDOP;
    QLabel *PDOP;
    QLabel *HDOP;
    QLabel *VDOP;
    QWidget *widget_Xyz;
    QVBoxLayout *verticalLayout_5;
    QWidget *widget_Xyz_2;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_8;
    QLabel *label_xyz_plot;
    QSpacerItem *horizontalSpacer_5;
    QCustomPlot *customPlot_xyz;
    QWidget *widget_3;
    QVBoxLayout *verticalLayout_7;
    QWidget *widget_Cn0;
    QVBoxLayout *verticalLayout;
    QWidget *widget_Cn0_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_10;
    QLabel *label_cno_plot;
    QSpacerItem *horizontalSpacer;
    QLabel *DispSnr;
    QWidget *widget_Cep;
    QVBoxLayout *verticalLayout_4;
    QWidget *widget_Cep_2;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label_nstas_plot;
    QSpacerItem *horizontalSpacer_3;
    QCustomPlot *customPlot_nstas;
    QCustomPlot *customPlot_dop;
    QWidget *widget_10;
    QVBoxLayout *verticalLayout_8;
    QWidget *widget_Sky;
    QVBoxLayout *verticalLayout_2;
    QWidget *widget_Sky_2;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_9;
    QLabel *label_sky_plot;
    QSpacerItem *horizontalSpacer_2;
    QLabel *DispSky;
    QWidget *widget_Other;
    QVBoxLayout *verticalLayout_9;
    QWidget *widget_Other_2;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer_7;
    QLabel *label_res_plot;
    QSpacerItem *horizontalSpacer_6;
    QCustomPlot *customPlot_res;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QMenu *menu_4;
    QMenu *menu_5;
    QStatusBar *statusBar;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(903, 523);
        actionopen = new QAction(MainWindow);
        actionopen->setObjectName(QStringLiteral("actionopen"));
        actionupdate = new QAction(MainWindow);
        actionupdate->setObjectName(QStringLiteral("actionupdate"));
        actionFreq = new QAction(MainWindow);
        actionFreq->setObjectName(QStringLiteral("actionFreq"));
        actionsConfig = new QAction(MainWindow);
        actionsConfig->setObjectName(QStringLiteral("actionsConfig"));
        actionsPost = new QAction(MainWindow);
        actionsPost->setObjectName(QStringLiteral("actionsPost"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout_9 = new QHBoxLayout(centralWidget);
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        widget_2 = new QWidget(centralWidget);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        verticalLayout_6 = new QVBoxLayout(widget_2);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        widget = new QWidget(widget_2);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout_7 = new QHBoxLayout(widget);
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        widget_9 = new QWidget(widget);
        widget_9->setObjectName(QStringLiteral("widget_9"));
        horizontalLayout_8 = new QHBoxLayout(widget_9);
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        widget_4 = new QWidget(widget_9);
        widget_4->setObjectName(QStringLiteral("widget_4"));
        verticalLayout_3 = new QVBoxLayout(widget_4);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label_longitude = new QLabel(widget_4);
        label_longitude->setObjectName(QStringLiteral("label_longitude"));

        verticalLayout_3->addWidget(label_longitude);

        label_latitude = new QLabel(widget_4);
        label_latitude->setObjectName(QStringLiteral("label_latitude"));

        verticalLayout_3->addWidget(label_latitude);

        label_height = new QLabel(widget_4);
        label_height->setObjectName(QStringLiteral("label_height"));

        verticalLayout_3->addWidget(label_height);

        label_speed = new QLabel(widget_4);
        label_speed->setObjectName(QStringLiteral("label_speed"));

        verticalLayout_3->addWidget(label_speed);

        label_acceleration = new QLabel(widget_4);
        label_acceleration->setObjectName(QStringLiteral("label_acceleration"));

        verticalLayout_3->addWidget(label_acceleration);

        label_UTC = new QLabel(widget_4);
        label_UTC->setObjectName(QStringLiteral("label_UTC"));

        verticalLayout_3->addWidget(label_UTC);

        label_GPS = new QLabel(widget_4);
        label_GPS->setObjectName(QStringLiteral("label_GPS"));

        verticalLayout_3->addWidget(label_GPS);


        horizontalLayout_8->addWidget(widget_4);

        widget_6 = new QWidget(widget_9);
        widget_6->setObjectName(QStringLiteral("widget_6"));
        verticalLayout_10 = new QVBoxLayout(widget_6);
        verticalLayout_10->setSpacing(6);
        verticalLayout_10->setContentsMargins(11, 11, 11, 11);
        verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
        longitude = new QLabel(widget_6);
        longitude->setObjectName(QStringLiteral("longitude"));

        verticalLayout_10->addWidget(longitude);

        latitude = new QLabel(widget_6);
        latitude->setObjectName(QStringLiteral("latitude"));

        verticalLayout_10->addWidget(latitude);

        height = new QLabel(widget_6);
        height->setObjectName(QStringLiteral("height"));

        verticalLayout_10->addWidget(height);

        speed = new QLabel(widget_6);
        speed->setObjectName(QStringLiteral("speed"));

        verticalLayout_10->addWidget(speed);

        acceleration = new QLabel(widget_6);
        acceleration->setObjectName(QStringLiteral("acceleration"));

        verticalLayout_10->addWidget(acceleration);

        UTC = new QLabel(widget_6);
        UTC->setObjectName(QStringLiteral("UTC"));

        verticalLayout_10->addWidget(UTC);

        GPS = new QLabel(widget_6);
        GPS->setObjectName(QStringLiteral("GPS"));

        verticalLayout_10->addWidget(GPS);


        horizontalLayout_8->addWidget(widget_6);


        horizontalLayout_7->addWidget(widget_9);

        widget_8 = new QWidget(widget);
        widget_8->setObjectName(QStringLiteral("widget_8"));
        horizontalLayout_4 = new QHBoxLayout(widget_8);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        widget_5 = new QWidget(widget_8);
        widget_5->setObjectName(QStringLiteral("widget_5"));
        verticalLayout_12 = new QVBoxLayout(widget_5);
        verticalLayout_12->setSpacing(6);
        verticalLayout_12->setContentsMargins(11, 11, 11, 11);
        verticalLayout_12->setObjectName(QStringLiteral("verticalLayout_12"));
        label_system = new QLabel(widget_5);
        label_system->setObjectName(QStringLiteral("label_system"));

        verticalLayout_12->addWidget(label_system);

        label_nstas = new QLabel(widget_5);
        label_nstas->setObjectName(QStringLiteral("label_nstas"));

        verticalLayout_12->addWidget(label_nstas);

        label_positon_mode = new QLabel(widget_5);
        label_positon_mode->setObjectName(QStringLiteral("label_positon_mode"));

        verticalLayout_12->addWidget(label_positon_mode);

        label_GDOP = new QLabel(widget_5);
        label_GDOP->setObjectName(QStringLiteral("label_GDOP"));

        verticalLayout_12->addWidget(label_GDOP);

        label_PDOP = new QLabel(widget_5);
        label_PDOP->setObjectName(QStringLiteral("label_PDOP"));

        verticalLayout_12->addWidget(label_PDOP);

        label_HDOP = new QLabel(widget_5);
        label_HDOP->setObjectName(QStringLiteral("label_HDOP"));

        verticalLayout_12->addWidget(label_HDOP);

        label_VDOP = new QLabel(widget_5);
        label_VDOP->setObjectName(QStringLiteral("label_VDOP"));

        verticalLayout_12->addWidget(label_VDOP);


        horizontalLayout_4->addWidget(widget_5);

        widget_7 = new QWidget(widget_8);
        widget_7->setObjectName(QStringLiteral("widget_7"));
        verticalLayout_11 = new QVBoxLayout(widget_7);
        verticalLayout_11->setSpacing(6);
        verticalLayout_11->setContentsMargins(11, 11, 11, 11);
        verticalLayout_11->setObjectName(QStringLiteral("verticalLayout_11"));
        system = new QLabel(widget_7);
        system->setObjectName(QStringLiteral("system"));

        verticalLayout_11->addWidget(system);

        nstas = new QLabel(widget_7);
        nstas->setObjectName(QStringLiteral("nstas"));

        verticalLayout_11->addWidget(nstas);

        positon_mode = new QLabel(widget_7);
        positon_mode->setObjectName(QStringLiteral("positon_mode"));

        verticalLayout_11->addWidget(positon_mode);

        GDOP = new QLabel(widget_7);
        GDOP->setObjectName(QStringLiteral("GDOP"));

        verticalLayout_11->addWidget(GDOP);

        PDOP = new QLabel(widget_7);
        PDOP->setObjectName(QStringLiteral("PDOP"));

        verticalLayout_11->addWidget(PDOP);

        HDOP = new QLabel(widget_7);
        HDOP->setObjectName(QStringLiteral("HDOP"));

        verticalLayout_11->addWidget(HDOP);

        VDOP = new QLabel(widget_7);
        VDOP->setObjectName(QStringLiteral("VDOP"));

        verticalLayout_11->addWidget(VDOP);


        horizontalLayout_4->addWidget(widget_7);


        horizontalLayout_7->addWidget(widget_8);


        verticalLayout_6->addWidget(widget);

        widget_Xyz = new QWidget(widget_2);
        widget_Xyz->setObjectName(QStringLiteral("widget_Xyz"));
        verticalLayout_5 = new QVBoxLayout(widget_Xyz);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        widget_Xyz_2 = new QWidget(widget_Xyz);
        widget_Xyz_2->setObjectName(QStringLiteral("widget_Xyz_2"));
        widget_Xyz_2->setMaximumSize(QSize(16777215, 20));
        horizontalLayout_5 = new QHBoxLayout(widget_Xyz_2);
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_8 = new QSpacerItem(141, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_8);

        label_xyz_plot = new QLabel(widget_Xyz_2);
        label_xyz_plot->setObjectName(QStringLiteral("label_xyz_plot"));

        horizontalLayout_5->addWidget(label_xyz_plot);

        horizontalSpacer_5 = new QSpacerItem(141, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_5);


        verticalLayout_5->addWidget(widget_Xyz_2);

        customPlot_xyz = new QCustomPlot(widget_Xyz);
        customPlot_xyz->setObjectName(QStringLiteral("customPlot_xyz"));

        verticalLayout_5->addWidget(customPlot_xyz);


        verticalLayout_6->addWidget(widget_Xyz);


        horizontalLayout_9->addWidget(widget_2);

        widget_3 = new QWidget(centralWidget);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        verticalLayout_7 = new QVBoxLayout(widget_3);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        widget_Cn0 = new QWidget(widget_3);
        widget_Cn0->setObjectName(QStringLiteral("widget_Cn0"));
        verticalLayout = new QVBoxLayout(widget_Cn0);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        widget_Cn0_2 = new QWidget(widget_Cn0);
        widget_Cn0_2->setObjectName(QStringLiteral("widget_Cn0_2"));
        widget_Cn0_2->setMaximumSize(QSize(16777215, 20));
        horizontalLayout = new QHBoxLayout(widget_Cn0_2);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_10 = new QSpacerItem(100, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_10);

        label_cno_plot = new QLabel(widget_Cn0_2);
        label_cno_plot->setObjectName(QStringLiteral("label_cno_plot"));

        horizontalLayout->addWidget(label_cno_plot);

        horizontalSpacer = new QSpacerItem(100, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addWidget(widget_Cn0_2);

        DispSnr = new QLabel(widget_Cn0);
        DispSnr->setObjectName(QStringLiteral("DispSnr"));
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DispSnr->sizePolicy().hasHeightForWidth());
        DispSnr->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(DispSnr);


        verticalLayout_7->addWidget(widget_Cn0);

        widget_Cep = new QWidget(widget_3);
        widget_Cep->setObjectName(QStringLiteral("widget_Cep"));
        verticalLayout_4 = new QVBoxLayout(widget_Cep);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        widget_Cep_2 = new QWidget(widget_Cep);
        widget_Cep_2->setObjectName(QStringLiteral("widget_Cep_2"));
        widget_Cep_2->setMaximumSize(QSize(16777215, 20));
        horizontalLayout_3 = new QHBoxLayout(widget_Cep_2);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_4 = new QSpacerItem(94, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);

        label_nstas_plot = new QLabel(widget_Cep_2);
        label_nstas_plot->setObjectName(QStringLiteral("label_nstas_plot"));

        horizontalLayout_3->addWidget(label_nstas_plot);

        horizontalSpacer_3 = new QSpacerItem(94, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);


        verticalLayout_4->addWidget(widget_Cep_2);

        customPlot_nstas = new QCustomPlot(widget_Cep);
        customPlot_nstas->setObjectName(QStringLiteral("customPlot_nstas"));

        verticalLayout_4->addWidget(customPlot_nstas);

        customPlot_dop = new QCustomPlot(widget_Cep);
        customPlot_dop->setObjectName(QStringLiteral("customPlot_dop"));

        verticalLayout_4->addWidget(customPlot_dop);


        verticalLayout_7->addWidget(widget_Cep);


        horizontalLayout_9->addWidget(widget_3);

        widget_10 = new QWidget(centralWidget);
        widget_10->setObjectName(QStringLiteral("widget_10"));
        verticalLayout_8 = new QVBoxLayout(widget_10);
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setContentsMargins(11, 11, 11, 11);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        widget_Sky = new QWidget(widget_10);
        widget_Sky->setObjectName(QStringLiteral("widget_Sky"));
        verticalLayout_2 = new QVBoxLayout(widget_Sky);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        widget_Sky_2 = new QWidget(widget_Sky);
        widget_Sky_2->setObjectName(QStringLiteral("widget_Sky_2"));
        widget_Sky_2->setMaximumSize(QSize(16777215, 20));
        horizontalLayout_2 = new QHBoxLayout(widget_Sky_2);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_9 = new QSpacerItem(100, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_9);

        label_sky_plot = new QLabel(widget_Sky_2);
        label_sky_plot->setObjectName(QStringLiteral("label_sky_plot"));

        horizontalLayout_2->addWidget(label_sky_plot);

        horizontalSpacer_2 = new QSpacerItem(99, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout_2->addWidget(widget_Sky_2);

        DispSky = new QLabel(widget_Sky);
        DispSky->setObjectName(QStringLiteral("DispSky"));
        sizePolicy.setHeightForWidth(DispSky->sizePolicy().hasHeightForWidth());
        DispSky->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(DispSky);


        verticalLayout_8->addWidget(widget_Sky);

        widget_Other = new QWidget(widget_10);
        widget_Other->setObjectName(QStringLiteral("widget_Other"));
        verticalLayout_9 = new QVBoxLayout(widget_Other);
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setContentsMargins(11, 11, 11, 11);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        widget_Other_2 = new QWidget(widget_Other);
        widget_Other_2->setObjectName(QStringLiteral("widget_Other_2"));
        widget_Other_2->setMaximumSize(QSize(16777215, 20));
        horizontalLayout_6 = new QHBoxLayout(widget_Other_2);
        horizontalLayout_6->setSpacing(0);
        horizontalLayout_6->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_7 = new QSpacerItem(100, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_7);

        label_res_plot = new QLabel(widget_Other_2);
        label_res_plot->setObjectName(QStringLiteral("label_res_plot"));

        horizontalLayout_6->addWidget(label_res_plot);

        horizontalSpacer_6 = new QSpacerItem(99, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_6);


        verticalLayout_9->addWidget(widget_Other_2);

        customPlot_res = new QCustomPlot(widget_Other);
        customPlot_res->setObjectName(QStringLiteral("customPlot_res"));

        verticalLayout_9->addWidget(customPlot_res);


        verticalLayout_8->addWidget(widget_Other);


        horizontalLayout_9->addWidget(widget_10);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 903, 23));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QStringLiteral("menu_2"));
        menu_3 = new QMenu(menuBar);
        menu_3->setObjectName(QStringLiteral("menu_3"));
        menu_4 = new QMenu(menuBar);
        menu_4->setObjectName(QStringLiteral("menu_4"));
        menu_5 = new QMenu(menuBar);
        menu_5->setObjectName(QStringLiteral("menu_5"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menuBar->addAction(menu_3->menuAction());
        menuBar->addAction(menu_4->menuAction());
        menuBar->addAction(menu_5->menuAction());
        menu->addAction(actionopen);
        menu_4->addAction(actionsConfig);
        menu_4->addAction(actionupdate);
        menu_4->addAction(actionFreq);
        mainToolBar->addAction(actionupdate);
        mainToolBar->addAction(actionFreq);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        actionopen->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200", Q_NULLPTR));
        actionupdate->setText(QApplication::translate("MainWindow", "\346\233\264\346\226\260\345\210\235\345\247\213\344\275\215\347\275\256", Q_NULLPTR));
        actionFreq->setText(QApplication::translate("MainWindow", "\345\210\207\346\215\242\351\242\221\347\202\271", Q_NULLPTR));
        actionsConfig->setText(QApplication::translate("MainWindow", "\345\256\236\346\227\266\345\244\204\347\220\206\351\205\215\347\275\256", Q_NULLPTR));
        actionsPost->setText(QApplication::translate("MainWindow", "\344\272\213\345\220\216\345\244\204\347\220\206\351\205\215\347\275\256", Q_NULLPTR));
        label_longitude->setText(QApplication::translate("MainWindow", "\347\273\217\345\272\246", Q_NULLPTR));
        label_latitude->setText(QApplication::translate("MainWindow", "\347\272\254\345\272\246", Q_NULLPTR));
        label_height->setText(QApplication::translate("MainWindow", "\351\253\230\345\272\246", Q_NULLPTR));
        label_speed->setText(QApplication::translate("MainWindow", "\351\200\237\345\272\246", Q_NULLPTR));
        label_acceleration->setText(QApplication::translate("MainWindow", "\345\212\240\351\200\237\345\272\246", Q_NULLPTR));
        label_UTC->setText(QApplication::translate("MainWindow", "UTC\346\227\266\351\227\264", Q_NULLPTR));
        label_GPS->setText(QApplication::translate("MainWindow", "GPS\346\227\266\351\227\264", Q_NULLPTR));
        longitude->setText(QApplication::translate("MainWindow", "0 00' 00.0000\"", Q_NULLPTR));
        latitude->setText(QApplication::translate("MainWindow", "0 00' 00.0000\"", Q_NULLPTR));
        height->setText(QApplication::translate("MainWindow", "0.0", Q_NULLPTR));
        speed->setText(QApplication::translate("MainWindow", "0.0", Q_NULLPTR));
        acceleration->setText(QApplication::translate("MainWindow", "0.00km/h", Q_NULLPTR));
        UTC->setText(QApplication::translate("MainWindow", "00-00-00", Q_NULLPTR));
        GPS->setText(QApplication::translate("MainWindow", "08-00-00 ", Q_NULLPTR));
        label_system->setText(QApplication::translate("MainWindow", "\347\263\273\347\273\237", Q_NULLPTR));
        label_nstas->setText(QApplication::translate("MainWindow", "\345\215\253\346\230\237\346\225\260", Q_NULLPTR));
        label_positon_mode->setText(QApplication::translate("MainWindow", "\345\256\232\344\275\215\346\250\241\345\274\217", Q_NULLPTR));
        label_GDOP->setText(QApplication::translate("MainWindow", "GDOP", Q_NULLPTR));
        label_PDOP->setText(QApplication::translate("MainWindow", "PDOP", Q_NULLPTR));
        label_HDOP->setText(QApplication::translate("MainWindow", "HDOP", Q_NULLPTR));
        label_VDOP->setText(QApplication::translate("MainWindow", "VDOP", Q_NULLPTR));
        system->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        nstas->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        positon_mode->setText(QApplication::translate("MainWindow", "NONE", Q_NULLPTR));
        GDOP->setText(QApplication::translate("MainWindow", "0.0", Q_NULLPTR));
        PDOP->setText(QApplication::translate("MainWindow", "0.0", Q_NULLPTR));
        HDOP->setText(QApplication::translate("MainWindow", "0.0", Q_NULLPTR));
        VDOP->setText(QApplication::translate("MainWindow", "0.0", Q_NULLPTR));
        label_xyz_plot->setText(QApplication::translate("MainWindow", "XYZ", Q_NULLPTR));
        label_cno_plot->setText(QApplication::translate("MainWindow", "CNO", Q_NULLPTR));
        DispSnr->setText(QString());
        label_nstas_plot->setText(QApplication::translate("MainWindow", "NSTAS", Q_NULLPTR));
        label_sky_plot->setText(QApplication::translate("MainWindow", "SKY", Q_NULLPTR));
        DispSky->setText(QString());
        label_res_plot->setText(QApplication::translate("MainWindow", "RES", Q_NULLPTR));
        menu->setTitle(QApplication::translate("MainWindow", "\346\226\207\344\273\266", Q_NULLPTR));
        menu_2->setTitle(QApplication::translate("MainWindow", "\347\274\226\350\276\221", Q_NULLPTR));
        menu_3->setTitle(QApplication::translate("MainWindow", "\350\247\206\345\233\276", Q_NULLPTR));
        menu_4->setTitle(QApplication::translate("MainWindow", "\345\217\202\346\225\260\350\256\276\347\275\256", Q_NULLPTR));
        menu_5->setTitle(QApplication::translate("MainWindow", "\345\270\256\345\212\251", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
