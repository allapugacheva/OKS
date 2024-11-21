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

int Packet::getMSBPosotion(unsigned long long number) {

    if(number == 0)
        return -1;

    int position = 0;
    while(number >>= 1)
        position++;

    return position;
}

int Packet::weight(int number) {

    int weight = 0;

    while(number) {
        weight += number & 1;
        number >>= 1;
    }

    return weight;
}

int Packet::divide(unsigned long long divident) {

    if (divident == 0)
        memcpy(&divident, data.constData(), qMin((int)sizeof(divident), data.size()));
    int divider = 67;

    while(getMSBPosotion(divident) >= getMSBPosotion(divider))
        divident = divident ^ (static_cast<unsigned long long>(divider) << (getMSBPosotion(divident) - getMSBPosotion(divider)));

    return static_cast<int>(divident);
}

void Packet::error() {

    if((rand() % 100) < 40) {

        int byte = rand() % 5;
        int bit = rand() % 8;

        data[byte] ^= (1 << bit);
    }
}

void Packet::setData(const QByteArray& newData)
{
    data.fill(0,5);
    for(int i = 0; i<newData.size() && i < 5; i++)
        data[i] = newData[i];

    FCS[0] = static_cast<char>(divide());
}

unsigned long long Packet::shiftRight(unsigned long long data, int shift) {
    const int numBits = 40;
    const unsigned long long mask = (1ULL << numBits) - 1;

    shift = shift % numBits;
    unsigned long long relevantBits = data & mask;
    relevantBits = (relevantBits >> shift) | (relevantBits << (numBits - shift));

    data = (data & ~mask) | (relevantBits & mask);

    return data;
}

unsigned long long Packet::shiftLeft(unsigned long long data, int shift) {
    const int numBits = 40;
    const unsigned long long mask = (1ULL << numBits) - 1;

    shift = shift % numBits;
    unsigned long long relevantBits = data & mask;
    relevantBits = (relevantBits << shift) | (relevantBits >> (numBits - shift));

    data = (data & ~mask) | (relevantBits & mask);

    return data;
}

void Packet::repair() {

    unsigned long long divident;
    memcpy(&divident, data.constData(), qMin((int)sizeof(divident), data.size()));

    int tempCRC = divide(divident);
    int n = 0;

    while(1) {
        if(weight(tempCRC) <= 1) {
            divident ^= tempCRC;
            divident = shiftRight(divident, n);
            break;
        }
        else {
            divident = shiftLeft(divident, 1);
            n++;
            tempCRC = divide(divident);
        }
    }

    for(int i = 0; i < 5; i++) {
        data[i] = static_cast<char>(divident & 0xFF);
        divident >>= 8;
    }
}

QByteArray Packet::getData()
{
    //error();
    int tempFCS = divide();
    int currFCS = FCS[0];

    if(tempFCS != currFCS)
        repair();

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
