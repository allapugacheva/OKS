#ifndef PACKET_H
#define PACKET_H

#include <QByteArray>
#include <QString>

#include <cstdlib>
#include <ctime>
#include <bit>
#include <cstdint>

#include <QDebug>

class Packet
{
    QByteArray flag;
    QByteArray dstAddr;
    QByteArray srcAddr;
    QByteArray data;
    QByteArray FCS;

    int getMSBPosotion(unsigned long long number);
    int weight(int number);
    int divide(unsigned long long divident = 0);
    void error();
    void repair();
    unsigned long long shiftLeft(unsigned long long data, int shift);
    unsigned long long shiftRight(unsigned long long data, int shift);

public:
    Packet();

    void setData(const QByteArray& newData);
    QByteArray getData();

    void setSrcAddr(int comNumber);

    QByteArray toByteArray();
    void toPacket(const QByteArray& byteArray);
};

#endif // PACKET_H
