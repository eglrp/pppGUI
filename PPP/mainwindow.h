#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QPushButton>
#include "src/rtklib.h"
#include<QDebug>
#include <QTimer>
#include<QLabel>
#include<QPainter>
#include "qcustomplot.h"
#include "dialogconfig.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
      char streamPaths[8][1024];
      char inputFilePaths[2][1024];
      char OutResultPath[1024];
private slots:
    void dateUpdate(void);
    void timerEvent(QTimerEvent *);
    void  UpdatePlot(void);
    void DrawPlot(QLabel *plot,QLabel *plot_sky, int freq);
    void changeplot();
    void DrawSnr(QPainter *c, int w, int h, int x0, int y0,int index, int freq);
    QColor SnrColor(int snr);
    void DrawSky(QPainter *c, int w, int h, int x0, int y0);
    void DrawText(QPainter *c, int x, int y, const QString &s,const QColor &color, int align);
    void DrawSat(QPainter *c, int w, int h, int x0, int y0, int index, int freq);
    void realtimeDataSlot_xyz();
    void setupRealtimeDataDemo_xyz(QCustomPlot *customPlot);
    void setupRealtimeDataDemo_Nstas(QCustomPlot *customPlot);
    void realtimeDataSlot_Nstas();
    void setupRealtimeDataDemo_Dop(QCustomPlot *customPlot);
    void realtimeDataSlot_Dop();
    void setupRealtimeDataDemo_Res(QCustomPlot *customPlot);
    void realtimeDataSlot_Res();
    QColor  PrnColor(int prn);

private:
    Ui::MainWindow *ui;
    DialogConfig *PppConfig;
    int id;
    int Nsat;   //hua tu xiang guan bian liang
    int Sat[MAXSAT],Snr[MAXSAT][NFREQ],Vsat[MAXSAT];
    double Az[MAXSAT],El[MAXSAT];
    int FreqType1;
    double initial_solution[6];
    double solution_ppp[6];
    //QTimer dataTimer;
    void setLablSsize();

};

#endif // MAINWINDOW_H
