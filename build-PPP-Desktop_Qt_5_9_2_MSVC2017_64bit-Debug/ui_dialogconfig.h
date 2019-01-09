/********************************************************************************
** Form generated from reading UI file 'dialogconfig.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGCONFIG_H
#define UI_DIALOGCONFIG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DialogConfig
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *TabSheet1;
    QVBoxLayout *verticalLayout_2;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QComboBox *PosMode;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_2;
    QComboBox *Freq;
    QWidget *widget_4;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_3;
    QComboBox *ElMask;
    QWidget *widget_5;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_4;
    QComboBox *TideCorr;
    QWidget *widget_6;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_5;
    QComboBox *IonoOpt;
    QWidget *widget_7;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_6;
    QComboBox *TropOpt;
    QWidget *widget_8;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_7;
    QComboBox *SatEphem;
    QWidget *widget_9;
    QHBoxLayout *horizontalLayout_10;
    QCheckBox *PosOpt1;
    QCheckBox *PosOpt2;
    QCheckBox *PosOpt3;
    QCheckBox *PosOpt5_2;
    QWidget *widget_10;
    QHBoxLayout *horizontalLayout_11;
    QCheckBox *NavSys1;
    QCheckBox *NavSys2;
    QCheckBox *NavSys3;
    QCheckBox *NavSys4;
    QWidget *TabSheet2;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *GroupBox3_2;
    QWidget *widget_12;
    QLabel *Label10_2;
    QLineEdit *MeasErr5_2;
    QWidget *widget_13;
    QLineEdit *MeasErr4_2;
    QLabel *Label16_2;
    QWidget *widget_14;
    QLineEdit *MeasErr3_2;
    QLabel *Label7_2;
    QLineEdit *MeasErr2_2;
    QWidget *widget_15;
    QLineEdit *MeasErrR2_2;
    QLabel *Label6_2;
    QLineEdit *MeasErrR1_2;
    QGroupBox *GroupBox4_2;
    QLabel *Label33_2;
    QLineEdit *PrNoise5_2;
    QLabel *Label26_2;
    QLineEdit *PrNoise1_2;
    QLabel *Label27_2;
    QLineEdit *PrNoise2_2;
    QLabel *Label28_2;
    QLineEdit *PrNoise3_2;
    QLabel *Label29_2;
    QLineEdit *SatClkStab_2;
    QLineEdit *PrNoise4_2;
    QWidget *tab_2;
    QLabel *LabelF2;
    QLabel *Label6_4;
    QPushButton *BtnStr1;
    QCheckBox *StreamRover;
    QLabel *Label5_2;
    QComboBox *StreamRaw;
    QLineEdit *FilePath3;
    QCheckBox *StreamSSR;
    QLineEdit *FilePath2;
    QComboBox *StreamEtrip;
    QCheckBox *OutEventC;
    QPushButton *BtnFile3;
    QPushButton *BtnFile2;
    QPushButton *BtnStr2;
    QLabel *Label10_4;
    QComboBox *StreamOutputRaw;
    QPushButton *BtnStr3;
    QCheckBox *StreamOutput;
    QLabel *Label6_5;
    QComboBox *FormatRover;
    QComboBox *FormatSSR;
    QLabel *LabelF1;
    QLineEdit *FilePath1;
    QPushButton *BtnFile1;
    QWidget *tab;
    QPushButton *BtnSatPcvView_2;
    QLabel *Label23_2;
    QPushButton *BtnDCBFile_2;
    QPushButton *BtnOLFile_2;
    QPushButton *BtnGeoidDataFile_2;
    QPushButton *BtnDCBView_2;
    QLineEdit *DCBFile_2;
    QLineEdit *SatPcvFile_2;
    QPushButton *BtnAntPcvView_2;
    QPushButton *BtnAntPcvFile_2;
    QLineEdit *GeoidDataFile_2;
    QLabel *Label1_2;
    QLabel *Label34_2;
    QLineEdit *OLFile_2;
    QPushButton *BtnEOPView_2;
    QLabel *Label38_2;
    QPushButton *BtnEOPFile_2;
    QPushButton *BtnOLFileView_2;
    QLineEdit *AntPcvFile_2;
    QLabel *Label48_2;
    QLineEdit *EOPFile_2;
    QPushButton *BtnSatPcvFile_2;
    QLineEdit *ClockFile;
    QPushButton *Clock;
    QLabel *Label38_3;
    QPushButton *Orbit;
    QLabel *Label38_4;
    QLineEdit *OrbitFile;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *OK;
    QPushButton *BtnCancel;
    QSpacerItem *horizontalSpacer;

    void setupUi(QDialog *DialogConfig)
    {
        if (DialogConfig->objectName().isEmpty())
            DialogConfig->setObjectName(QStringLiteral("DialogConfig"));
        DialogConfig->resize(605, 511);
        verticalLayout = new QVBoxLayout(DialogConfig);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tabWidget = new QTabWidget(DialogConfig);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setEnabled(true);
        TabSheet1 = new QWidget();
        TabSheet1->setObjectName(QStringLiteral("TabSheet1"));
        verticalLayout_2 = new QVBoxLayout(TabSheet1);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        widget_2 = new QWidget(TabSheet1);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        horizontalLayout_3 = new QHBoxLayout(widget_2);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label = new QLabel(widget_2);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_3->addWidget(label);

        PosMode = new QComboBox(widget_2);
        PosMode->setObjectName(QStringLiteral("PosMode"));
        PosMode->setEnabled(true);

        horizontalLayout_3->addWidget(PosMode);


        verticalLayout_2->addWidget(widget_2);

        widget_3 = new QWidget(TabSheet1);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        horizontalLayout_4 = new QHBoxLayout(widget_3);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_2 = new QLabel(widget_3);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_4->addWidget(label_2);

        Freq = new QComboBox(widget_3);
        Freq->setObjectName(QStringLiteral("Freq"));
        Freq->setEnabled(true);

        horizontalLayout_4->addWidget(Freq);


        verticalLayout_2->addWidget(widget_3);

        widget_4 = new QWidget(TabSheet1);
        widget_4->setObjectName(QStringLiteral("widget_4"));
        horizontalLayout_5 = new QHBoxLayout(widget_4);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_3 = new QLabel(widget_4);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_5->addWidget(label_3);

        ElMask = new QComboBox(widget_4);
        ElMask->setObjectName(QStringLiteral("ElMask"));
        ElMask->setEnabled(true);

        horizontalLayout_5->addWidget(ElMask);


        verticalLayout_2->addWidget(widget_4);

        widget_5 = new QWidget(TabSheet1);
        widget_5->setObjectName(QStringLiteral("widget_5"));
        horizontalLayout_6 = new QHBoxLayout(widget_5);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_4 = new QLabel(widget_5);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_6->addWidget(label_4);

        TideCorr = new QComboBox(widget_5);
        TideCorr->setObjectName(QStringLiteral("TideCorr"));
        TideCorr->setEnabled(true);

        horizontalLayout_6->addWidget(TideCorr);


        verticalLayout_2->addWidget(widget_5);

        widget_6 = new QWidget(TabSheet1);
        widget_6->setObjectName(QStringLiteral("widget_6"));
        horizontalLayout_7 = new QHBoxLayout(widget_6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label_5 = new QLabel(widget_6);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_7->addWidget(label_5);

        IonoOpt = new QComboBox(widget_6);
        IonoOpt->setObjectName(QStringLiteral("IonoOpt"));
        IonoOpt->setEnabled(true);

        horizontalLayout_7->addWidget(IonoOpt);


        verticalLayout_2->addWidget(widget_6);

        widget_7 = new QWidget(TabSheet1);
        widget_7->setObjectName(QStringLiteral("widget_7"));
        horizontalLayout_8 = new QHBoxLayout(widget_7);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        label_6 = new QLabel(widget_7);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_8->addWidget(label_6);

        TropOpt = new QComboBox(widget_7);
        TropOpt->setObjectName(QStringLiteral("TropOpt"));
        TropOpt->setEnabled(true);

        horizontalLayout_8->addWidget(TropOpt);


        verticalLayout_2->addWidget(widget_7);

        widget_8 = new QWidget(TabSheet1);
        widget_8->setObjectName(QStringLiteral("widget_8"));
        horizontalLayout_9 = new QHBoxLayout(widget_8);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        label_7 = new QLabel(widget_8);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout_9->addWidget(label_7);

        SatEphem = new QComboBox(widget_8);
        SatEphem->setObjectName(QStringLiteral("SatEphem"));
        SatEphem->setEnabled(true);

        horizontalLayout_9->addWidget(SatEphem);


        verticalLayout_2->addWidget(widget_8);

        widget_9 = new QWidget(TabSheet1);
        widget_9->setObjectName(QStringLiteral("widget_9"));
        horizontalLayout_10 = new QHBoxLayout(widget_9);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        PosOpt1 = new QCheckBox(widget_9);
        PosOpt1->setObjectName(QStringLiteral("PosOpt1"));
        PosOpt1->setChecked(true);

        horizontalLayout_10->addWidget(PosOpt1);

        PosOpt2 = new QCheckBox(widget_9);
        PosOpt2->setObjectName(QStringLiteral("PosOpt2"));
        PosOpt2->setChecked(false);

        horizontalLayout_10->addWidget(PosOpt2);

        PosOpt3 = new QCheckBox(widget_9);
        PosOpt3->setObjectName(QStringLiteral("PosOpt3"));
        PosOpt3->setChecked(true);

        horizontalLayout_10->addWidget(PosOpt3);

        PosOpt5_2 = new QCheckBox(widget_9);
        PosOpt5_2->setObjectName(QStringLiteral("PosOpt5_2"));
        PosOpt5_2->setChecked(false);

        horizontalLayout_10->addWidget(PosOpt5_2);


        verticalLayout_2->addWidget(widget_9);

        widget_10 = new QWidget(TabSheet1);
        widget_10->setObjectName(QStringLiteral("widget_10"));
        horizontalLayout_11 = new QHBoxLayout(widget_10);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        NavSys1 = new QCheckBox(widget_10);
        NavSys1->setObjectName(QStringLiteral("NavSys1"));
        NavSys1->setEnabled(true);
        NavSys1->setChecked(true);

        horizontalLayout_11->addWidget(NavSys1);

        NavSys2 = new QCheckBox(widget_10);
        NavSys2->setObjectName(QStringLiteral("NavSys2"));
        NavSys2->setChecked(false);

        horizontalLayout_11->addWidget(NavSys2);

        NavSys3 = new QCheckBox(widget_10);
        NavSys3->setObjectName(QStringLiteral("NavSys3"));
        NavSys3->setChecked(false);

        horizontalLayout_11->addWidget(NavSys3);

        NavSys4 = new QCheckBox(widget_10);
        NavSys4->setObjectName(QStringLiteral("NavSys4"));
        NavSys4->setChecked(false);

        horizontalLayout_11->addWidget(NavSys4);


        verticalLayout_2->addWidget(widget_10);

        tabWidget->addTab(TabSheet1, QString());
        TabSheet2 = new QWidget();
        TabSheet2->setObjectName(QStringLiteral("TabSheet2"));
        verticalLayout_3 = new QVBoxLayout(TabSheet2);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        GroupBox3_2 = new QGroupBox(TabSheet2);
        GroupBox3_2->setObjectName(QStringLiteral("GroupBox3_2"));
        widget_12 = new QWidget(GroupBox3_2);
        widget_12->setObjectName(QStringLiteral("widget_12"));
        widget_12->setGeometry(QRect(10, 153, 531, 38));
        Label10_2 = new QLabel(widget_12);
        Label10_2->setObjectName(QStringLiteral("Label10_2"));
        Label10_2->setEnabled(true);
        Label10_2->setGeometry(QRect(9, 9, 132, 16));
        MeasErr5_2 = new QLineEdit(widget_12);
        MeasErr5_2->setObjectName(QStringLiteral("MeasErr5_2"));
        MeasErr5_2->setEnabled(true);
        MeasErr5_2->setGeometry(QRect(240, 10, 261, 20));
        widget_13 = new QWidget(GroupBox3_2);
        widget_13->setObjectName(QStringLiteral("widget_13"));
        widget_13->setGeometry(QRect(10, 109, 531, 38));
        MeasErr4_2 = new QLineEdit(widget_13);
        MeasErr4_2->setObjectName(QStringLiteral("MeasErr4_2"));
        MeasErr4_2->setEnabled(true);
        MeasErr4_2->setGeometry(QRect(240, 9, 261, 20));
        Label16_2 = new QLabel(widget_13);
        Label16_2->setObjectName(QStringLiteral("Label16_2"));
        Label16_2->setEnabled(true);
        Label16_2->setGeometry(QRect(9, 9, 222, 16));
        widget_14 = new QWidget(GroupBox3_2);
        widget_14->setObjectName(QStringLiteral("widget_14"));
        widget_14->setGeometry(QRect(10, 65, 521, 38));
        MeasErr3_2 = new QLineEdit(widget_14);
        MeasErr3_2->setObjectName(QStringLiteral("MeasErr3_2"));
        MeasErr3_2->setEnabled(true);
        MeasErr3_2->setGeometry(QRect(370, 9, 131, 20));
        Label7_2 = new QLabel(widget_14);
        Label7_2->setObjectName(QStringLiteral("Label7_2"));
        Label7_2->setEnabled(true);
        Label7_2->setGeometry(QRect(9, 9, 198, 16));
        MeasErr2_2 = new QLineEdit(widget_14);
        MeasErr2_2->setObjectName(QStringLiteral("MeasErr2_2"));
        MeasErr2_2->setEnabled(true);
        MeasErr2_2->setGeometry(QRect(240, 9, 121, 20));
        widget_15 = new QWidget(GroupBox3_2);
        widget_15->setObjectName(QStringLiteral("widget_15"));
        widget_15->setGeometry(QRect(10, 21, 512, 38));
        MeasErrR2_2 = new QLineEdit(widget_15);
        MeasErrR2_2->setObjectName(QStringLiteral("MeasErrR2_2"));
        MeasErrR2_2->setEnabled(true);
        MeasErrR2_2->setGeometry(QRect(370, 9, 133, 20));
        Label6_2 = new QLabel(widget_15);
        Label6_2->setObjectName(QStringLiteral("Label6_2"));
        Label6_2->setEnabled(true);
        Label6_2->setGeometry(QRect(9, 9, 216, 16));
        MeasErrR1_2 = new QLineEdit(widget_15);
        MeasErrR1_2->setObjectName(QStringLiteral("MeasErrR1_2"));
        MeasErrR1_2->setEnabled(true);
        MeasErrR1_2->setGeometry(QRect(240, 9, 121, 20));

        verticalLayout_3->addWidget(GroupBox3_2);

        GroupBox4_2 = new QGroupBox(TabSheet2);
        GroupBox4_2->setObjectName(QStringLiteral("GroupBox4_2"));
        Label33_2 = new QLabel(GroupBox4_2);
        Label33_2->setObjectName(QStringLiteral("Label33_2"));
        Label33_2->setEnabled(true);
        Label33_2->setGeometry(QRect(10, 27, 216, 16));
        PrNoise5_2 = new QLineEdit(GroupBox4_2);
        PrNoise5_2->setObjectName(QStringLiteral("PrNoise5_2"));
        PrNoise5_2->setEnabled(true);
        PrNoise5_2->setGeometry(QRect(380, 30, 131, 20));
        Label26_2 = new QLabel(GroupBox4_2);
        Label26_2->setObjectName(QStringLiteral("Label26_2"));
        Label26_2->setEnabled(true);
        Label26_2->setGeometry(QRect(10, 59, 156, 16));
        PrNoise1_2 = new QLineEdit(GroupBox4_2);
        PrNoise1_2->setObjectName(QStringLiteral("PrNoise1_2"));
        PrNoise1_2->setEnabled(true);
        PrNoise1_2->setGeometry(QRect(250, 59, 261, 20));
        Label27_2 = new QLabel(GroupBox4_2);
        Label27_2->setObjectName(QStringLiteral("Label27_2"));
        Label27_2->setEnabled(true);
        Label27_2->setGeometry(QRect(10, 91, 210, 16));
        PrNoise2_2 = new QLineEdit(GroupBox4_2);
        PrNoise2_2->setObjectName(QStringLiteral("PrNoise2_2"));
        PrNoise2_2->setEnabled(true);
        PrNoise2_2->setGeometry(QRect(250, 91, 261, 16));
        Label28_2 = new QLabel(GroupBox4_2);
        Label28_2->setObjectName(QStringLiteral("Label28_2"));
        Label28_2->setEnabled(true);
        Label28_2->setGeometry(QRect(10, 123, 174, 16));
        PrNoise3_2 = new QLineEdit(GroupBox4_2);
        PrNoise3_2->setObjectName(QStringLiteral("PrNoise3_2"));
        PrNoise3_2->setEnabled(true);
        PrNoise3_2->setGeometry(QRect(250, 123, 261, 20));
        Label29_2 = new QLabel(GroupBox4_2);
        Label29_2->setObjectName(QStringLiteral("Label29_2"));
        Label29_2->setEnabled(true);
        Label29_2->setGeometry(QRect(8, 160, 186, 20));
        SatClkStab_2 = new QLineEdit(GroupBox4_2);
        SatClkStab_2->setObjectName(QStringLiteral("SatClkStab_2"));
        SatClkStab_2->setEnabled(true);
        SatClkStab_2->setGeometry(QRect(250, 160, 261, 20));
        PrNoise4_2 = new QLineEdit(GroupBox4_2);
        PrNoise4_2->setObjectName(QStringLiteral("PrNoise4_2"));
        PrNoise4_2->setEnabled(true);
        PrNoise4_2->setGeometry(QRect(250, 30, 121, 20));

        verticalLayout_3->addWidget(GroupBox4_2);

        tabWidget->addTab(TabSheet2, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        LabelF2 = new QLabel(tab_2);
        LabelF2->setObjectName(QStringLiteral("LabelF2"));
        LabelF2->setEnabled(true);
        LabelF2->setGeometry(QRect(10, 270, 161, 21));
        Label6_4 = new QLabel(tab_2);
        Label6_4->setObjectName(QStringLiteral("Label6_4"));
        Label6_4->setEnabled(true);
        Label6_4->setGeometry(QRect(310, 20, 75, 40));
        BtnStr1 = new QPushButton(tab_2);
        BtnStr1->setObjectName(QStringLiteral("BtnStr1"));
        BtnStr1->setEnabled(true);
        BtnStr1->setGeometry(QRect(310, 66, 75, 23));
        StreamRover = new QCheckBox(tab_2);
        StreamRover->setObjectName(QStringLiteral("StreamRover"));
        StreamRover->setGeometry(QRect(10, 70, 144, 15));
        Label5_2 = new QLabel(tab_2);
        Label5_2->setObjectName(QStringLiteral("Label5_2"));
        Label5_2->setEnabled(true);
        Label5_2->setGeometry(QRect(160, 20, 151, 40));
        StreamRaw = new QComboBox(tab_2);
        StreamRaw->setObjectName(QStringLiteral("StreamRaw"));
        StreamRaw->setEnabled(true);
        StreamRaw->setGeometry(QRect(160, 67, 131, 20));
        FilePath3 = new QLineEdit(tab_2);
        FilePath3->setObjectName(QStringLiteral("FilePath3"));
        FilePath3->setEnabled(true);
        FilePath3->setGeometry(QRect(10, 367, 423, 20));
        StreamSSR = new QCheckBox(tab_2);
        StreamSSR->setObjectName(QStringLiteral("StreamSSR"));
        StreamSSR->setGeometry(QRect(10, 128, 144, 15));
        FilePath2 = new QLineEdit(tab_2);
        FilePath2->setObjectName(QStringLiteral("FilePath2"));
        FilePath2->setEnabled(true);
        FilePath2->setGeometry(QRect(10, 297, 423, 20));
        StreamEtrip = new QComboBox(tab_2);
        StreamEtrip->setObjectName(QStringLiteral("StreamEtrip"));
        StreamEtrip->setEnabled(true);
        StreamEtrip->setGeometry(QRect(160, 125, 131, 20));
        OutEventC = new QCheckBox(tab_2);
        OutEventC->setObjectName(QStringLiteral("OutEventC"));
        OutEventC->setGeometry(QRect(10, 337, 141, 20));
        BtnFile3 = new QPushButton(tab_2);
        BtnFile3->setObjectName(QStringLiteral("BtnFile3"));
        BtnFile3->setEnabled(true);
        BtnFile3->setGeometry(QRect(440, 367, 95, 23));
        BtnFile2 = new QPushButton(tab_2);
        BtnFile2->setObjectName(QStringLiteral("BtnFile2"));
        BtnFile2->setEnabled(true);
        BtnFile2->setGeometry(QRect(439, 296, 95, 23));
        BtnStr2 = new QPushButton(tab_2);
        BtnStr2->setObjectName(QStringLiteral("BtnStr2"));
        BtnStr2->setEnabled(true);
        BtnStr2->setGeometry(QRect(310, 124, 75, 23));
        Label10_4 = new QLabel(tab_2);
        Label10_4->setObjectName(QStringLiteral("Label10_4"));
        Label10_4->setEnabled(true);
        Label10_4->setGeometry(QRect(10, 20, 144, 40));
        StreamOutputRaw = new QComboBox(tab_2);
        StreamOutputRaw->setObjectName(QStringLiteral("StreamOutputRaw"));
        StreamOutputRaw->setEnabled(true);
        StreamOutputRaw->setGeometry(QRect(160, 177, 131, 20));
        BtnStr3 = new QPushButton(tab_2);
        BtnStr3->setObjectName(QStringLiteral("BtnStr3"));
        BtnStr3->setEnabled(true);
        BtnStr3->setGeometry(QRect(310, 176, 75, 23));
        StreamOutput = new QCheckBox(tab_2);
        StreamOutput->setObjectName(QStringLiteral("StreamOutput"));
        StreamOutput->setGeometry(QRect(10, 180, 144, 15));
        Label6_5 = new QLabel(tab_2);
        Label6_5->setObjectName(QStringLiteral("Label6_5"));
        Label6_5->setEnabled(true);
        Label6_5->setGeometry(QRect(410, 20, 75, 40));
        FormatRover = new QComboBox(tab_2);
        FormatRover->setObjectName(QStringLiteral("FormatRover"));
        FormatRover->setEnabled(true);
        FormatRover->setGeometry(QRect(410, 70, 131, 20));
        FormatSSR = new QComboBox(tab_2);
        FormatSSR->setObjectName(QStringLiteral("FormatSSR"));
        FormatSSR->setEnabled(true);
        FormatSSR->setGeometry(QRect(410, 130, 131, 20));
        LabelF1 = new QLabel(tab_2);
        LabelF1->setObjectName(QStringLiteral("LabelF1"));
        LabelF1->setEnabled(true);
        LabelF1->setGeometry(QRect(10, 200, 161, 21));
        FilePath1 = new QLineEdit(tab_2);
        FilePath1->setObjectName(QStringLiteral("FilePath1"));
        FilePath1->setEnabled(true);
        FilePath1->setGeometry(QRect(11, 231, 423, 20));
        BtnFile1 = new QPushButton(tab_2);
        BtnFile1->setObjectName(QStringLiteral("BtnFile1"));
        BtnFile1->setEnabled(true);
        BtnFile1->setGeometry(QRect(440, 230, 95, 23));
        tabWidget->addTab(tab_2, QString());
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        BtnSatPcvView_2 = new QPushButton(tab);
        BtnSatPcvView_2->setObjectName(QStringLiteral("BtnSatPcvView_2"));
        BtnSatPcvView_2->setEnabled(true);
        BtnSatPcvView_2->setGeometry(QRect(458, 110, 28, 24));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(BtnSatPcvView_2->sizePolicy().hasHeightForWidth());
        BtnSatPcvView_2->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QStringLiteral(":/buttons/doc.bmp"), QSize(), QIcon::Normal, QIcon::Off);
        BtnSatPcvView_2->setIcon(icon);
        BtnSatPcvView_2->setFlat(true);
        Label23_2 = new QLabel(tab);
        Label23_2->setObjectName(QStringLiteral("Label23_2"));
        Label23_2->setEnabled(true);
        Label23_2->setGeometry(QRect(10, 302, 442, 11));
        BtnDCBFile_2 = new QPushButton(tab);
        BtnDCBFile_2->setObjectName(QStringLiteral("BtnDCBFile_2"));
        BtnDCBFile_2->setEnabled(true);
        BtnDCBFile_2->setGeometry(QRect(492, 267, 75, 23));
        BtnOLFile_2 = new QPushButton(tab);
        BtnOLFile_2->setObjectName(QStringLiteral("BtnOLFile_2"));
        BtnOLFile_2->setEnabled(true);
        BtnOLFile_2->setGeometry(QRect(490, 383, 75, 23));
        BtnGeoidDataFile_2 = new QPushButton(tab);
        BtnGeoidDataFile_2->setObjectName(QStringLiteral("BtnGeoidDataFile_2"));
        BtnGeoidDataFile_2->setEnabled(true);
        BtnGeoidDataFile_2->setGeometry(QRect(492, 215, 75, 23));
        BtnDCBView_2 = new QPushButton(tab);
        BtnDCBView_2->setObjectName(QStringLiteral("BtnDCBView_2"));
        BtnDCBView_2->setEnabled(true);
        BtnDCBView_2->setGeometry(QRect(492, 244, 75, 23));
        BtnDCBView_2->setFlat(true);
        DCBFile_2 = new QLineEdit(tab);
        DCBFile_2->setObjectName(QStringLiteral("DCBFile_2"));
        DCBFile_2->setEnabled(true);
        DCBFile_2->setGeometry(QRect(10, 268, 476, 20));
        SatPcvFile_2 = new QLineEdit(tab);
        SatPcvFile_2->setObjectName(QStringLiteral("SatPcvFile_2"));
        SatPcvFile_2->setEnabled(true);
        SatPcvFile_2->setGeometry(QRect(10, 141, 476, 20));
        BtnAntPcvView_2 = new QPushButton(tab);
        BtnAntPcvView_2->setObjectName(QStringLiteral("BtnAntPcvView_2"));
        BtnAntPcvView_2->setEnabled(true);
        BtnAntPcvView_2->setGeometry(QRect(492, 110, 28, 24));
        sizePolicy.setHeightForWidth(BtnAntPcvView_2->sizePolicy().hasHeightForWidth());
        BtnAntPcvView_2->setSizePolicy(sizePolicy);
        BtnAntPcvView_2->setIcon(icon);
        BtnAntPcvView_2->setFlat(true);
        BtnAntPcvFile_2 = new QPushButton(tab);
        BtnAntPcvFile_2->setObjectName(QStringLiteral("BtnAntPcvFile_2"));
        BtnAntPcvFile_2->setEnabled(true);
        BtnAntPcvFile_2->setGeometry(QRect(492, 169, 75, 23));
        GeoidDataFile_2 = new QLineEdit(tab);
        GeoidDataFile_2->setObjectName(QStringLiteral("GeoidDataFile_2"));
        GeoidDataFile_2->setEnabled(true);
        GeoidDataFile_2->setGeometry(QRect(10, 216, 476, 20));
        Label1_2 = new QLabel(tab);
        Label1_2->setObjectName(QStringLiteral("Label1_2"));
        Label1_2->setEnabled(true);
        Label1_2->setGeometry(QRect(10, 244, 442, 11));
        Label34_2 = new QLabel(tab);
        Label34_2->setObjectName(QStringLiteral("Label34_2"));
        Label34_2->setEnabled(true);
        Label34_2->setGeometry(QRect(10, 361, 442, 11));
        OLFile_2 = new QLineEdit(tab);
        OLFile_2->setObjectName(QStringLiteral("OLFile_2"));
        OLFile_2->setEnabled(true);
        OLFile_2->setGeometry(QRect(10, 385, 476, 20));
        BtnEOPView_2 = new QPushButton(tab);
        BtnEOPView_2->setObjectName(QStringLiteral("BtnEOPView_2"));
        BtnEOPView_2->setEnabled(true);
        BtnEOPView_2->setGeometry(QRect(492, 302, 75, 23));
        BtnEOPView_2->setFlat(true);
        Label38_2 = new QLabel(tab);
        Label38_2->setObjectName(QStringLiteral("Label38_2"));
        Label38_2->setEnabled(true);
        Label38_2->setGeometry(QRect(10, 110, 442, 24));
        BtnEOPFile_2 = new QPushButton(tab);
        BtnEOPFile_2->setObjectName(QStringLiteral("BtnEOPFile_2"));
        BtnEOPFile_2->setEnabled(true);
        BtnEOPFile_2->setGeometry(QRect(492, 326, 75, 23));
        BtnOLFileView_2 = new QPushButton(tab);
        BtnOLFileView_2->setObjectName(QStringLiteral("BtnOLFileView_2"));
        BtnOLFileView_2->setEnabled(true);
        BtnOLFileView_2->setGeometry(QRect(492, 361, 75, 23));
        BtnOLFileView_2->setFlat(true);
        AntPcvFile_2 = new QLineEdit(tab);
        AntPcvFile_2->setObjectName(QStringLiteral("AntPcvFile_2"));
        AntPcvFile_2->setEnabled(true);
        AntPcvFile_2->setGeometry(QRect(10, 170, 476, 20));
        Label48_2 = new QLabel(tab);
        Label48_2->setObjectName(QStringLiteral("Label48_2"));
        Label48_2->setEnabled(true);
        Label48_2->setGeometry(QRect(10, 198, 442, 11));
        EOPFile_2 = new QLineEdit(tab);
        EOPFile_2->setObjectName(QStringLiteral("EOPFile_2"));
        EOPFile_2->setEnabled(true);
        EOPFile_2->setGeometry(QRect(10, 327, 476, 20));
        BtnSatPcvFile_2 = new QPushButton(tab);
        BtnSatPcvFile_2->setObjectName(QStringLiteral("BtnSatPcvFile_2"));
        BtnSatPcvFile_2->setEnabled(true);
        BtnSatPcvFile_2->setGeometry(QRect(492, 140, 75, 23));
        ClockFile = new QLineEdit(tab);
        ClockFile->setObjectName(QStringLiteral("ClockFile"));
        ClockFile->setEnabled(true);
        ClockFile->setGeometry(QRect(10, 84, 476, 20));
        Clock = new QPushButton(tab);
        Clock->setObjectName(QStringLiteral("Clock"));
        Clock->setEnabled(true);
        Clock->setGeometry(QRect(490, 83, 75, 23));
        Label38_3 = new QLabel(tab);
        Label38_3->setObjectName(QStringLiteral("Label38_3"));
        Label38_3->setEnabled(true);
        Label38_3->setGeometry(QRect(10, 58, 442, 24));
        Orbit = new QPushButton(tab);
        Orbit->setObjectName(QStringLiteral("Orbit"));
        Orbit->setEnabled(true);
        Orbit->setGeometry(QRect(490, 30, 75, 23));
        Label38_4 = new QLabel(tab);
        Label38_4->setObjectName(QStringLiteral("Label38_4"));
        Label38_4->setEnabled(true);
        Label38_4->setGeometry(QRect(10, 5, 442, 24));
        OrbitFile = new QLineEdit(tab);
        OrbitFile->setObjectName(QStringLiteral("OrbitFile"));
        OrbitFile->setEnabled(true);
        OrbitFile->setGeometry(QRect(10, 30, 476, 20));
        tabWidget->addTab(tab, QString());

        verticalLayout->addWidget(tabWidget);

        widget = new QWidget(DialogConfig);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer_2 = new QSpacerItem(223, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        OK = new QPushButton(widget);
        OK->setObjectName(QStringLiteral("OK"));
        OK->setMaximumSize(QSize(50, 30));

        horizontalLayout->addWidget(OK);

        BtnCancel = new QPushButton(widget);
        BtnCancel->setObjectName(QStringLiteral("BtnCancel"));
        BtnCancel->setMaximumSize(QSize(50, 30));

        horizontalLayout->addWidget(BtnCancel);

        horizontalSpacer = new QSpacerItem(223, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addWidget(widget);


        retranslateUi(DialogConfig);

        tabWidget->setCurrentIndex(2);
        PosMode->setCurrentIndex(1);
        Freq->setCurrentIndex(1);
        ElMask->setCurrentIndex(2);
        TideCorr->setCurrentIndex(1);
        IonoOpt->setCurrentIndex(2);
        TropOpt->setCurrentIndex(2);
        SatEphem->setCurrentIndex(2);
        StreamRaw->setCurrentIndex(1);
        StreamEtrip->setCurrentIndex(7);


        QMetaObject::connectSlotsByName(DialogConfig);
    } // setupUi

    void retranslateUi(QDialog *DialogConfig)
    {
        DialogConfig->setWindowTitle(QApplication::translate("DialogConfig", "Dialog", Q_NULLPTR));
        label->setText(QApplication::translate("DialogConfig", "\345\256\232\344\275\215\346\250\241\345\274\217", Q_NULLPTR));
        PosMode->clear();
        PosMode->insertItems(0, QStringList()
         << QApplication::translate("DialogConfig", "Single", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "PPP Kinematic", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "PPP Static", Q_NULLPTR)
        );
#ifndef QT_NO_TOOLTIP
        PosMode->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        label_2->setText(QApplication::translate("DialogConfig", "\345\256\232\344\275\215\346\250\241\345\274\217", Q_NULLPTR));
        Freq->clear();
        Freq->insertItems(0, QStringList()
         << QApplication::translate("DialogConfig", "L1", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "L1+L2", Q_NULLPTR)
        );
#ifndef QT_NO_TOOLTIP
        Freq->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        label_3->setText(QApplication::translate("DialogConfig", "\351\253\230\345\272\246\346\210\252\346\255\242\350\247\222(\302\260)", Q_NULLPTR));
        ElMask->clear();
        ElMask->insertItems(0, QStringList()
         << QApplication::translate("DialogConfig", "0", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "5", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "10", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "15", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "20", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "25", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "30", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "35", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "40", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "45", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "50", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "55", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "60", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "65", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "70", Q_NULLPTR)
        );
#ifndef QT_NO_TOOLTIP
        ElMask->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        label_4->setText(QApplication::translate("DialogConfig", "\345\234\260\347\220\203\345\233\272\344\275\223\346\275\256\344\277\256\346\255\243", Q_NULLPTR));
        TideCorr->clear();
        TideCorr->insertItems(0, QStringList()
         << QApplication::translate("DialogConfig", "OFF", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "solid", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "solid+otl", Q_NULLPTR)
        );
#ifndef QT_NO_TOOLTIP
        TideCorr->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        label_5->setText(QApplication::translate("DialogConfig", "\347\224\265\347\246\273\345\261\202\346\250\241\345\236\213", Q_NULLPTR));
        IonoOpt->clear();
        IonoOpt->insertItems(0, QStringList()
         << QApplication::translate("DialogConfig", "OFF", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "Broadcast", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "Iono-Free LC", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "Estimate STEC", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "IONEX TEC", Q_NULLPTR)
        );
#ifndef QT_NO_TOOLTIP
        IonoOpt->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        label_6->setText(QApplication::translate("DialogConfig", "\345\257\271\346\265\201\345\261\202\346\250\241\345\236\213", Q_NULLPTR));
        TropOpt->clear();
        TropOpt->insertItems(0, QStringList()
         << QApplication::translate("DialogConfig", "OFF", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "Saastamoinen", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "Estimate ZTD", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "Estimate ZTD+Grad", Q_NULLPTR)
        );
#ifndef QT_NO_TOOLTIP
        TropOpt->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        label_7->setText(QApplication::translate("DialogConfig", "\346\230\237\345\216\206\345\217\212\351\222\237\345\267\256", Q_NULLPTR));
        SatEphem->clear();
        SatEphem->insertItems(0, QStringList()
         << QApplication::translate("DialogConfig", "Broadcast", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "Precise", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "Broadcast+SSR APC", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "Broadcast+SSR CoM", Q_NULLPTR)
        );
#ifndef QT_NO_TOOLTIP
        SatEphem->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        PosOpt1->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        PosOpt1->setText(QApplication::translate("DialogConfig", "Sat PCV", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        PosOpt2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        PosOpt2->setText(QApplication::translate("DialogConfig", "Rec PCV", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        PosOpt3->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        PosOpt3->setText(QApplication::translate("DialogConfig", "Ph-Windup", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        PosOpt5_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        PosOpt5_2->setText(QApplication::translate("DialogConfig", "RAIM FDE", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        NavSys1->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        NavSys1->setText(QApplication::translate("DialogConfig", "GPS", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        NavSys2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        NavSys2->setText(QApplication::translate("DialogConfig", "GLO", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        NavSys3->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        NavSys3->setText(QApplication::translate("DialogConfig", "Galileo", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        NavSys4->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        NavSys4->setText(QApplication::translate("DialogConfig", "BeiDou", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(TabSheet1), QApplication::translate("DialogConfig", "Setting1", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        GroupBox3_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        GroupBox3_2->setTitle(QApplication::translate("DialogConfig", "Measurement Errors (1-sigma)", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        Label10_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        Label10_2->setText(QApplication::translate("DialogConfig", "Doppler Frequency (Hz)", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        MeasErr5_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        MeasErr5_2->setText(QApplication::translate("DialogConfig", "1.000", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        MeasErr4_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        MeasErr4_2->setText(QApplication::translate("DialogConfig", "0.000", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        Label16_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        Label16_2->setText(QApplication::translate("DialogConfig", "Carrier-Phase Error/Baseline (m/10km)", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        MeasErr3_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        MeasErr3_2->setText(QApplication::translate("DialogConfig", "0.003", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        Label7_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        Label7_2->setText(QApplication::translate("DialogConfig", "Carrier-Phase Error a+b/sinEl (m)", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        MeasErr2_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        MeasErr2_2->setText(QApplication::translate("DialogConfig", "0.003", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        MeasErrR2_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        MeasErrR2_2->setText(QApplication::translate("DialogConfig", "100.0", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        Label6_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        Label6_2->setText(QApplication::translate("DialogConfig", "Code/Carrier-Phase Error Ratio L1/L2", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        MeasErrR1_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        MeasErrR1_2->setText(QApplication::translate("DialogConfig", "100.0", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        GroupBox4_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        GroupBox4_2->setTitle(QApplication::translate("DialogConfig", "Process Noises (1-sigma/sqrt(s))", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        Label33_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        Label33_2->setText(QApplication::translate("DialogConfig", "Receiver Accel Horiz/Vertical (m/s2)", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        PrNoise5_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        PrNoise5_2->setText(QApplication::translate("DialogConfig", "1.0E-04", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        Label26_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        Label26_2->setText(QApplication::translate("DialogConfig", "Carrier-Phase Bias (cycle)", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        PrNoise1_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        PrNoise1_2->setText(QApplication::translate("DialogConfig", "1.0E-04", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        Label27_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        Label27_2->setText(QApplication::translate("DialogConfig", "Vertical Ionospheric Delay (m/10km)", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        PrNoise2_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        PrNoise2_2->setText(QApplication::translate("DialogConfig", "1.0E-03", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        Label28_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        Label28_2->setText(QApplication::translate("DialogConfig", "Zenith Tropospheric Delay (m)", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        PrNoise3_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        PrNoise3_2->setText(QApplication::translate("DialogConfig", "1.0E-04", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        Label29_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        Label29_2->setText(QApplication::translate("DialogConfig", "Satellite Clock Stability (s/s)", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        SatClkStab_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        SatClkStab_2->setText(QApplication::translate("DialogConfig", "5.0E-12", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        PrNoise4_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        PrNoise4_2->setText(QApplication::translate("DialogConfig", "1.0E-04", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(TabSheet2), QApplication::translate("DialogConfig", "Setting2", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        LabelF2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        LabelF2->setText(QApplication::translate("DialogConfig", "Save Raw Data File Paths", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        Label6_4->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        Label6_4->setText(QApplication::translate("DialogConfig", "Option", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        BtnStr1->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        BtnStr1->setText(QApplication::translate("DialogConfig", "...", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        StreamRover->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        StreamRover->setText(QApplication::translate("DialogConfig", "(6) Rover", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        Label5_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        Label5_2->setText(QApplication::translate("DialogConfig", "Type", Q_NULLPTR));
        StreamRaw->clear();
        StreamRaw->insertItems(0, QStringList()
         << QApplication::translate("DialogConfig", "STR_NONE", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "STR_SERIAL", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "STR_FILE", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "STR_TCPSVR", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "STR_TCPCLI", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "STR_UDP", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "STR_NTRIPSVR", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "STR_NTRIPCLI", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "STR_FTP", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "STR_HTTP", Q_NULLPTR)
        );
#ifndef QT_NO_TOOLTIP
        StreamRaw->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        FilePath3->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        FilePath3->setText(QString());
#ifndef QT_NO_TOOLTIP
        StreamSSR->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        StreamSSR->setText(QApplication::translate("DialogConfig", "(7) Correction", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        FilePath2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        FilePath2->setText(QString());
        StreamEtrip->clear();
        StreamEtrip->insertItems(0, QStringList()
         << QApplication::translate("DialogConfig", "STR_NONE", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "STR_SERIAL", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "STR_FILE", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "STR_TCPSVR", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "STR_TCPCLI", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "STR_UDP", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "STR_NTRIPSVR", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "STR_NTRIPCLI", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "STR_FTP", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "STR_HTTP", Q_NULLPTR)
        );
#ifndef QT_NO_TOOLTIP
        StreamEtrip->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        OutEventC->setText(QApplication::translate("DialogConfig", "Save PostResult Paths", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        BtnFile3->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        BtnFile3->setText(QApplication::translate("DialogConfig", "...", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        BtnFile2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        BtnFile2->setText(QApplication::translate("DialogConfig", "...", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        BtnStr2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        BtnStr2->setText(QApplication::translate("DialogConfig", "...", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        Label10_4->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        Label10_4->setText(QApplication::translate("DialogConfig", "Log Stream", Q_NULLPTR));
        StreamOutputRaw->clear();
        StreamOutputRaw->insertItems(0, QStringList()
         << QApplication::translate("DialogConfig", "STR_NONE", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "STR_SERIAL", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "STR_FILE", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "STR_TCPSVR", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "STR_TCPCLI", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "STR_UDP", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "STR_NTRIPSVR", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "STR_NTRIPCLI", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "STR_FTP", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "STR_HTTP", Q_NULLPTR)
        );
#ifndef QT_NO_TOOLTIP
        StreamOutputRaw->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        BtnStr3->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        BtnStr3->setText(QApplication::translate("DialogConfig", "...", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        StreamOutput->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        StreamOutput->setText(QApplication::translate("DialogConfig", "(8) Output Raw", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        Label6_5->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        Label6_5->setText(QApplication::translate("DialogConfig", "Format", Q_NULLPTR));
        FormatRover->clear();
        FormatRover->insertItems(0, QStringList()
         << QApplication::translate("DialogConfig", "RTCM3", Q_NULLPTR)
        );
#ifndef QT_NO_TOOLTIP
        FormatRover->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        FormatSSR->clear();
        FormatSSR->insertItems(0, QStringList()
         << QApplication::translate("DialogConfig", "RTCM3", Q_NULLPTR)
         << QApplication::translate("DialogConfig", "RTCM2", Q_NULLPTR)
        );
#ifndef QT_NO_TOOLTIP
        FormatSSR->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        LabelF1->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        LabelF1->setText(QApplication::translate("DialogConfig", "Input Raw Data File Paths", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        FilePath1->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        FilePath1->setText(QString());
#ifndef QT_NO_TOOLTIP
        BtnFile1->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        BtnFile1->setText(QApplication::translate("DialogConfig", "...", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("DialogConfig", "input", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        BtnSatPcvView_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        BtnSatPcvView_2->setText(QString());
#ifndef QT_NO_TOOLTIP
        Label23_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        Label23_2->setText(QApplication::translate("DialogConfig", "EOP Data File", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        BtnDCBFile_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        BtnDCBFile_2->setText(QApplication::translate("DialogConfig", "...", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        BtnOLFile_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        BtnOLFile_2->setText(QApplication::translate("DialogConfig", "...", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        BtnGeoidDataFile_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        BtnGeoidDataFile_2->setText(QApplication::translate("DialogConfig", "...", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        BtnDCBView_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        BtnDCBView_2->setText(QString());
#ifndef QT_NO_TOOLTIP
        DCBFile_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        DCBFile_2->setText(QString());
#ifndef QT_NO_TOOLTIP
        SatPcvFile_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        SatPcvFile_2->setText(QString());
#ifndef QT_NO_TOOLTIP
        BtnAntPcvView_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        BtnAntPcvView_2->setText(QString());
#ifndef QT_NO_TOOLTIP
        BtnAntPcvFile_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        BtnAntPcvFile_2->setText(QApplication::translate("DialogConfig", "...", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        GeoidDataFile_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        GeoidDataFile_2->setText(QString());
#ifndef QT_NO_TOOLTIP
        Label1_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        Label1_2->setText(QApplication::translate("DialogConfig", "DCB Data File", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        Label34_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        Label34_2->setText(QApplication::translate("DialogConfig", "Ocean Loading BLQ Format", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        OLFile_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        OLFile_2->setText(QString());
#ifndef QT_NO_TOOLTIP
        BtnEOPView_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        BtnEOPView_2->setText(QString());
#ifndef QT_NO_TOOLTIP
        Label38_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        Label38_2->setText(QApplication::translate("DialogConfig", "Satellite/Receiver Antenna PCV File ANTEX/NGS PCV", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        BtnEOPFile_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        BtnEOPFile_2->setText(QApplication::translate("DialogConfig", "...", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        BtnOLFileView_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        BtnOLFileView_2->setText(QString());
#ifndef QT_NO_TOOLTIP
        AntPcvFile_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        AntPcvFile_2->setText(QString());
#ifndef QT_NO_TOOLTIP
        Label48_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        Label48_2->setText(QApplication::translate("DialogConfig", "Geoid Data File", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        EOPFile_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        EOPFile_2->setText(QString());
#ifndef QT_NO_TOOLTIP
        BtnSatPcvFile_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        BtnSatPcvFile_2->setText(QApplication::translate("DialogConfig", "...", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        ClockFile->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        ClockFile->setText(QString());
#ifndef QT_NO_TOOLTIP
        Clock->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        Clock->setText(QApplication::translate("DialogConfig", "...", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        Label38_3->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        Label38_3->setText(QApplication::translate("DialogConfig", "PreciseClockFile", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        Orbit->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        Orbit->setText(QApplication::translate("DialogConfig", "...", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        Label38_4->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        Label38_4->setText(QApplication::translate("DialogConfig", "PreciseOrbitFile", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        OrbitFile->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        OrbitFile->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("DialogConfig", "Product", Q_NULLPTR));
        OK->setText(QApplication::translate("DialogConfig", "OK", Q_NULLPTR));
        BtnCancel->setText(QApplication::translate("DialogConfig", "Cancel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class DialogConfig: public Ui_DialogConfig {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGCONFIG_H
