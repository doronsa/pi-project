/*
 * VERSION 3.1 * 03/07/24
 * TO DO List
 * set the tab order to 1
 * add the new seting on starting
 * * add all to load seting
 * add the sonar system
 *


*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qdebug.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <QMessageBox>
#include <QTime>
#include <QTimer>
#include <time.h>
#include <iostream>
#include <wiringPi.h>
#include <qfile.h>
#include <QStringList>
#include <QSettings>
#include <iostream>
#include <iostream>
#include <QFile>
#include <QTextStream>

QString VERSION = "VERSION 3.5";
#define MAXTIMINGS	85
#define DHTPIN		4
int GpioHit = 17;
int InputGpio = 12;//27;
int DebagTimerCount = 0;
int DebagTimerCounthit = 0;
int DebagTimer = 0;
int DebagTimerHIT = 0;
int GpioLhgit = 25;
//new gpio
int EchoTemp = 4;
int TrigUsonic = 23;
int EchoSonic =24;
int oldValue = 1;
int NewValue = 0;
int SabatVentaTimeOff = 0;
bool SabatTimeOnOff = 0;
bool ok;
int dht11_dat[5] = { 0, 0, 0, 0, 0 };
int OnOFF = 1;

using namespace std;
void DebugMssg(QString mssg);
QSettings settings2("/home/pi/Desktop/venta.ini", QSettings::IniFormat);
void timerVentaHendel(void);
void timerHitHendel(void);
QString GetConfigMode(void);

//QSettings settings("/home/pi/Desktop/NewSonar.ini", QSettings::IniFormat);
void SaveToLogfile(QString mssge);
bool on = 1;
bool off = 0;
//const  VANTAHW  1
//const  LHGITHW  2
//const  HITH     3
void MainWindow::HDControl(int hd,bool val)
{
    QString msval = "from HDControl :";
    switch (hd) {
    case 1:
        digitalWrite( GpioVanta, val );
        msval = msval + "Vanta";
        msval = msval + "val :" + QString::number(val)+"\n";
        DebugMssg(msval);
        break;
    case 2:
        digitalWrite( GpioLhgit, val );
        msval = msval + "Lhgit";
        msval = msval + "val :" + QString::number(val)+"\n";
        DebugMssg(msval);
        break;
    case 3:
        digitalWrite( GpioHit, val );
        msval = msval + "HIT";
        msval = msval + "val :" + QString::number(val)+"\n";
        DebugMssg(msval);
        break;
    default:
        break;
    }

}

void DebugMssg(QString mssg)
{
    mssg = mssg + "SabatVentaTimeOff :" + QString::number(SabatVentaTimeOff)+"\n";
    mssg = mssg + "SabatTimeOnOff :" + QString::number(SabatTimeOnOff)+"\n";
    mssg = mssg + "OnOFF :"+ QString::number(OnOFF)+"\n";

    SaveToLogfile(mssg);
}

void MainWindow::ReadFile()
{
//    settings.beginGroup("Sonar");
//    QString SonarDsttns = settings.value("Value").toByteArray().constData();
//    settings.endGroup();
    QFile file("/home/pi/Desktop/NewSonar.ini");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString SonarDsttns = file.readAll();
        int ansser = QString::compare(SonarDsttns, "Value", Qt::CaseInsensitive);

//        QString myString("This is a string");
        QString subString = SonarDsttns.mid(14,6); // subString contains "is"
//        subString.append("n't"); // subString
        SonarValue = subString;
        ui->temp_lcdNumber_sonar->display(subString);
//        qDebug()<<"SonarDsttns  " << subString<<"ansser"<<ansser<< "\n";
    }


}




void SaveToLogfile(QString mssge)
{
//    QTime Timet_h = QTime::currentTime();
//    QDate DateT = QDate::currentDate();
//    QString Date_Time = DateT.toString();

//    QFile file("/home/pi/Desktop/log.txt");
//    if(file.open(QIODevice::Append | QIODevice::Text))
//    {
//         QTextStream out(&file);
//         mssge = mssge +" " + Timet_h.toString("hh:mm:ss") + " " + Date_Time;
//         out << mssge << "\n";
//         qDebug()<<mssge<< "\n";
//    }
//    file.close();

}





void MainWindow::LoadSeting()
{
    settings2.beginGroup("VantaTime");
    QString ventatime = settings2.value("Time").toByteArray().constData();
    settings2.endGroup();
    ui->horizontalScrollBarVenta->setValue(ventatime.toInt());

    settings2.beginGroup("HitTime");
    QString hittime = settings2.value("Time").toByteArray().constData();
    settings2.endGroup();
    ui->horizontalScrollBarHIT->setValue(hittime.toInt());

    settings2.beginGroup("VentaSabatTime");
    QString sabattime = settings2.value("Time").toByteArray().constData();
    settings2.endGroup();
    ui->horizontalScrollBarSabat->setValue(sabattime.toInt());


    settings2.beginGroup("SabatTime");
    QString SabatOnTime = settings2.value("SabatTimeOn").toByteArray().constData();
    QString SabatOffTime = settings2.value("SabatTimeOff").toByteArray().constData();
    settings2.endGroup();
    ui->horizontalScrollBarSabatOff->setValue(SabatOffTime.toInt());
    ui->horizontalScrollBarSabaton->setValue(SabatOnTime.toInt());

    settings2.beginGroup("Humidity");
    QString humyval = settings2.value("Humy").toByteArray().constData();
    QString timeval = settings2.value("Time").toByteArray().constData();
    settings2.endGroup();
    ui->horizontalScrollBarHumidity->setValue(humyval.toInt());
    ui->horizontalScrollBarTimeAdd->setValue(timeval.toInt());

    settings2.beginGroup("Sonar");
    QString SonarValue = settings2.value("value").toByteArray().constData();
    ui->horizontalScrollBarSonar->setValue(SonarValue.toInt());
    settings2.endGroup();

}
void MainWindow::SetNewSetingFile()
{
    settings2.beginGroup("Mode");
    settings2.setValue("Value","venta");
    settings2.endGroup();
     /**/
    settings2.beginGroup("Humidity");
    settings2.setValue("Humy",65);
    settings2.setValue("Time",10);
    settings2.endGroup();
    /**/
    settings2.beginGroup("SabatTime");
    settings2.setValue("SabatTimeOn",8);
    settings2.setValue("SabatTimeOff",12);
    settings2.endGroup();
    /**/
    settings2.beginGroup("VantaTime");
    settings2.setValue("Time",30);
    settings2.endGroup();
    /**/
    settings2.beginGroup("HitTime");
    settings2.setValue("Time",5);
    settings2.endGroup();
    /**/
    settings2.beginGroup("VentaSabatTime");
    settings2.setValue("Time",20);
    settings2.endGroup();
    /**/
    settings2.beginGroup("Sonar");
    settings2.setValue("Value",90);
    settings2.endGroup();

}
int MainWindow::ChackSetingFile()
{
    settings2.beginGroup("Mode");
    QString sysmode = settings2.value("Value").toByteArray().constData();
    settings2.endGroup();
    qDebug() << "Mode : " << sysmode;

    settings2.beginGroup("VantaTime");
    QString ventatime = settings2.value("Time").toByteArray().constData();
    settings2.endGroup();
    qDebug() << "VentaTime : " << ventatime;

    settings2.beginGroup("HitTime");
    QString hittime = settings2.value("Time").toByteArray().constData();
    settings2.endGroup();
    qDebug() << "VHITTime : " << hittime;

    settings2.beginGroup("VentaSabatTime");
    QString sabattime = settings2.value("Time").toByteArray().constData();
    settings2.endGroup();
    qDebug() << "sabattime : " << sabattime;

    settings2.beginGroup("SabatTime");
    QString SabatOnTime = settings2.value("SabatTimeOn").toByteArray().constData();
    QString SabatOffTime = settings2.value("SabatTimeOn").toByteArray().constData();
    settings2.endGroup();
    qDebug() << "SabatTimeOn : " << SabatOnTime;
    qDebug() << "SabatTimeOn : " << SabatOffTime;

    settings2.beginGroup("Humidity");
    QString humyval = settings2.value("Humy").toByteArray().constData();
    QString timeval = settings2.value("Time").toByteArray().constData();
    settings2.endGroup();

    if(ventatime.toInt()!=0)
    {
       if(hittime.toInt()!=0)
       {
           if(sabattime.toInt()!=0)
           {
               if (humyval.toInt()!=0)
               {
                  return (1);
               }else {
                        return (0);
                     }
               return (1);
           }else{
                   return (0);
                }
       }else{
              return (0);
            }
    }else{
            return (0);
         }
}

