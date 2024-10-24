#ifndef SERIALPORTSENDER_H
#define SERIALPORTSENDER_H

#include "serialport.h"

class SerialPortSender : public SerialPort
{
    Q_OBJECT

public:
    explicit SerialPortSender(QObject* parent = nullptr);

    void changePort(const QString& newPort);
    void send(const QString& data);
    QByteArray stuff(const QByteArray& data);

signals:
    void stuffed(const QByteArray& stuffedData);
};

#endif // SERIALPORTSENDER_H
