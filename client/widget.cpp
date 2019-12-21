#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QTextCodec>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    QPalette palette;
    palette.setColor(QPalette::Background, QColor(238, 230, 133));
    ui->content->setAutoFillBackground(true);
    ui->content->setPalette(palette);

    //初始化选项编号
    optionGroup = new QButtonGroup(this);
    optionGroup->addButton(ui->buttonA, 0);
    optionGroup->addButton(ui->buttonB, 1);
    optionGroup->addButton(ui->buttonC, 2);
    optionGroup->addButton(ui->buttonD, 3);

    //初始化连接按钮与断开按钮状态
    ui->connectServer->setEnabled(true);
    ui->disconnectServer->setEnabled(false);

    client = new QTcpSocket(this);  //实例化
    client->abort();    //取消所有连接

    //绑定槽函数
    connect(client, SIGNAL(readyRead()), this, SLOT(receiveData()));
}

void Widget::receiveData()
{
    qDebug()<<"receiveData()";
    QByteArray buffer = client->readAll();
    if(!buffer.isEmpty())
    {
        //ui->content->setText(buffer);
        data = QTextCodec::codecForName("UTF-8")->toUnicode(buffer);
        qDebug()<<data;
    }
    QStringList datas = data.split(",");

    //填充数据到对应控件
    ui->content->setText(datas.at(0));
    ui->buttonA->setText("选项A "+ datas.at(2));
    ui->buttonB->setText("选项B "+ datas.at(3));
    ui->buttonC->setText("选项C "+ datas.at(4));
    ui->buttonD->setText("选项D "+ datas.at(5));
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
    //对应关系：A-0 B-1 C-2 D-3
    QString clientChoice;

    switch (optionGroup->checkedId())
    {
    case 0:
        clientChoice = "A";
        break;
    case 1:
        clientChoice = "B";
        break;
    case 2:
        clientChoice = "C";
        break;
    case 3:
        clientChoice = "D";
        break;
    }

    if(clientChoice == data.split(",")[1])
    {
        clientChoice.append(",答案正确");
        QMessageBox::information(NULL, "提示", "回答正确(●'◡'●)", QMessageBox::Ok);
    }
    else
    {
        clientChoice.append(",答案错误");
        QMessageBox::information(NULL, "提示", "回答错误( ▼-▼ )", QMessageBox::Ok);
    }

    qDebug()<<"commit data to server: "<<clientChoice.toUtf8();

    client->write(clientChoice.toUtf8());

}
