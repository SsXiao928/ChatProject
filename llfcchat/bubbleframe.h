#ifndef BUBBLEFRAME_H
#define BUBBLEFRAME_H

#include<QFrame>
#include"global.h"
#include <QHBoxLayout>

class BubbleFrame : public QFrame
{
    Q_OBJECT
public:
    //ChatRole role是自己的对话框还是别人的
    BubbleFrame(ChatRole role, QWidget *parent = nullptr);
    void setMargin(int margin);
    //inline int margin(){return margin;}
    void setWidget(QWidget *w);
protected:
    //绘制气泡
    void paintEvent(QPaintEvent *e);
private:
    //水平的布局
    QHBoxLayout *m_pHLayout;
    ChatRole m_role;
    int      m_margin;
};

#endif // BUBBLEFRAME_H
