#ifndef CLICKEDLABEL_H
#define CLICKEDLABEL_H

#include <QLabel>
#include "global.h"

class ClickedLabel : public QLabel
{
    //用信号和槽机制，添加Q_OBJECT
    Q_OBJECT
public:
    ClickedLabel(QWidget *parent=nullptr);
    //因为基类是虚函数，所以这里不加virtual也是虚函数，override告诉编译器重写基类虚函数
    virtual void enterEvent(QEnterEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *ev) override;
    virtual void mouseReleaseEvent(QMouseEvent *ev) override;
    void SetState(QString normal = "", QString hover = "", QString press = "",
                  QString select = "", QString select_hover = "", QString select_press = "");
    ClickLbState GetCurState();//获取点击状态
    bool SetCurState(ClickLbState state);
    void ResetNormalState();

private:
    //当Label处于普通状态，被点击后，切换为选中状态，再次点击又切换为普通状态
    QString _normal;//普通状态
    QString _normal_hover;//普通的悬浮状态
    QString _normal_press;//普通的点击状态

    QString _selected;//选中状态
    QString _selected_hover;//选中的悬浮状态
    QString _selected_press;//选中的点击状态
    ClickLbState _curstate;

signals:
    void clicked(QString, ClickLbState);
};

#endif // CLICKEDLABEL_H
