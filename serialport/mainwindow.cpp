#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    int ret=0;
    timer=new QTimer();

    ui->setupUi(this);
    ret=SerialPortInit();

    timer->setInterval(100);

    connect(serialport,SIGNAL(readyRead()),this,SLOT(ReadSerialPortData()));
    connect(timer,SIGNAL(timeout()),this,SLOT(TimerFunc()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//初始化串口
int MainWindow::SerialPortInit(void)
{
    int ret=0;
    int time=10;

    serialport=new QSerialPort(this);

    serialport->setPortName(QString("COM6"));
    serialport->setBaudRate(QSerialPort::Baud9600);
    serialport->setDataBits(QSerialPort::Data8);//8位数据位
    serialport->setParity(QSerialPort::NoParity );//无校验
    serialport->setStopBits(QSerialPort::OneStop );//1个停止位
    serialport->setFlowControl(QSerialPort::NoFlowControl);//无流控制
    do                                                     //打开串口
    {
        ret=serialport->open(QIODevice::ReadWrite);
    }
    while (time--);

    if(ret!=0)
        goto SERIAL_PORT_ERR;

    this->timer->start();

   // this->ui->comboBox_2->addItem("");
#ifdef DEBUG
    qDebug("serial_port open OK");
#endif
    return 0;
SERIAL_PORT_ERR:

#ifdef DEBUG
    qDebug("serial_port open FAIL");
#endif
    return -1;
}

//读取串口数据
int MainWindow::ReadSerialPortData(void)
{
    int ret=0;
    QByteArray readba;
    QByteArray tempba;
    QString str;
    tempba.clear();
    while(1)
    {
        tempba=serialport->readAll();
        if(tempba.isEmpty())
            break;
        readba.append(tempba);
    }

    str.append(readba);
    qDebug(readba.toHex());
    ShowSerialPortData(str,0);
    return ret;
}

//发送串口数据
int MainWindow::WriteSerialPortData(QByteArray sendbuf)
{
    int ret=0;
    ret=serialport->write(sendbuf);
    this->ui->comboBox_2->addItem("0000");
    return ret;
}
//串口发送数据按钮
void MainWindow::on_sendButton_clicked()
{
    QString send_str=this->ui->SendTextEdit->toPlainText();
    WriteSerialPortData(send_str.toLatin1());
    qDebug("write");
}
//清除接收窗口内容
void MainWindow::on_pushButton_clicked()
{
    this->ui->textBrowser->clear();
}
//显示串口信息
int MainWindow::ShowSerialPortData(QString str, int mode)
{
    int ret=0;
    static QString strbuf;

    switch (mode)
    {
        case 0:
             this->ui->textBrowser->append(str);
            break;
        case 1:

            break;
        default:
            break;
    }

    return ret;
}


//刷新按钮
void MainWindow::on_flushButton_clicked()
{
   UpdateSerialPort();
}
//更新串口列表
int MainWindow::UpdateSerialPort(void)
{
    /* 查找可用串口 */
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        info.isNull();
        /* 判断端口是否能打开 */
        if(serial.open(QIODevice::ReadWrite))
        {
            int isHaveItemInList = 0;
            /* 判断是不是已经在列表中了 */
            for(int i=0; i<ui->serial_name_Box->count(); i++)
            {
                /* 如果，已经在列表中了，那么不添加这一项了就 */
                if(ui->serial_name_Box->itemText(i) == serial.portName())
                {
                    isHaveItemInList=1;
                }
            }

            if(isHaveItemInList == 0)
            {
                 ui->serial_name_Box->addItem(serial.portName());
            }
            serial.close();
        }
    }
}

//定时器
void MainWindow::TimerFunc(void)
{
    UpdateSerialPort();
}














