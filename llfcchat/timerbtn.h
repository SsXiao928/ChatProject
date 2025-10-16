#ifndef TIMERBTN_H
#define TIMERBTN_H
#include <QPushButton>
#include <QTimer>

class TimerBtn: public QPushButton
{
public:
    TimerBtn(QWidget *parent = nullptr);
    ~TimerBtn();
    //指示编译器当前的方法是对基类虚函数的重写（覆盖）
    //确保方法正确重写基类虚函数
    void mouseReleaseEvent(QMouseEvent* e) override;
private:
    QTimer *_timer;//定时器
    int _counter;//计数器
};

#endif // TIMERBTN_H
