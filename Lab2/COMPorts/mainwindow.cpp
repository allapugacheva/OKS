#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), sender(new SerialPortSender(this)), receiver(new SerialPortReceiver(this)),sendedAmount(0)
{
    ui->setupUi(this);
    this->setFixedSize(670, 350);
    this->setWindowTitle("COM передатчик 2000");
    this->setWindowIcon(QIcon("D:/OKS/Lab1/com.png"));

    ui->statusWindowLine->setText("Параметры COM-портов: скорость передачи: 9600 бит/с, количество бит в 1 байте: 8, количество стоп-бит: 1, паритет: нет, управление потоком: нет");

    checkSerialPorts();

    connect(receiver, &SerialPortReceiver::dataReceived, this, &MainWindow::handleRead);
    connect(sender, &SerialPortSender::stuffed, this, &MainWindow::handleStuff);
    connect(ui->senderComboBox, &QComboBox::currentTextChanged, this, &MainWindow::onSenderComboBoxCurrentTextChanged);
    connect(ui->receiverComboBox, &QComboBox::currentTextChanged, this, &MainWindow::onReceiverComboBoxCurrentTextChanged);
    ui->inputWindowLine->installEventFilter(this);

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::checkSerialPorts);
    timer->start(5000);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->inputWindowLine && event->type() == QEvent::KeyPress) {

        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if(keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return) {

            handleSend();
            return true;
        }
    }
    return QObject::eventFilter(obj, event);
}

MainWindow::~MainWindow() {
    delete ui;
    delete sender;
    delete receiver;
}

void MainWindow::checkSerialPorts()
{
    ui->senderComboBox->blockSignals(true);
    ui->receiverComboBox->blockSignals(true);

    QString senderName = "", senderPair = "", receiverName = "", receiverPair = "";
    if(sender != nullptr && sender->isOpen()) {
        senderName = sender->getPortName();
        senderPair = QString("COM") + QString::number(senderName.mid(3).toInt() + 1);
    }
    if(receiver != nullptr && receiver->isOpen()) {
        receiverName = receiver->getPortName();
        receiverPair = QString("COM") + QString::number(receiverName.mid(3).toInt() - 1);
    }

    ui->senderComboBox->clear();
    ui->receiverComboBox->clear();

    ui->senderComboBox->addItem("порт не задан");
    ui->receiverComboBox->addItem("порт не задан");

    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    for(int i = 0; i < ports.size(); i++) {

        if(ports[i].portName() == senderPair || ports[i].portName() == receiverPair)
            continue;

        if(senderName != "" && ports[i].portName() == senderName) {
            ui->senderComboBox->addItem(senderName);
            ui->senderComboBox->setCurrentText(senderName);
        }

        if(receiverName != "" && ports[i].portName() == receiverName) {
            ui->receiverComboBox->addItem(receiverName);
            ui->receiverComboBox->setCurrentText(receiverName);
        }

        QSerialPort tempPort;
        tempPort.setPortName(ports[i].portName());
        if(tempPort.open(QIODevice::ReadWrite)) {

            tempPort.close();
            ui->senderComboBox->addItem(ports[i].portName());
            ui->receiverComboBox->addItem(ports[i].portName());
        }
    }

    ui->senderComboBox->blockSignals(false);
    ui->receiverComboBox->blockSignals(false);
}

void MainWindow::handleRead(const QByteArray& data)
{
    ui->outputWindowLine->setText(data);
}

void MainWindow::handleStuff(const QByteArray &stuffedData)
{
    sendedAmount += stuffedData.size();

    QString temp = stuffedData.toHex().toUpper();
    for(int i = 2; i < temp.length(); i+=3)
        temp.insert(i, ' ');
    int tIndex = temp.indexOf("40 65");
    while(tIndex != -1) {
        temp.insert(tIndex, '\n');
        tIndex = temp.indexOf("40 65", tIndex + 5);
    }

    QString hexData = QString("Отправлено ") + QString::number(sendedAmount) + ((sendedAmount % 2 ==0 || sendedAmount % 4 == 0) ? QString(" байта.\n") : QString("байт.\n"))
                      + QString("Структура кадров после байт-стаффинга (в шестнадцатеричном виде):") + temp;
    ui->statusWindowLine->setPlainText(hexData);
    QTextCharFormat format;
    format.setForeground(Qt::red);
    QString sequence = "7D 5D 7D 5E";
    int index = hexData.indexOf(sequence);
    QTextCursor cursor(ui->statusWindowLine->document());

    while(index != -1) {

        cursor.setPosition(index);
        cursor.setPosition(index + 11, QTextCursor::KeepAnchor);

        cursor.mergeCharFormat(format);

        index = hexData.indexOf(sequence, index + 11);
    }

    sequence = "7D 5F";
    index = hexData.indexOf(sequence);

    while(index != -1) {

        cursor.setPosition(index);
        cursor.setPosition(index + 5, QTextCursor::KeepAnchor);

        cursor.mergeCharFormat(format);

        index = hexData.indexOf(sequence, index + 5);
    }
}

void MainWindow::handleSend()
{
    try {
        if(!sender->isOpen())
            QMessageBox::critical(this, "Ошибка при отправке данных", "Выберите COM порт");
        else if(ui->inputWindowLine->toPlainText() == "")
            QMessageBox::critical(this, "Ошибка при отправке данных", "Введите данные");
        else {
            sender->send(ui->inputWindowLine->toPlainText());
            ui->inputWindowLine->clear();
        }
    }
    catch(...) {
        QMessageBox::critical(this, "Ошибка при отправке данных", "Данные не были отправлены корректно");
    }
}

void MainWindow::onSenderComboBoxCurrentTextChanged(const QString &portName)
{
    if(sender != nullptr) {
        try {
            if(portName != "порт не задан")
                sender->changePort(portName);
            else
                sender->close();
        }
        catch(...) {

            QMessageBox::critical(this, "Ошибка", "Не удалось изменить COM порт.");
            ui->senderComboBox->setCurrentIndex(0);
        }

        checkSerialPorts();
    }
}

void MainWindow::onReceiverComboBoxCurrentTextChanged(const QString &portName)
{
    if(receiver != nullptr) {
        try {
            if(portName != "порт не задан")
                receiver->changePort(portName);
            else
                receiver->close();
        }
        catch(...) {

            QMessageBox::critical(this, "Ошибка", "Не удалось изменить COM порт.");
            ui->receiverComboBox->setCurrentIndex(0);
        }

        checkSerialPorts();
    }
}
