#ifndef PACKET_H
#define PACKET_H

#include <QByteArray>
#include <QString>

class Packet
{
    QByteArray flag;
    QByteArray dstAddr;
    QByteArray srcAddr;
    QByteArray data;
    QByteArray FCS;

public:
    Packet();

    void setData(const QByteArray& newData);
    QByteArray getData();

    void setSrcAddr(int comNumber);

    QByteArray toByteArray();
    void toPacket(const QByteArray& byteArray);
};

#endif // PACKET_H
