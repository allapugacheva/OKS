#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    ComPair(new COMPair())
{
    ui->setupUi(this);
    this->setFixedSize(470, 425);
    this->setWindowTitle("COM передатчик 2000");
    this->setWindowIcon(QIcon("D:/OKS/Lab1/com.png"));

    receivedAmount = 0;

    ui->label_3->setText("Параметры COM-портов:");
    ui->label_4->setText("Скорость передачи: 9600 бит/с, количество бит в 1 байте: 8");
    ui->label_14->setText("Количество стоп-бит: 1, паритет: нет, управление потоком: нет");

    checkSerialPorts();

    connect(&ComPair->hComReceiver, &QSerialPort::readyRead, this, &MainWindow::handleRead);
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &MainWindow::on_pushButton_clicked);

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::checkSerialPorts);
    timer->start(5000);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete ComPair;
}

void MainWindow::checkSerialPorts()
{
    ui->comboBox->blockSignals(true);
    ui->comboBox_2->blockSignals(true);

    QString senderName = "порт не задан", senderPair = "", receiverName = "порт не задан", receiverPair = "";
    if(ComPair != nullptr) {
        if(ComPair->hComSender.isOpen()) {
            senderName = ComPair->hComSender.portName();
            senderPair = QString("COM") + QString::number(senderName.mid(3).toInt() + 1);
        }
        if(ComPair->hComReceiver.isOpen()) {
            receiverName = ComPair->hComReceiver.portName();
            receiverPair = QString("COM") + QString::number(receiverName.mid(3).toInt() - 1);
        }
    }

    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

    ports.removeIf([&](const QSerialPortInfo& portInfo){
        return ((portInfo.portName() == senderPair) || (portInfo.portName() == receiverPair) || (portInfo.portName() == senderName) || (portInfo.portName() == receiverName));
    });

    QStringList available;

    for(int i = 0; i < ports.size(); i++) {

        QSerialPort tempPort;
        tempPort.setPortName(ports[i].portName());

        if(tempPort.open(QIODevice::ReadWrite)) {
            tempPort.close();

            available.append(ports[i].portName());
        }
    }

    if(receiverName != "порт не задан")
        available.append(receiverName);

    available.sort();
    available.prepend("порт не задан");

    ui->comboBox_2->clear();
    ui->comboBox_2->addItems(available);

    if(receiverName != "порт не задан")
        ui->comboBox_2->setCurrentText(receiverName);

    available.removeAll("порт не задан");
    if(receiverName != "порт не задан")
        available.removeAll(receiverName);

    if(senderName != "порт не задан")
        available.append(senderName);

    available.sort();
    available.prepend("порт не задан");

    ui->comboBox->clear();
    ui->comboBox->addItems(available);

    if(senderName != "порт не задан")
        ui->comboBox->setCurrentText(senderName);

    ui->comboBox->blockSignals(false);
    ui->comboBox_2->blockSignals(false);
}

void MainWindow::handleRead()
{
    QByteArray data = ComPair->hComReceiver.readAll();

    if(ui->label_4->text() != "") {
        ui->label_4->setText("");
        ui->label_14->setText("");
    }
    ui->textEdit->setText(data);

    switch((++receivedAmount) % 10) {
    case 1:
        if(receivedAmount % 100 != 11)
            ui->label_3->setText(QString("Получена ") + QString::number(receivedAmount) + QString(" порция данных."));
        else
            ui->label_3->setText(QString("Получено ") + QString::number(receivedAmount) + QString(" порций данных."));
        break;
    case 2:
    case 3:
    case 4:
        if(receivedAmount % 100 < 12 || receivedAmount % 100 > 14)
            ui->label_3->setText(QString("Получено ") + QString::number(receivedAmount) + QString(" порции данных."));
        else
            ui->label_3->setText(QString("Получено ") + QString::number(receivedAmount) + QString(" порций данных."));
        break;
    default:
        ui->label_3->setText(QString("Получено ") + QString::number(receivedAmount) + QString(" порций данных."));
        break;
    }
}

void MainWindow::on_pushButton_clicked()
{
    if(!ComPair->hComSender.isOpen())
        QMessageBox::critical(this, "Ошибка при отправке данных", "Выберите COM порт");
    else if(ui->lineEdit->text() == "")
        QMessageBox::critical(this, "Ошибка при отправке данных", "Введите данные");
    else {
        QByteArray writeData = ui->lineEdit->text().toUtf8();
        if(ComPair->hComSender.write(writeData) == -1)
            QMessageBox::critical(this, "Ошибка при отправке данных", ComPair->hComSender.errorString());
        ui->lineEdit->setText("");
    }
}

void MainWindow::on_comboBox_currentTextChanged(const QString &portName)
{
    if(ComPair != nullptr) {
        try {
            if(portName != "не задан")
                ComPair->changeSender(portName);
            else
                ComPair->hComSender.close();
        }
        catch(...) {

            QMessageBox::critical(this, "Ошибка", "Не удалось изменить COM порт.");
            ui->comboBox->setCurrentText(ComPair->hComSender.portName());
        }

        checkSerialPorts();
    }
}

void MainWindow::on_comboBox_2_currentTextChanged(const QString &portName)
{
    if(ComPair != nullptr) {
        try {
            if(portName != "порт не задан")
                ComPair->changeReceiver(portName);
            else
                ComPair->hComReceiver.close();
        }
        catch(...) {

            QMessageBox::critical(this, "Ошибка", "Не удалось изменить COM порт.");
            ui->comboBox_2->setCurrentText(ComPair->hComReceiver.portName());
        }

        checkSerialPorts();
    }
}