void MainWindow::read_dht11_dat()
{
    uint8_t laststate	= HIGH;
    uint8_t counter		= 0;
    uint8_t j		= 0, i;
    float	f;

    dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;

    pinMode( DHTPIN, OUTPUT );
    digitalWrite( DHTPIN, LOW );
    delay( 18 );
    digitalWrite( DHTPIN, HIGH );
    delayMicroseconds( 40 );
    pinMode( DHTPIN, INPUT );

    for ( i = 0; i < MAXTIMINGS; i++ )
    {
        counter = 0;
        while ( digitalRead( DHTPIN ) == laststate )
        {
            counter++;
            delayMicroseconds( 1 );
            if ( counter == 255 )
            {
                break;
            }
        }
        laststate = digitalRead( DHTPIN );

        if ( counter == 255 )
            break;

        if ( (i >= 4) && (i % 2 == 0) )
        {
            dht11_dat[j / 8] <<= 1;
            if ( counter > 50 )
                dht11_dat[j / 8] |= 1;
            j++;
        }
    }

    if ( (j >= 40) &&
         (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF) ) )
    {
        f = dht11_dat[2] * 9. / 5. + 32;
        printf( "Humidity = %d.%d %% Temperature = %d.%d C (%.1f F)\n",
            dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3], f );
        QString desply_data = QString("%1.%2").arg(dht11_dat[2]).arg(dht11_dat[3]);;
        QString desply_data2 = QString("%1.%2").arg(dht11_dat[0]).arg(dht11_dat[1]);;
        ui->temp_lcdNumber_2->display(desply_data);
        ui->temp_lcdNumber_4->display(desply_data2);
    }else  {
        printf( "Data not good, skip\n" );
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->menuVer_3->setTitle(VERSION);
    wiringPiSetupGpio();
    ui->tabWidget->tabWhatsThis(1);
    pinMode(GpioVanta, OUTPUT);
    pinMode(GpioHit, OUTPUT);
    pinMode(InputGpio, INPUT);
    pinMode(GpioLhgit, OUTPUT);
//    HDControl(GpioVanta,0);
//    HDControl(GpioHit,0);
    digitalWrite(GpioVanta,false);
    digitalWrite(GpioHit,false);
    digitalWrite(GpioLhgit,false);
    DebagTimerCount = 30*1000*60;
    DebagTimer = 0;
    ui->temp_label_OFF->hide();
    ui->temp_label_ON->hide();
    ui->temp_label_OFF_HIT->hide();
    ui->temp_label_ON_HIT->hide();
    /* open log file*/
     QTextStream stream;
//    if (file.open(QIODevice::ReadWrite))
//    {
//        stream(&file);
//    }
        //stream << "something" << Qt::endl;
     /* time timer*/
    DateTimetimer = new QTimer(this);
    QObject::connect(DateTimetimer, SIGNAL(timeout()), this, SLOT(showTime()));
    DateTimetimer->start(1000);
    /* input gpio timer
    * (need to see if i can do it whit intrupt)*/
    GetInputGPIO = new QTimer(this);
    QObject::connect(GetInputGPIO, SIGNAL(timeout()), this, SLOT(GetGPIO()));
    GetInputGPIO->start(1000);
    /* vanta timer*/
    VantaTimer = new QTimer(this);
    QObject::connect(VantaTimer, SIGNAL(timeout()), this, SLOT(StopTimerVenta()));
    /* hit timer*/
    HitTimer = new QTimer(this);
    QObject::connect(HitTimer, SIGNAL(timeout()), this, SLOT(StopTimerHit()));
    /* sabat timer*/
    SabatTimer = new QTimer(this);
    QObject::connect(SabatTimer, SIGNAL(timeout()), this, SLOT(ControlSabatTime()));

    if (1 == ChackSetingFile())
    {
        LoadSeting();
    }else {
             SetNewSetingFile();
             LoadSeting();
          }

        QString mode = GetConfigMode();
        if (mode.compare("Sabat")==0)
        {
            ui->pushButton_shbat->click();
        }else{
                 if (mode.compare("Vanta")==0)
                 {
                    ui->pushButton_venta->click();
                 }else{
                        if(mode.compare("HIT")==0)
                        {
                            ui->pushButton_hit->click();
                        }
                      }

             }

}

