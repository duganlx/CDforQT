#include "question.h"
#include <QtDebug>

Question::Question(QString content, QString answer, QString A, QString B, QString C, QString D)
    :content(content), answer(answer), optionA(A), optionB(B), optionC(C), optionD(D)
{

}

QString Question::show()
{
    return "内容:"+content+",答案:"+answer+",选项A:"+optionA+",选项B:"+optionB+",选项C:"+optionC+",选项D:"+optionD;
}

QString Question::getContent()
{
    return content;
}

QString Question::getAnswer()
{
    return answer;
}

QString Question::getA()
{
    return optionA;
}

QString Question::getB()
{
    return optionB;
}

QString Question::getC()
{
    return optionC;
}

QString Question::getD()
{
    return optionD;
}

QString Question::getStr()
{
    return content+","+answer+","+optionA+","+optionB+","+optionC+","+optionD;
}
