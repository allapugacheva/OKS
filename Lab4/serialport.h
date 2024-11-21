
#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QSerialPort>
#include <QObject>
#include "packet.h"

class SerialPort : public QObject
{
    Q_OBJECT

protected:
    QSerialPort* hSerialPort;
    Packet* p;

public:
    explicit SerialPort(QObject* parent = nullptr);
    virtual ~SerialPort();

    void changePort(const QString& newPort, QIODeviceBase::OpenMode mode);
    bool isOpen();
    QString getPortName();
    void close();
};

#endif // SERIALPORT_H
