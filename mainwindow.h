#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QtCore>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void timerEvent(QTimerEvent *event);

private slots:
    void on_scrResyLE_textChanged(const QString &arg1);
    void on_scrResxLE_textChanged(const QString &arg1);
    void on_runCmdPB_clicked();
    void updateImage();
    void on_scrUpdateLE_returnPressed();

private:
    Ui::MainWindow *ui;
    qint32 currpic;
    qint32 scrResy;
    qint32 scrResx;
    qint32 screentimeout;
    qint32 regulartimeout;
    QBasicTimer timer;
    QPoint dragStartPosition;
};

#endif // MAINWINDOW_H
