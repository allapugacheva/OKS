#include "serialportsender.h"

SerialPortSender::SerialPortSender(QObject* parent) : SerialPort(parent) {}

void SerialPortSender::changePort(const QString &newPort)
{
    SerialPort::changePort(newPort, QIODevice::WriteOnly);
    p->setSrcAddr(getPortName().mid(3).toInt());
}

void SerialPortSender::send(const QString& data)
{
    QByteArray temp = data.toUtf8() + '\n', result;
    while(temp.length() > 0) {

        p->setData(temp.left(5));
        result += stuff(p->toByteArray());
        temp.remove(0,5);
    }
    if(hSerialPort->write(result) == -1)
        throw 1;
    emit stuffed(result);
}

QByteArray SerialPortSender::stuff(const QByteArray &data)
{
    QByteArray stuffedData;
    for(int i = 0; i<data.size(); i++)
        if(i < data.size() - 1 && i != 0 && data[i] == 0x40 && data[i+1] == 0x65) {

            stuffedData.append(0x7D);
            stuffedData.append(0x5D);
            stuffedData.append(0x7D);
            stuffedData.append(0x5E);

            i++;
        } else if(data[i] == 0x7D) {
            stuffedData.append(0x7D);
            stuffedData.append(0x5F);
        }
        else
            stuffedData.append(data[i]);

    return stuffedData;
}
