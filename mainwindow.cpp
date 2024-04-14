/*
 * VERSION 2.3
 *
 *
 * add time selact chakbox
 * 5m ,10m ,15m, 20m ,25m ,30m
 * add time set for relay time disply count down
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
#include <wiringPi.h>
#include <iostream>
#include "libSonar.h"

int trigger = 4;//23;
int echo = 5;//24;



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
bool time_off=false,time_on=false;
//define all the day
#define sanday 7
#define mandy 0


//void wiringPiSetup(void);
void timerVentaHendel(void);
void timerHitHendel(void);
int CaeckFile();
void SaveFile(QString mode);
void DeleteFile(void);
int ChakVantaConfig(void);
QString GetConfigMode(void);

using namespace std;		// No need to keep using “std”
QSettings settings2("/home/pi/Desktop/venta.ini", QSettings::IniFormat);

#define MAXTIMINGS	85
#define DHTPIN		4

int dht11_dat[5] = { 0, 0, 0, 0, 0 };

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

double sonar_data;
void  MainWindow::StartSonar()
{
    if (wiringPiSetup() == -1)
    {
        //return -1;
        exit;
    }
    else{
        Sonar sonar;
        sonar.init(trigger, echo);
        //QString sonar_data = QString("Distance is %1 cm.").arg(sonar.distance(30000));
        //double valueAsDouble = 1.2;

        sonar_data = sonar.distance(30000);
//        QString valueAsString = QString::number(sonar_data);
//        ui->lineEdit->insert(valueAsString);
        //ui->textEdit->setText(sonar_data);
        //while(1)
        //{
            //cout << "Distance is " << sonar.distance(30000) << " cm." << endl;
        //}
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    wiringPiSetupGpio();
    pinMode(GpioVanta, OUTPUT);
    pinMode(GpioHit, OUTPUT);
    pinMode(InputGpio, INPUT);
    pinMode(GpioLhgit, OUTPUT);
    digitalWrite(GpioVanta,false);
    digitalWrite(GpioHit,false);
    //digitalWrite(GpioLhgit,false);
    DebagTimerCount = 30*1000*60;
    DebagTimer = 0;
    ui->temp_label_OFF->hide();
    ui->temp_label_ON->hide();
    ui->temp_label_OFF_HIT->hide();
    ui->temp_label_ON_HIT->hide();
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
//    if (CaeckFile() == 1)
//        ui->pushButton_shbat->click();
//    else {
//        DeleteFile();
//    }
    //delay(500);
    int chack = ChakVantaConfig();
    if (chack == 1)
    {
        SetUpTimeSetingFromConfigFile();
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
}

MainWindow::~MainWindow()
{
    delete ui;
}

int ChakVantaConfig(void)
{
    settings2.beginGroup("Mode");
    QString sysmode = settings2.value("Mode").toByteArray().constData();
    settings2.endGroup();

    settings2.beginGroup("VantaTime");
    QString ventatime = settings2.value("Time").toByteArray().constData();
    settings2.endGroup();

    settings2.beginGroup("HitTime");
    QString hittime = settings2.value("Time").toByteArray().constData();
    settings2.endGroup();

    settings2.beginGroup("SabatTime");
    QString sabattime = settings2.value("Time").toByteArray().constData();
    settings2.endGroup();
    if(ventatime.toInt()!=0)
    {
       if(hittime.toInt()!=0)
       {
           if(sabattime.toInt()!=0)
           {
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

QString GetConfigMode(void)
{
    settings2.beginGroup("Mode");
    qDebug() << "Mode : " << settings2.value("Mode").toByteArray().constData();
    QString sysmode = settings2.value("Mode").toByteArray().constData();
    settings2.endGroup();
    return (sysmode);
}

void MainWindow::SetUpTimeSetingFromConfigFile(void)
{
    settings2.beginGroup("VantaTime");
    QString ventatime = settings2.value("Time").toByteArray().constData();
    settings2.endGroup();
    settings2.beginGroup("HitTime");
    QString hittime = settings2.value("Time").toByteArray().constData();
    settings2.endGroup();
    settings2.beginGroup("SabatTime");
    QString sabattime = settings2.value("Time").toByteArray().constData();
    settings2.endGroup();
    ///sabat time off on on
    settings2.beginGroup("SabatTimeClose");
    QString timeCloseH = settings2.value("TimeH").toByteArray().constData();
//    QString timeCloseM = settings2.value("TimeM").toByteArray().constData();
    settings2.endGroup();
    settings2.beginGroup("SabatTimeOpen");
    QString timeOpenH = settings2.value("TimeH").toByteArray().constData();
//    QString timeOpenM = settings2.value("TimeM").toByteArray().constData();
    settings2.endGroup();



    if (ventatime.toInt()<=0)
    {
        ui->spinBoxTimeVanta->setValue(15);
        ui->horizontalScrollBarVenta->setValue(15);
    }else{
            ui->spinBoxTimeVanta->setValue(ventatime.toInt());
            ui->horizontalScrollBarVenta->setValue(ventatime.toInt());
         }
    if(hittime.toInt()<=0)
    {
         ui->spinBoxTimeHit->setValue(5);
         ui->horizontalScrollBarHIT->setValue(15);
    }else{
        ui->spinBoxTimeHit->setValue(hittime.toInt());
        ui->horizontalScrollBarHIT->setValue(hittime.toInt());
    }
    if(sabattime.toInt()<=0)
    {
        ui->spinBoxTimeSabat->setValue(15);
        ui->horizontalScrollBarSabat->setValue(15);
    }else{
        ui->spinBoxTimeSabat->setValue(sabattime.toInt());
        ui->horizontalScrollBarSabat->setValue(sabattime.toInt());
    }
//    int ttt= timeCloseH.compare("0:0");
//    if(timeCloseH.compare("00:00")==0)
//    {
//        ui->timeEditOff->setTime(QTime(23,45,00));
//        ui->timeEditOn->setTime(QTime(07,20,00));
//    }else{
//        ui->timeEditOff->setTime(QTime(timeCloseH.toInt(),0));
//        ui->timeEditOn->setTime(QTime(timeOpenH.toInt(),0));
//    }

}

int CaeckFile()
{
    QFile file("SaveMode.txt");
    // QFile(filename).setPermissions(QFile::ReadOwner)
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qDebug() << " No File Fuond =SaveMode.txt" ;
        return (0);
    }
    QTextStream in(&file);
    QString line = in.readLine();
    //int stam = line.compare("sabat");

   qDebug() << "Read from Text :" << line << "comper :" <<line ;
   if(line.compare("sabat")==0)
   {
      qDebug() << "the contin is equal to mode 3: t\n" ;
      return (1);
   }
return (0);
}

void SaveFile(QString mode)
{
    QFile file("SaveMode.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << " No File Fuond =SaveMode.txt" ;
        return ;
    }
    QTextStream out(&file);
        //out << "mode 3" ;
        out << mode;

    file.close();
     qDebug() << "SaveMode.txt creating" ;
}

void DeleteFile(void)
{
    QFile file("SaveMode.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return;
    }
    else{
         file.remove();
         qDebug() << "File deleted" ;

        }

}



void MainWindow::StopTimerVenta()
{
    if(dht11_dat[0] > 65)
    {
        VantaTimer->start(ui->spinBoxTimeVanta->value()*1000*60);
        DebagTimer = 1;
        DebagTimerCount = 10*60;// ui->spinBoxTimeVanta->value()*60;
        ui->temp_lcdNumber_5->display(DebagTimerCount/60);
    }else {
            ui->checkBoxVenta->setChecked(false);
            digitalWrite(GpioVanta, LOW);
            VantaTimer->stop();
            DebagTimerCount = 0;
            DebagTimer = 0;
            ui->temp_label_OFF->show();
            ui->temp_label_ON->hide();
            //DeleteFile();
            //temp debug
            digitalWrite(GpioLhgit,false);
            LhegitStatus = false;
           }

}

void MainWindow::StopTimerHit()
{
    ui->checkBoxHit->setChecked(false);
    LhegitStatus = false;
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
    //DeleteFile();






}

void MainWindow::GetGPIO()
{
    NewValue = digitalRead(12);
    if(oldValue != NewValue)
    {
        oldValue = NewValue;
        delay(100);
        if (NewValue == 0 && LhegitStatus == false )
        {
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
                    digitalWrite(GpioLhgit,0);
                    LhegitStatus = false;

                }
              }
    }
}
//bool status = 1;
void MainWindow::on_pushButton_venta_clicked()
{
    if (SabatStatus == 0)
    {
        bool status = ui->checkBoxVenta->isChecked();
        if (status == 0)
        {
            ui->checkBoxVenta->setChecked(true);
            VantaTimer->start(ui->spinBoxTimeVanta->value()*1000*60);
            //VantaTimer->start(ui->spinBoxTimeVanta->value()*1000);
            DebagTimer = 1;
            DebagTimerCount = ui->spinBoxTimeVanta->value()*60;
            ui->temp_lcdNumber_5->display(DebagTimerCount/60);
            digitalWrite(GpioLhgit,true);
            digitalWrite(GpioVanta, true);
            //creat file and save
            //SaveFile("venta");
            ui->temp_label_OFF->hide();
            ui->temp_label_ON->show();
            settings2.beginGroup("Mode");
            settings2.setValue("Mode","Vanta");
            settings2.endGroup();

        }else{

                digitalWrite(GpioVanta, false);
                ui->checkBoxVenta->setChecked(false);
                ui->pushButton_stop_all->click();
                VantaTimer->stop();
                DebagTimerCount = 0;
                DebagTimer = 0;
                ui->temp_label_OFF->show();
                ui->temp_label_ON->hide();
                //temp debug
                digitalWrite(GpioLhgit,false);
                LhegitStatus = false;
                //delet save file
                DeleteFile();

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
        digitalWrite(GpioHit, HIGH);
        DebagTimerCounthit = ui->spinBoxTimeHit->value()*60;
        ui->temp_lcdNumber_HIT->display(DebagTimerCounthit/60);
        ui->temp_label_OFF_HIT->hide();
        ui->temp_label_ON_HIT->show();
        DebagTimerHIT = 1;
        //SaveFile("hit");
        settings2.beginGroup("Mode");
        settings2.setValue("Mode","Hit");
        settings2.endGroup();
    }else {
            ui->checkBoxHit->setChecked(false);
            digitalWrite(GpioHit, LOW);
            HitTimer->stop();
            DebagTimerCounthit = 0;
            DebagTimerHIT = 0;
            ui->temp_label_OFF_HIT->show();
            ui->temp_label_ON_HIT->hide();
            ui->temp_lcdNumber_HIT->display(DebagTimerCounthit);
            //DeleteFile();
          }

}


void MainWindow::ControlSabatTime()
{
    if(SabatVentaTimeOff == 0)
    {
            DebagTimer = 1;
            if (OnOFF == 1)
            {
                digitalWrite(GpioVanta,1);
                ui->temp_label_OFF->hide();
                ui->temp_label_ON->show();
                OnOFF = 0;
            }else{
                    //DebagTimerCount = 0;
                    digitalWrite(GpioVanta,0);//now it 0
                    ui->temp_label_OFF->show();
                    ui->temp_label_ON->hide();
                    OnOFF = 1;
                  }

    qDebug()<<"normal mode ControlSabatTime SabatVentaTimeOff = 0";
    }
    SabatTimer->start(ui->spinBoxTimeSabat->value()*1000*60);
    DebagTimerCount = ui->spinBoxTimeSabat->value()*60;
    ui->temp_lcdNumber_5->display(DebagTimerCount/60);
    qDebug()<<"ControlSabatTime SabatVentaTimeOff ="<<SabatVentaTimeOff;

}

void MainWindow::on_pushButton_shbat_clicked()
{
    ui->pushButton_stop_all->click();
    bool status = ui->checkBoxSabat->isChecked();
    if (status == 0)
    {
        ui->checkBoxSabat->setChecked(true);
        SabatStatus = true;
        //SaveFile("sabat");
        ui->temp_label_OFF->hide();
        ui->temp_label_ON->show();
        //temp debug
        digitalWrite(GpioLhgit,true);
        digitalWrite(GpioVanta,true);
        LhegitStatus = true;
        settings2.beginGroup("Mode");
        settings2.setValue("Mode","Sabat");
        settings2.endGroup();

        settings2.beginGroup("SabatTimeClose");
        QString timeCloseH = settings2.value("TimeH").toByteArray().constData();
        QString timeCloseM = settings2.value("TimeM").toByteArray().constData();
        settings2.endGroup();
        settings2.beginGroup("SabatTimeOpen");
        QString timeOpenH = settings2.value("TimeH").toByteArray().constData();
        QString timeOpenM = settings2.value("TimeM").toByteArray().constData();
        settings2.endGroup();
        ControlSabatTime();

    }else{
            digitalWrite(GpioVanta,false);
            SabatTimer->stop();
            ui->checkBoxSabat->setChecked(false);
            SabatStatus = false;
            //DeleteFile();
            digitalWrite(GpioLhgit,false);
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
        //digitalWrite(GpioLhgit,FALSE);
        ui->checkBoxStopAll->setChecked(true);
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
        time_on = false;
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

void MainWindow::showTime()
{
    read_dht11_dat();

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
    //StartSonar();


}


void MainWindow::on_horizontalScrollBarVenta_valueChanged(int value)
{
    ui->spinBoxTimeVanta->setValue(value);
    settings2.beginGroup("VantaTime");
    settings2.setValue("Time",ui->spinBoxTimeVanta->value());
    settings2.endGroup();

}

void MainWindow::on_horizontalScrollBarHIT_valueChanged(int value)
{
    ui->spinBoxTimeHit->setValue(value);
    settings2.beginGroup("HitTime");
    settings2.setValue("Time",ui->spinBoxTimeHit->value());
    settings2.endGroup();
}

void MainWindow::on_horizontalScrollBarSabat_valueChanged(int value)
{
    ui->spinBoxTimeSabat->setValue(value);
    settings2.beginGroup("SabatTime");
    settings2.setValue("Time",ui->spinBoxTimeSabat->value());
    settings2.endGroup();
}


void MainWindow::on_timeEditOff_timeChanged(const QTime &time)
{
    settings2.beginGroup("SabatTimeClose");
//    settings2.setValue("TimeH",ui->timeEditOff->text());
    settings2.endGroup();
}

void MainWindow::on_timeEditOn_timeChanged(const QTime &time)
{
    settings2.beginGroup("SabatTimeOpen");
//    settings2.setValue("TimeH",ui->timeEditOn->text());
    settings2.endGroup();
}