MainWindow::~MainWindow()
{
    delete ui;
}

QString GetConfigMode(void)
{
    settings2.beginGroup("Mode");
    qDebug() << "Mode : " << settings2.value("Mode").toByteArray().constData();
    QString sysmode = settings2.value("Mode").toByteArray().constData();
    settings2.endGroup();
    return (sysmode);
}






void MainWindow::StopTimerVenta()
{
    DebugMssg("423 From StopTimerVenta:\n");
    if(SabatVentaTimeOff == 0)

    {
        if(dht11_dat[0] > ui->spinBoxHumidity->value())
        {
            int timeadd =  ui->spinBoxTimeAdd->value();

            VantaTimer->start(timeadd *1000*60);//;
            DebagTimer = 1;
            DebagTimerCount = timeadd*60;
            ui->temp_lcdNumber_5->display(DebagTimerCount/60);
            DebugMssg("From StopTimerVenta:\n");
        }else {
                    int SonatTrhsold = ui->horizontalScrollBarSonar->value();
                    int SonarVal = ui->temp_lcdNumber_sonar->value();
                    DebugMssg("438 From StopTimerVenta:\n");
                    qDebug()<<"SonatTrhsold = " << SonatTrhsold <<"SonarVal "<<SonarVal;
                    if(SonarVal < SonatTrhsold)
                    {
                        int timeadd1 =  ui->spinBoxTimeAdd->value();
                        VantaTimer->start(timeadd1 *1000*60);//;
                        DebagTimer = 1;
                        DebagTimerCount = timeadd1*60;
                        ui->temp_lcdNumber_5->display(DebagTimerCount/60);
                        DebugMssg("447 From StopTimerVenta:\n");
                    }else
                        {
                            ui->checkBoxVenta->setChecked(false);
    //                        HDControl(GpioVanta,0);
    //                        HDControl(GpioLhgit,0);
                            digitalWrite(GpioVanta,false);
                            digitalWrite(GpioLhgit,false);
                            VantaTimer->stop();
                            DebagTimerCount = 0;
                            DebagTimer = 0;
                            ui->temp_label_OFF->show();
                            ui->temp_label_ON->hide();
                            LhegitStatus = false;
                            SabatVentaTimeOff = 0;
                            DebugMssg("460From StopTimerVenta:\n");
                    }
               }
    }
}

