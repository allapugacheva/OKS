#include "packet.h"

Packet::Packet()
{
    flag.fill(0,2);
    flag[0] = '@';
    flag[1] = 'e';
    dstAddr.fill(0,1);
    srcAddr.fill(0,1);
    data.fill(0,5);
    FCS.fill(0,1);
}

void Packet::setData(const QByteArray& newData)
{
    data = newData;
    if(data.size() != 5)
        data.resize(5);
}

QByteArray Packet::getData()
{
    return data;
}

void Packet::setSrcAddr(int comNumber)
{
    srcAddr[0] = comNumber;
}

QByteArray Packet::toByteArray()
{
    QByteArray result = flag + dstAddr + srcAddr + data + FCS;
    return result;
}

void Packet::toPacket(const QByteArray& byteArray)
{
    flag[0] = byteArray[0];
    flag[1] = byteArray[1];
    dstAddr[0] = byteArray[2];
    srcAddr[0] = byteArray[3];
    for(int i = 0; i<5; i++)
        data[i] = byteArray[4 + i];
    FCS[0] = byteArray[9];
}
