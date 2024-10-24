#ifndef SERIALPORTRECEIVER_H
#define SERIALPORTRECEIVER_H

#include "serialport.h"

class SerialPortReceiver : public SerialPort
{
    Q_OBJECT

public:
    explicit SerialPortReceiver(QObject *parent = nullptr);

    void changePort(const QString& newPort);
    QByteArray destuff(const QByteArray& data);

signals:
    void dataReceived(const QByteArray& data);

private slots:
    void handleRead();
};

#endif // SERIALPORTRECEIVER_H
