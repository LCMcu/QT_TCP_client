#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <qserialportinfo.h>
#include <QTimer>
#define DEBUG   1

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int SerialPortInit(void);
    int WriteSerialPortData(QByteArray sendbuf);
    int ShowSerialPortData(QString str, int mode);
    int UpdateSerialPort(void);
    QString rcv_str;
private:
    Ui::MainWindow *ui;
    QSerialPort *serialport;
    QTimer *timer;
public slots:
    //int ReadSerialPortData(char *readbuf, int len);
    int ReadSerialPortData();

private slots:
    void on_sendButton_clicked();
    void clear_pushButton_clicked();
    void on_pushButton_clicked();
    void on_flushButton_clicked();
    void TimerFunc(void);
};

#endif // MAINWINDOW_H