void MainWindow::StopTimerHit()
{
    ui->checkBoxHit->setChecked(false);
    LhegitStatus = false;
//    HDControl(GpioHit,0);
    digitalWrite(GpioHit,false);
    HitTimer->stop();
    DebagTimerCounthit = 0;
    DebagTimerHIT = 0;
    ui->temp_label_OFF_HIT->show();
    ui->temp_label_ON_HIT->hide();
    ui->temp_lcdNumber_HIT->display(DebagTimerCounthit);
    LhegitStatus = false;

    DebagTimerCount = 0;
    DebagTimerCounthit = 0;
    DebagTimer = 0;
    DebagTimerHIT = 0;
    HitTimer->stop();
    ui->checkBoxHit->setChecked(false);
    ui->temp_label_OFF->hide();
    ui->temp_label_ON->hide();
}

void MainWindow::GetGPIO()
{
    NewValue = digitalRead(12);
    if(oldValue != NewValue)
    {
        oldValue = NewValue;
        delay(500);
        if (NewValue == 0 && LhegitStatus == false )
        {
//            HDControl(GpioLhgit,1);
            digitalWrite(GpioLhgit,1);
            LhegitStatus = true;
            if(SabatStatus == 0 )//&& oldValue == 0)
            {

                   //ui->pushButton_stop_all->click();
                    ui->pushButton_venta->click();
                    //digitalWrite(GpioLhgit,1);
                    //LhegitStatus = true;
            }
        }else {

                //if(NewValue == 1)//
                if (digitalRead(InputGpio) == 1)
                {
//                    HDControl(GpioLhgit,0);
                    digitalWrite(GpioLhgit,0);
                    LhegitStatus = false;

                }
              }
    }
}

