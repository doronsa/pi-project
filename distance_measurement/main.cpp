#include <QCoreApplication>
#include <QCoreApplication>
#include <wiringPi.h>
#include <qdebug.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
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
#include <iostream>
#include <vector>
#include "libSonar.h"
float numbers[400];
int sizeOfarray = 20;
QSettings settings("/home/pi/Desktop/NewSonar.ini", QSettings::IniFormat);

void saveseting(QString val);
void SaveFile(QString Value);


int trigger = 4;// 16;//23;
int echo = 5;//18;//24;
using namespace std;
int newseting = 0;

QFile file("/home/pi/Desktop/NewSonar.ini");

void SaveFile(QString Value)
{
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << " No File Fuond =SaveMode.txt" ;
        return ;
    }
    QTextStream out(&file);
    out << "[Sonar]"<<Qt::endl;
    out << "Value="<<Value;
    file.close();
    qDebug() << "Save Sonar" ;
}


float average(float numbers[], int size) {
    double sum = 0;
    for (int x = 0; x < size; x++)
    {
        sum += numbers[x];
    }
    return sum /(double)size;
}

void saveseting(QString val)
{
        SaveFile(val);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (wiringPiSetup() == -1)
         return -1;

     Sonar sonar;
     sonar.init(trigger, echo);

     while(1)
     {
         for (int i=0 ;i <sizeOfarray;i++)
         {
             numbers[i] = sonar.distance(30000);
         }
         QString distance = QString::number(sonar.distance(30000));
         float sonardisd = average(numbers, 20);
         delay(100);
         saveseting(QString::number(sonardisd, 'g', 4));
//         qDebug() << "distance timer " << distance<<"sonardisd :"<<sonardisd;
         delay(5000);
     }
    return a.exec();
}
