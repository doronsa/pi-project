#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <iostream>
//#include <wiringPi.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int GpioVanta = 26;
    int GpioHit = 17;
    int InputGpio = 27;
    int DebagTimerCount = 0;
    int DebagTimerCounthit = 0;
    int DebagTimer = 0;
    int DebagTimerHIT = 0;
    int GpioLhgit = 25;
    bool SabatStatus = false;
    int OnOFF = 1;
    bool LhegitStatus = false;
    //int SabatVentaTimeOff = 0;
    int statutusAutoplaStop = 0;
    int Debugcounter = 0;
    void SetUpTimeSetingFromConfigFile(void);
    void StartSonar();


private slots:

    void on_pushButton_venta_clicked();
    void GetGPIO(void);
    void on_pushButton_hit_clicked();

    void on_pushButton_shbat_clicked();

    void on_pushButton_stop_all_clicked();
    void showTime();
    void read_dht11_dat();
    void StopTimerVenta();
    void StopTimerHit();
//    void timerSabatHendel();
    void ControlSabatTime();
    void LCDdesplyDownCounter();



    void on_horizontalScrollBarVenta_valueChanged(int value);

    void on_horizontalScrollBarHIT_valueChanged(int value);

    void on_horizontalScrollBarSabat_valueChanged(int value);

//    void on_timeEditOff_userTimeChanged(const QTime &time);

//    void on_timeEditOn_userTimeChanged(const QTime &time);

    void on_timeEditOff_timeChanged(const QTime &time);

    void on_timeEditOn_timeChanged(const QTime &time);

private:

    QTimer *DateTimetimer;
    QTimer *VantaTimer;
    QTimer *HitTimer;
    QTimer *SabatTimer;
    QTimer *GetInputGPIO;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
