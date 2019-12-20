#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    QPalette palette;
    palette.setColor(QPalette::Background, QColor(238, 230, 133));
    ui->content->setAutoFillBackground(true);
    ui->content->setPalette(palette);

    client = new QTcpSocket(this);  //实例化
    client->abort();    //取消所有连接

    //初始化连接按钮与断开按钮状态
    ui->connectServer->setEnabled(true);
    ui->disconnectServer->setEnabled(false);


}

void Widget::receiveData()
{
    QByteArray buffer = client->readAll();
    if(!buffer.isEmpty())
    {
        //ui->content->setText(buffer);
        qDebug()<<buffer;
    }
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_connectServer_clicked()
{
    QString ipAddr = ui->ipAddr->text();
    quint16 port = ui->port->text().toUShort();
    client->connectToHost(ipAddr, port);

    qDebug()<<"connecting server...";

    if(client->waitForConnected(1000))
    {
        qDebug()<<"connect server success";
        QMessageBox::information(NULL, "提示", "连接成功", QMessageBox::Ok);
        ui->disconnectServer->setEnabled(true);
        ui->connectServer->setEnabled(false);
    }
    else
    {
        QMessageBox::critical(NULL, "提示", "连接失败，请检查ip地址和端口号", QMessageBox::Ok);
    }
}

void Widget::on_disconnectServer_clicked()
{
    client->disconnectFromHost();

    qDebug()<<"disconnecting server...";

    if(client->state() == QAbstractSocket::UnconnectedState
            || client->waitForDisconnected(1000))
    {
        qDebug()<<"disconnect server success";
        QMessageBox::information(NULL, "提示", "断开成功", QMessageBox::Ok);
        ui->disconnectServer->setEnabled(false);
        ui->connectServer->setEnabled(true);
    }
    else
    {
        QMessageBox::critical(NULL, "提示", "断开失败，请重试", QMessageBox::Ok);
    }
}


void Widget::on_commit_clicked()
{
    qDebug()<<"commit data";
}