void MainWindow::on_pushButton_venta_clicked()
{
    if (SabatStatus == 0)
    {
        bool status = ui->checkBoxVenta->isChecked();
        if (status == 0)
        {
            ui->checkBoxVenta->setChecked(true);
            VantaTimer->start(ui->spinBoxTimeVanta->value()*1000*60);
            DebagTimer = 1;
            DebagTimerCount = ui->spinBoxTimeVanta->value()*60;
            ui->temp_lcdNumber_5->display(DebagTimerCount/60);
//            HDControl(GpioLhgit,1);
//            HDControl(GpioVanta,1);
            digitalWrite(GpioLhgit,true);
            digitalWrite(GpioVanta,true);
            ui->temp_label_OFF->hide();
            ui->temp_label_ON->show();
            settings2.beginGroup("Mode");
            settings2.setValue("Mode","Vanta");
            settings2.endGroup();

        }else{

//                HDControl(GpioVanta,0);
//                HDControl(GpioLhgit,0);
                digitalWrite(GpioVanta, false);
                digitalWrite(GpioLhgit,false);
                ui->checkBoxVenta->setChecked(false);
                ui->pushButton_stop_all->click();
                VantaTimer->stop();
                DebagTimerCount = 0;
                DebagTimer = 0;
                ui->temp_label_OFF->show();
                ui->temp_label_ON->hide();               
                LhegitStatus = false;

             }

    }else {
             ui->checkBoxVenta->setChecked(false);
             VantaTimer->stop();
             DebagTimerCount = 0;
           }
}


void MainWindow::on_pushButton_hit_clicked()
{
    bool status = ui->checkBoxHit->isChecked();
    if (status == 0)
    {
        ui->checkBoxHit->setChecked(true);
        HitTimer->start(ui->spinBoxTimeHit->value()*1000*60);
//        HDControl(GpioHit,1);
        digitalWrite(GpioHit, HIGH);
        DebagTimerCounthit = ui->spinBoxTimeHit->value()*60;
        ui->temp_lcdNumber_HIT->display(DebagTimerCounthit/60);
        ui->temp_label_OFF_HIT->hide();
        ui->temp_label_ON_HIT->show();
        DebagTimerHIT = 1;
        settings2.beginGroup("Mode");
        settings2.setValue("Mode","Hit");
        settings2.endGroup();
    }else {
            ui->checkBoxHit->setChecked(false);
//            HDControl(GpioHit,0);
            digitalWrite(GpioHit, LOW);
            HitTimer->stop();
            DebagTimerCounthit = 0;
            DebagTimerHIT = 0;
            ui->temp_label_OFF_HIT->show();
            ui->temp_label_ON_HIT->hide();
            ui->temp_lcdNumber_HIT->display(DebagTimerCounthit);
          }

}


void MainWindow::ControlSabatTime()
{
    //for deubg
//    SabatTimeOnOff =1;
    if(SabatTimeOnOff == 0)//close venta and lghit
    {
        if(SabatVentaTimeOff == 1)
        {
                DebagTimer = 1;
                if (OnOFF == 1)
                {
//                    HDControl(GpioVanta,1);
                    digitalWrite(GpioVanta,1);
                     digitalWrite(GpioLhgit,1);
                    ui->temp_label_OFF->hide();
                    ui->temp_label_ON->show();
                    OnOFF = 0;
                    qDebug()<<"from ControlSabatTime on"<< "\n";
                }else{
                        //DebagTimerCount = 0;
//                        HDControl(GpioVanta,0);
                        digitalWrite(GpioVanta,0);//now it 0
                        ui->temp_label_OFF->show();
                        ui->temp_label_ON->hide();
                        OnOFF = 1;
                        qDebug()<<"from ControlSabatTime off"<< "\n";
                       }

        //qDebug()<<"normal mode ControlSabatTime SabatVentaTimeOff = 0";
        }
        SabatTimer->start(ui->spinBoxTimeSabat->value()*1000*60);//ui->spinBoxTimeSabat->setValue(value);
        DebagTimerCount = ui->spinBoxTimeSabat->value()*60;
        ui->temp_lcdNumber_5->display(DebagTimerCount/60);
        qDebug()<<" from ControlSabatTime SabatVentaTimeOff ="<<SabatVentaTimeOff;
    }else{
//                SaveToLogfile("SabatTimeOnOff = 1 all off");
                DebugMssg("From ControlSabatTime:\n");
//                HDControl(GpioVanta,0);
//                HDControl(GpioLhgit,0);
                digitalWrite(GpioVanta,0);
                digitalWrite(GpioLhgit,0);
                ui->temp_label_OFF->show();
                ui->temp_label_ON->hide();
                OnOFF = 0;
                VantaTimer->stop();
         }
}

