#ifndef SCREENCAP_H
#define SCREENCAP_H

#include <QObject>


class ScreenCap : public QObject
{
    Q_OBJECT


public:
    ScreenCap(qint32 desscreentimeout);

public slots:
    void GetScreen();

signals:
    void finished();

private:
    qint32 screentimeout;

};

#endif // SCREENCAP_H
