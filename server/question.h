#ifndef QUESTION_H
#define QUESTION_H

#include <QObject>

class Question : public QObject
{
    Q_OBJECT
public:
    Question(QString content, QString answer, QString A, QString B, QString C, QString D);
    QString show();
    QString getContent();
    QString getAnswer();
    QString getA();
    QString getB();
    QString getC();
    QString getD();
    QString getStr(); //获取对象的字符串形式

signals:

public slots:

private:
    QString content;
    QString answer;
    QString optionA;
    QString optionB;
    QString optionC;
    QString optionD;
};

#endif // QUESTION_H
