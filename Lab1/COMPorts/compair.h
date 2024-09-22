#ifndef COMPAIR_H
#define COMPAIR_H

#include <QMessageBox>
#include <QString>
#include <QtSerialPort>
#include <algorithm>

const QSerialPort::Parity parityTypes[] = {QSerialPort::NoParity, QSerialPort::EvenParity, QSerialPort::OddParity, QSerialPort::SpaceParity, QSerialPort::MarkParity};

class COMPair
{
    QSerialPort hComSender;
    QSerialPort hComReceiver;

public:
    COMPair();
    ~COMPair();

    void changeSender(const QString& portName);
    void changeReceiver(const QString& portName);

    friend class MainWindow;
};

#endif // COMPAIR_H
