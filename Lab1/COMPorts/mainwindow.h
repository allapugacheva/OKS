#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStringList>
#include <QTimer>

#include <QMainWindow>
#include "compair.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    COMPair* ComPair;

    int receivedAmount;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    void checkSerialPorts();

private slots:
    void handleRead();
    void on_pushButton_clicked();
    void on_comboBox_2_currentTextChanged(const QString &arg1);
    void on_comboBox_currentTextChanged(const QString &arg1);
};
#endif // MAINWINDOW_H