void MainWindow::on_pushButton_shbat_clicked()
{
    ui->pushButton_stop_all->click();
    bool status = ui->checkBoxSabat->isChecked();
    if (status == 0)
    {
        ui->checkBoxSabat->setChecked(true);
        SabatStatus = true;
        SabatVentaTimeOff = 1;//for loop control timers
        //SaveFile("sabat");
        ui->temp_label_OFF->hide();
        ui->temp_label_ON->show();
        //temp debug
//        HDControl(GpioLhgit,1);
//        HDControl(GpioVanta,1);
        digitalWrite(GpioLhgit,true);
        digitalWrite(GpioVanta,true);
        LhegitStatus = true;
        settings2.beginGroup("Mode");
        settings2.setValue("Mode","Sabat");
        settings2.endGroup();
        ControlSabatTime();

    }else{
//            HDControl(GpioLhgit,0);
//            HDControl(GpioVanta,0);
            digitalWrite(GpioVanta,false);
            digitalWrite(GpioLhgit,false);
            SabatTimer->stop();
            ui->checkBoxSabat->setChecked(false);
            SabatStatus = false;
            //DeleteFile();

            LhegitStatus = false;
            ui->temp_label_OFF->show();
            ui->temp_label_ON->hide();
         }
}


void MainWindow::on_pushButton_stop_all_clicked()
{
    bool status = ui->checkBoxStopAll->isChecked();
    if (status == 0)
    {
        LhegitStatus = false;
        ui->checkBoxStopAll->setChecked(true);
//        HDControl(GpioLhgit,0);
//        HDControl(GpioVanta,0);
        digitalWrite(GpioVanta,false);
        digitalWrite(GpioHit,false);
        DebagTimerCount = 0;
        DebagTimerCounthit = 0;
        DebagTimer = 0;
        DebagTimerHIT = 0;
        VantaTimer->stop();
        SabatTimer->stop();
        HitTimer->stop();
        SabatStatus =false;
        ui->temp_lcdNumber_5->display(DebagTimerCount);
        ui->temp_lcdNumber_HIT->display(DebagTimerCounthit);
        ui->checkBoxSabat->setChecked(false);
        ui->checkBoxHit->setChecked(false);
        ui->checkBoxVenta->setChecked(false);
        ui->temp_label_OFF->hide();
        ui->temp_label_ON->hide();
        SabatVentaTimeOff = 0;
//        time_on = false;
        //DeleteFile();

    }else{
            ui->checkBoxStopAll->setChecked(false);


         }
}

