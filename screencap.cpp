#include <QtCore>

#include "screencap.h"

ScreenCap::ScreenCap(qint32 desscreentimeout)
{
    screentimeout = desscreentimeout;
}

void ScreenCap::GetScreen()
{
    // screen capture
    qDebug() << "Started screencap" << QDateTime::currentDateTime().toString(Qt::ISODate);
    QProcess process;
    process.start("adb shell screencap -p /sdcard/curr.png");
    if(process.waitForFinished(screentimeout))
    {
        qDebug() << "Finished screencap" << QDateTime::currentDateTime().toString(Qt::ISODate);
        QProcess process2;
        qDebug() << "Started pull" << QDateTime::currentDateTime().toString(Qt::ISODate);
        process2.start("adb pull /sdcard/curr.png");
        process2.waitForFinished(screentimeout);
        qDebug() << "Started pull" << QDateTime::currentDateTime().toString(Qt::ISODate);
        //QPixmap myQP = QPixmap("curr.png");
    }

    emit finished();
}
