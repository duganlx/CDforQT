#include "widget.h"
#include "ui_widget.h"
#include <QTextCodec>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    server = new QTcpServer(this);
    ui->sendData->setEnabled(false);
    ui->journal->setEnabled(false);

    //准备题目数据
    ui->questionList->addItem(new QListWidgetItem(QIcon(":/question.ico"),tr("习题1")));
    ui->questionList->addItem(new QListWidgetItem(QIcon(":/question.ico"),tr("习题2")));
    ui->questionList->addItem(new QListWidgetItem(QIcon(":/question.ico"),tr("习题3")));
    ui->questionList->addItem(new QListWidgetItem(QIcon(":/question.ico"),tr("习题4")));

    questionList = preparedQuestion();

    ui->journal->append("初始化服务器...");
    for(int i=0; i<questionList.size(); i++)
    {
        targetQuestion = questionList[i];
        ui->journal->append("题目"+QString::number(i+1)+":"+targetQuestion->show());
    }

    //绑定槽函数
    connect(server, SIGNAL(newConnection()), this, SLOT(connectingClient()));

}

Widget::~Widget()
{
    delete ui;
}

QList<Question*> Widget::preparedQuestion()
{
    QList<Question*> list;
    Question *question;

    question = new Question("把作业空间中使用的逻辑地址变为内存中物理地址称为（）", "B", "加载", "重定向", "物理化", "逻辑化");
    list<<question;
    question = new Question("分区管理要求对每一个作业都分配（）的内存单元", "A", "地址连续", "若干地址不连续", "若干连续的块", "若干不连续的块");
    list<<question;
    question = new Question("在固定分区分配中，每个分区的大小是（）", "C", "相同", "随作业长度变化", "可以不同但预先固定", "可以不同但根据作业长度固定");
    list<<question;
    question = new Question("在可变式分区存储管理中的拼接技术可以（）", "A", "集中空闲分区", "增加内存容量", "缩短访问周期", "加速地址转换");
    list<<question;

    return list;
}

void Widget::connectingClient()
{
    client = server->nextPendingConnection(); //获取连接过来的客户端信息
    clients.append(client); //添加入客户端列表
    ui->journal->append("客户端("+client->peerAddress().toString().split("::ffff:")[1]+":"+QString::number(client->peerPort())+") 连接服务器");

    //绑定槽函数
    connect(client, SIGNAL(readyRead()), this, SLOT(receiveData()));
    connect(client, SIGNAL(disconnected()), this, SLOT(disconnectedClient()));
}

void Widget::disconnectedClient()
{
    //由于disconnected信号并未提供SocketDescriptor，所以需要遍历寻找
    for(int i=0; i<clients.length(); i++)
    {
        if(clients[i]->state() == QAbstractSocket::UnconnectedState)
        {
            //删除记录
            ui->journal->append("客户端("+clients[i]->peerAddress().toString().split("::ffff:")[1]+":"+QString::number(clients[i]->peerPort())+") 断开连接");
            clients[i]->destroyed();
            clients.removeAt(i);
        }
    }
}

void Widget::receiveData()
{
    qDebug()<<"receiveData()";

    for(int i=0; i<clients.length(); i++)
    {
        QByteArray buffer = clients[i]->readAll();
        if(buffer.isEmpty()) continue;

        QString data = QTextCodec::codecForName("UTF-8")->toUnicode(buffer);
        QStringList datas = data.split(",");

        QString message = tr("客户端(%1:%2):所选答案为：%3,%4").arg(clients[i]->peerAddress().toString().split("::ffff:")[1])
                                             .arg(clients[i]->peerPort()).arg(datas.at(0)).arg(datas.at(1));
        ui->journal->append(message);
    }
}

void Widget::on_run_clicked()
{
    if(ui->run->text()=="监听")
    {
        QString ipAddr = ui->ipAddr->text();
        quint16 port = ui->port->text().toUShort();

        bool ok = server->listen(QHostAddress::Any, port);
        if(ok)
        {
            QMessageBox::information(NULL, "提示", "已监听", QMessageBox::Ok);
            ui->journal->append("服务器监听成功(ip:"+ipAddr+",port:"+QString::number(port)+")");
            ui->run->setText("断开");
            ui->sendData->setEnabled(true);
        }
        else
        {
            QMessageBox::critical(NULL, "提示", "监听失败", QMessageBox::Ok);
        }
    }
    else
    {
        for(int i=0; i<clients.length(); i++) //断开所有连接
        {
            clients[i]->disconnectFromHost();
            bool ok = clients[i]->waitForDisconnected(1000);
            if(!ok)
            {
                qDebug()<<"客户端"<<i<<"断开异常";
            }
            clients.removeAt(i);
        }
        QMessageBox::information(NULL, "提示", "已断开", QMessageBox::Ok);
        server->close();
        ui->run->setText("监听");
        ui->sendData->setEnabled(false);
    }
}

void Widget::on_sendData_clicked()
{
    qDebug()<<"sendData(),selected:"<<ui->questionList->currentRow();

    int questionId = ui->questionList->currentRow();

    if(questionId>=0&&questionId<=3)
    {
        targetQuestion = questionList[questionId];
        QMessageBox::information(NULL, "提示", "发送题目成功", QMessageBox::Ok);
    }
    else
    {
        QMessageBox::information(NULL, "提示", "请选择题目", QMessageBox::Ok);
    }

    qDebug()<<"question data: "+targetQuestion->getStr();

    for(int i=0; i<clients.length(); i++)
    {
        ui->journal->append("客户端("+clients[i]->peerAddress().toString()+":"+QString::number(clients[i]->peerPort())+")发送习题"+
                            QString::number(questionId));
        clients[i]->write(targetQuestion->getStr().toUtf8());
    }

}
