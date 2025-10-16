#ifndef STATEWIDGET_H
#define STATEWIDGET_H
/*实现侧边栏按钮功能，希望点击一个按钮，清空其他按钮的选中状态。而我们又希望按钮上面能在有新的通知的时候出现红点的图标*/

#include <QWidget>
#include "global.h"
#include <QLabel>

class StateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StateWidget(QWidget *parent = nullptr);

    void SetState(QString normal="", QString hover="", QString press="",
                  QString select="", QString select_hover="", QString select_press="");

    ClickLbState GetCurState();
    void ClearState();

    void SetSelected(bool bselected);//设置被选中
    void AddRedPoint();//加上红点
    void ShowRedPoint(bool show=true);//展示红点

protected:
    void paintEvent(QPaintEvent* event);

    virtual void mousePressEvent(QMouseEvent *ev) override;
    virtual void mouseReleaseEvent(QMouseEvent *ev) override;
    virtual void enterEvent(QEnterEvent *event) override;
    virtual void leaveEvent(QEvent* event) override;

private:

    QString _normal;
    QString _normal_hover;
    QString _normal_press;

    QString _selected;
    QString _selected_hover;
    QString _selected_press;

    ClickLbState _curstate;
    QLabel * _red_point;

signals:
    void clicked(void);

signals:

public slots:
};
#endif // STATEWIDGET_H
