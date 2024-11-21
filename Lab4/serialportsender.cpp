#include "serialportsender.h"

SerialPortSender::SerialPortSender(QObject* parent) : SerialPort(parent) {}

void SerialPortSender::changePort(const QString &newPort)
{
    SerialPort::changePort(newPort, QIODevice::WriteOnly);
    p->setSrcAddr(getPortName().mid(3).toInt());
}

void SerialPortSender::send(const QString& data)
{
    QByteArray temp = data.toUtf8(), result, tempResult;
    if(data.length() % 5 != 0)
        temp += 0x1B;
    int n = 0;
    collisions.clear();
    while(temp.length() > 0) {

        bool success = true;
        p->setData(temp.left(5));
        collisions.append(0);

        int tries = 0;
        while (true) {
            while(!listen());
            hSerialPort->write(stuff(p->toByteArray()));
            //result += stuff(p->toByteArray());
            collisionWindow();
            if (collision()) {
                tries++;
                collisions[n]++;
                if (tries > 16) {
                    success = false;
                    break;
                }
                randomDelay(tries);
            } else
                break;
        }
        if(success)
            tempResult += stuff(p->toByteArray());
        temp.remove(0,5);
        n++;
    }

    // if(hSerialPort->write(result) == -1)
    //     throw 1;
    emit stuffed(tempResult);
}

QByteArray SerialPortSender::stuff(const QByteArray &data)
{
    QByteArray stuffedData;
    for(int i = 0; i < data.size(); i++)
        if(i < data.size() - 1 && i != 0 && data[i] == 0x40 && data[i+1] == 0x65) {

            stuffedData.append(0x7D);
            stuffedData.append(0x5D);

            i++;
        } else if(data[i] == 0x7D) {
            stuffedData.append(0x7D);
            stuffedData.append(0x5F);
        }
        else
            stuffedData.append(data[i]);

    return stuffedData;
}

int SerialPortSender::getNCollisions(int n)
{
    return collisions[n];
}

bool SerialPortSender::listen()
{
    return ((rand() % 100) < 40);
}

bool SerialPortSender::collision()
{
    return ((rand() % 100) < 60);
}

void SerialPortSender::randomDelay(int tries)
{
    int delayTime = rand() % static_cast<int>(pow(2, std::min(tries, 10)));
    Sleep(delayTime);
}

void SerialPortSender::collisionWindow()
{
    Sleep(100);
}
