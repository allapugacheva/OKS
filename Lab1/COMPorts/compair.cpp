#include "compair.h"

COMPair::COMPair() {

    hComSender.setBaudRate(QSerialPort::Baud9600);
    hComSender.setDataBits(QSerialPort::Data8);
    hComSender.setParity(QSerialPort::NoParity);
    hComSender.setStopBits(QSerialPort::OneStop);
    hComSender.setFlowControl(QSerialPort::NoFlowControl);

    hComReceiver.setBaudRate(QSerialPort::Baud9600);
    hComReceiver.setDataBits(QSerialPort::Data8);
    hComReceiver.setParity(QSerialPort::NoParity);
    hComReceiver.setStopBits(QSerialPort::OneStop);
    hComReceiver.setFlowControl(QSerialPort::NoFlowControl);
}

COMPair::~COMPair() {

    if(hComSender.isOpen())
        hComSender.close();
    if(hComReceiver.isOpen())
        hComReceiver.close();
}

void COMPair::changeSender(const QString &portName)
{
    if(hComSender.isOpen())
        hComSender.close();

    hComSender.setPortName(portName);

    if(!hComSender.open(QIODevice::WriteOnly))
        throw 1;
}

void COMPair::changeReceiver(const QString &portName)
{
    if(hComReceiver.isOpen())
        hComReceiver.close();

    hComReceiver.setPortName(portName);

    if(!hComReceiver.open(QIODevice::ReadOnly))
        throw 2;
}
