#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>
#include <QMessageBox>
#include <QListWidget>
#include <question.h>

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
    QTcpServer *server;
    QList<QTcpSocket*> clients;
    QList<Question*> questionList;
    Question *targetQuestion;

private slots:
    void receiveData();
    QList<Question*> preparedQuestion();

    void on_run_clicked();
    void on_sendData_clicked();
};

#endif // WIDGET_H
