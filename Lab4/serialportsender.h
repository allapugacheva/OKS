#ifndef SERIALPORTSENDER_H
#define SERIALPORTSENDER_H

#include "serialport.h"
#include <cmath>
#include <algorithm>
#include <windows.h>

class SerialPortSender : public SerialPort
{
    Q_OBJECT

    QList<int> collisions;

    bool listen();
    bool collision();
    void randomDelay(int tries);
    void collisionWindow();

public:
    explicit SerialPortSender(QObject* parent = nullptr);

    void changePort(const QString& newPort);
    void send(const QString& data);
    QByteArray stuff(const QByteArray& data);

    int getNCollisions(int n);

signals:
    void stuffed(const QByteArray& stuffedData);
};

#endif // SERIALPORTSENDER_H
