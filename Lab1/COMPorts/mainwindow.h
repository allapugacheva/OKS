#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QKeyEvent>
#include <QEvent>
#include <QMessageBox>
#include <algorithm>
#include <QSerialPortInfo>
#include <QThread>
#include "serialportsender.h"
#include "serialportreceiver.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    SerialPortSender* sender;
    SerialPortReceiver* receiver;

    int sendedAmount;

public:
    MainWindow(QWidget *parent = nullptr);
    bool eventFilter(QObject* obj, QEvent* event) override;
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void checkSerialPorts();
    void handleSend();

private slots:
    void handleRead(const QByteArray& data);
    void handleStuff(const QByteArray& stuffedData);
    void onReceiverComboBoxCurrentTextChanged(const QString &arg1);
    void onSenderComboBoxCurrentTextChanged(const QString &arg1);
};
#endif // MAINWINDOW_H
