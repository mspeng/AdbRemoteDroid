#include "mainwindow.h"
#include "screencap.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //setMouseTracking(true);
    currpic = 0;
    scrResy = 1920;
    scrResx = 1080;
    screentimeout = ui->scrUpdateLE->text().toInt();
    regulartimeout = 2000;
    timer.start(screentimeout, this);
    qDebug() << "Timer started" << timer.isActive();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::mousePressEvent(QMouseEvent *e)
{
    //ui->statusBar->showMessage(QString("%1, %2").arg(e->x()).arg(e->y()));
    if(e->button() == Qt::LeftButton)
        dragStartPosition = e->pos();
}


void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        if ((e->pos() - dragStartPosition).manhattanLength()
            < QApplication::startDragDistance())
        {
            if ((e->x() < ui->screenLa->x()) || (e->y() < ui->screenLa->y()) ||
                (e->x() > (ui->screenLa->x()+ui->screenLa->width())) ||
                (e->y() > (ui->screenLa->y()+ui->screenLa->height())))
            {
            }
            else
            {
                qint32 actX = qint32(float(e->x()-ui->screenLa->x())/ui->screenLa->width()*scrResx);
                qint32 actY = qint32(float(e->y()-ui->screenLa->y())/ui->screenLa->height()*scrResy);
                //ui->shellPTE->appendPlainText(QString("[%3] scrx: %1, scry: %2").arg(actX).arg(actY).arg(QDateTime::currentDateTime().toString(Qt::ISODate)));
                // should do tap event
                QProcess process;
                qDebug() << "Started tap" << QDateTime::currentDateTime().toString(Qt::ISODate);
                process.start(QString("adb shell input tap %1 %2").arg(actX).arg(actY));
                if(process.waitForFinished(regulartimeout))
                {
                    ui->shellPTE->appendPlainText(QString("[%1] adb shell input tap %2 %3").arg(QDateTime::currentDateTime().toString(Qt::ISODate)).arg(actX).arg(actY));
                }
            }
        }
        else
        {
            qint32 stActX = qint32(float(dragStartPosition.rx()-ui->screenLa->x())/ui->screenLa->width()*scrResx);
            qint32 stActY = qint32(float(dragStartPosition.ry()-ui->screenLa->y())/ui->screenLa->height()*scrResy);
            qint32 edActX = qint32(float(e->x()-ui->screenLa->x())/ui->screenLa->width()*scrResx);
            qint32 edActY = qint32(float(e->y()-ui->screenLa->y())/ui->screenLa->height()*scrResy);
            //ui->shellPTE->appendPlainText(QString("[%5] Drag Event from (%1,%2) to (%3,%4)").arg(stActX).arg(stActY).arg(edActX).arg(edActY).arg(QDateTime::currentDateTime().toString(Qt::ISODate)));
            // should do swipe event
            QProcess process;
            qDebug() << "Started swipe" << QDateTime::currentDateTime().toString(Qt::ISODate);
            process.start(QString("adb shell input swipe %1 %2 %3 %4").arg(stActX).arg(stActY).arg(edActX).arg(edActY));
            if(process.waitForFinished(regulartimeout))
            {
                ui->shellPTE->appendPlainText(QString("[%1] adb shell input swipe %2 %3 %4 %5").arg(QDateTime::currentDateTime().toString(Qt::ISODate)).arg(stActX).arg(stActY).arg(edActX).arg(edActY));
            }
        }
    }
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    qDebug() << QString("Timer event: %1").arg(QDateTime::currentDateTime().toString(Qt::ISODate));
    if((event->timerId() == timer.timerId()) && (ui->scrUpdateQSli->value() == 0))
    {
        QThread *workerThread;
        ScreenCap *worker;

        qDebug() << QString("Timer screencap id'd: %1").arg(QDateTime::currentDateTime().toString(Qt::ISODate));
        workerThread = new QThread;
        worker = new ScreenCap(screentimeout);
        worker->moveToThread(workerThread);
        connect(workerThread, SIGNAL(started()), worker, SLOT(GetScreen()));
        connect(worker, SIGNAL(finished()), workerThread, SLOT(quit()));
        connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
        connect(workerThread, SIGNAL(finished()), workerThread, SLOT(deleteLater()));
        connect(worker, SIGNAL(finished()), this, SLOT(updateImage()));
        workerThread->start();
    }
    else
    {
        QWidget::timerEvent(event);
    }
}

void MainWindow::updateImage()
{
    qDebug() << QString("Image Updated: %1").arg(QDateTime::currentDateTime().toString(Qt::ISODate));
    ui->screenLa->setPixmap(QPixmap("curr.png")); //myQP);
    ui->statusBar->showMessage(QString("Screen updated: %1\t\t").arg(QDateTime::currentDateTime().toString(Qt::ISODate)));
}

void MainWindow::on_scrResyLE_textChanged(const QString &arg1)
{
    scrResy = ui->scrResyLE->text().toInt();
}

void MainWindow::on_scrResxLE_textChanged(const QString &arg1)
{
    scrResx = ui->scrResxLE->text().toInt();
}

void MainWindow::on_runCmdPB_clicked()
{
    // execute command

    QProcess process;
    QString adbcmdQS = ui->cmdLE->text();
    qDebug() << "Started adb command" << QDateTime::currentDateTime().toString(Qt::ISODate);
    process.setProcessChannelMode(QProcess::MergedChannels);
    process.start(QString("adb shell %1").arg(adbcmdQS));
    if(process.waitForFinished(regulartimeout*3))
    {
        qDebug() << "Finished cmd" << QDateTime::currentDateTime().toString(Qt::ISODate);
        ui->shellPTE->appendPlainText(QString(process.readAll()));
        ui->shellPTE->appendPlainText(QString("[%1] adb shell %2").arg(QDateTime::currentDateTime().toString(Qt::ISODate)).arg(adbcmdQS));
    }
}


void MainWindow::on_scrUpdateLE_returnPressed()
{
    timer.stop();
    screentimeout = ui->scrUpdateLE->text().toInt();
    timer.start(screentimeout, this);
}