void MainWindow::LCDdesplyDownCounter()
{
    if(DebagTimer == 1 )
    {
        DebagTimerCount--;
        if(DebagTimerCount <=0)
        {
            Debugcounter=0;
            DebagTimerCount = 0;
        }
        ui->temp_lcdNumber_5->display(DebagTimerCount/60);

    }

    if(DebagTimerHIT == 1)
    {

        DebagTimerCounthit--;
        if(DebagTimerCounthit <=0 )
        {
            DebagTimerHIT = 0;
            DebagTimerCounthit = 0;
        }
        ui->temp_lcdNumber_HIT->display(DebagTimerCounthit/60);

    }

}
bool setuplhitoff = 0;
bool setuplhiton = 0;
void MainWindow::showTime()
{
    read_dht11_dat();
    ReadFile();
    QLocale::setDefault(QLocale(QLocale::Hebrew, QLocale::Israel));
    QLocale hebrew; // Constructs a default QLocale
        QTime DateTimetimer = QTime::currentTime();
    QString text_Time = DateTimetimer.toString("hh:mm:ss");
    if ((DateTimetimer.second() % 2) == 0)
        text_Time[2] = ' ';
    ui->label_clock->setText(text_Time);
    QDate DateT = QDate::currentDate();
    QString Date_Time = DateT.toString();

    ui->labelDate->setLocale(hebrew);
    ui->labelDate->setText(Date_Time);

    LCDdesplyDownCounter();
    //QTime Timet_h = QTime::currentTime();
    QString TimeH = DateTimetimer.toString("hh");
    int time_int = TimeH.toInt();
    int timeval = ui->spinBoxSabatOff->value();
    if (timeval == 24)
        timeval = 0;
    //qDebug()<<"  TimeH"<<TimeH << " time_int "<< time_int;
    QString mssg;
    if(SabatVentaTimeOff == 1)
    {
        if((time_int == timeval)&&(setuplhitoff == 0))//if time off
        {
            SabatTimeOnOff = 1;
            setuplhiton = 0;
            setuplhitoff = 1;
            ControlSabatTime();
//            qDebug()<<"Off  TimeH"<<TimeH << " time_int "<< time_int;
            mssg = "time:" + TimeH +" BoxVal " + ui->spinBoxSabatOff->text()+" showTime all off";
            //SaveToLogfile("showTime all off");
//            SaveToLogfile(mssg);
        }else{
                if ((time_int == ui->spinBoxSabatOn->value())&&(setuplhiton == 0))
                {
                    setuplhiton = 1;
                    setuplhitoff = 0;
                    SabatTimeOnOff = 0;
                    OnOFF = 1;
                    ControlSabatTime();
//                    qDebug()<<"On  TimeH"<<TimeH << " time_int "<< time_int;
                    //SaveToLogfile("showTime all on");
                    mssg = "time:" + TimeH + +" BoxVal " + ui->spinBoxSabatOn->text() + " showTime all on";
//                    SaveToLogfile(mssg);
                }
            }
    }
}


void MainWindow::on_horizontalScrollBarVenta_valueChanged(int value)
{
    ui->spinBoxTimeVanta->setValue(value);
    settings2.beginGroup("VantaTime");
    settings2.setValue("Time",value);
    settings2.endGroup();
}

void MainWindow::on_horizontalScrollBarHIT_valueChanged(int value)
{
    ui->spinBoxTimeHit->setValue(value);
    settings2.beginGroup("HitTime");
    settings2.setValue("Time",value);
    settings2.endGroup();
}

void MainWindow::on_horizontalScrollBarSabat_valueChanged(int value)//off
{
    ui->spinBoxTimeSabat->setValue(value);
    settings2.beginGroup("VentaSabatTime");
    settings2.setValue("Time",value);
    settings2.endGroup();
}

void MainWindow::on_horizontalScrollBarSabatOff_valueChanged(int value)
{
    ui->spinBoxSabatOff->setValue(value);
    settings2.beginGroup("SabatTime");
    settings2.setValue("SabatTimeOff",value);
    settings2.endGroup();
}

void MainWindow::on_horizontalScrollBarHumidity_valueChanged(int value)
{
    ui->spinBoxHumidity->setValue(value);
    settings2.beginGroup("Humidity");
    settings2.setValue("Humy",value);
    settings2.endGroup();
}


void MainWindow::on_horizontalScrollBarTimeAdd_valueChanged(int value)
{
    ui->spinBoxTimeAdd->setValue(value);
    settings2.beginGroup("Humidity");
    settings2.setValue("Time",value);
    settings2.endGroup();
}

void MainWindow::on_pushButtonLhigt_clicked()
{
    digitalWrite(GpioLhgit,false);
//    HDControl(GpioLhgit,0);
}

void MainWindow::on_horizontalScrollBarSabaton_valueChanged(int value)
{
    ui->spinBoxSabatOn->setValue(value);
    settings2.beginGroup("SabatTime");
    settings2.setValue("SabatTimeOn",value);
    settings2.endGroup();
}

void MainWindow::on_pushButton_clicked()
{
    ReadFile();
}


void MainWindow::on_horizontalScrollBarSonar_valueChanged(int value)
{
    ui->spinBoxSabatOnSonar->setValue(value);
    settings2.beginGroup("Sonar");
    settings2.setValue("value",value);
    settings2.endGroup();
}









