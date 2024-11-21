#include "serialport.h"

SerialPort::SerialPort(QObject* parent) : QObject(parent) {

    hSerialPort = new QSerialPort();
    hSerialPort->setBaudRate(QSerialPort::Baud9600);
    hSerialPort->setDataBits(QSerialPort::Data8);
    hSerialPort->setParity(QSerialPort::NoParity);
    hSerialPort->setStopBits(QSerialPort::OneStop);
    hSerialPort->setFlowControl(QSerialPort::NoFlowControl);
    p = new Packet();
}

SerialPort::~SerialPort() {

    delete hSerialPort;
    delete p;
}

void SerialPort::changePort(const QString &newPort, QIODeviceBase::OpenMode mode)
{
    close();
    hSerialPort->setPortName(newPort);

    if(!hSerialPort->open(mode))
        throw 1;
}

bool SerialPort::isOpen()
{
    return hSerialPort->isOpen();
}

QString SerialPort::getPortName()
{
    return hSerialPort->portName();
}

void SerialPort::close()
{
    if(hSerialPort->isOpen())
        hSerialPort->close();
}



