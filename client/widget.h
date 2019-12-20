#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMessageBox>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    QTcpSocket *client;

private slots:
    void receiveData(); //接收数据

    void on_connectServer_clicked();
    void on_disconnectServer_clicked();
    void on_commit_clicked();
};

#endif // WIDGET_H
