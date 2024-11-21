#include "serialportreceiver.h"

SerialPortReceiver::SerialPortReceiver(QObject *parent) : SerialPort(parent) {
    connect(hSerialPort, &QSerialPort::readyRead, this, &SerialPortReceiver::handleRead);
}

void SerialPortReceiver::changePort(const QString &newPort)
{
    SerialPort::changePort(newPort, QIODevice::ReadOnly);
    p->setSrcAddr(getPortName().mid(3).toInt());
}

QByteArray SerialPortReceiver::destuff(const QByteArray &data)
{
    QByteArray destuffedData;
    for(int i = 0; i < data.size(); i++)
        if(i < data.size() - 1 && data[i] == 0x7D && data[i+1] == 0x5D) {
            destuffedData.append(0x40);
            destuffedData.append(0x65);

            i++;
        } else if (i < data.size() - 1 && data[i] == 0x7D && data[i+1] == 0x5F) {
            destuffedData.append(0x7D);

            i++;
        }
        else
            destuffedData.append(data[i]);

    return destuffedData;
}

void SerialPortReceiver::handleRead() {
    QByteArray data = destuff(hSerialPort->readAll()), result;
    while(data.size() >= 10) {
        p->toPacket(data.left(10));
        QByteArray temp = p->getData();
        int tempIndex = temp.indexOf(0x1B);
        if (tempIndex != -1)
            temp.resize(tempIndex);
        result += temp;
        data.remove(0,10);
    }
    // int index = result.lastIndexOf(0x1B);
    // if(index != -1)
    //     result.resize(index);
    emit dataReceived(result);
}
