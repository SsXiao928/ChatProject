#include "chatview.h"
#include <QScrollBar>
#include <QEvent>
#include <QStyleOption>
#include <QPainter>

ChatView::ChatView(QWidget *parent)  : QWidget(parent)
    , isAppended(false)
{
    //设置垂直布局
    QVBoxLayout *pMainLayout = new QVBoxLayout();
    this->setLayout(pMainLayout);
    // 设置所有边的内边距为 0
    pMainLayout->setContentsMargins(0, 0, 0, 0);

    //创建滚动区域
    m_pScrollArea = new QScrollArea();
    m_pScrollArea->setObjectName("chat_area");//设置名字，可以写qss
    pMainLayout->addWidget(m_pScrollArea);

    //
    QWidget *w = new QWidget(this);//父窗口是ChatView
    w->setObjectName("chat_bg");
    w->setAutoFillBackground(true);//充满所在区域

    //子布局
    QVBoxLayout *pVLayout_1 = new QVBoxLayout();//垂直布局
    pVLayout_1->addWidget(new QWidget(), 100000);
    w->setLayout(pVLayout_1);//w设置子布局
    m_pScrollArea->setWidget(w);//滚动区域中设置w，由于w设置了自动填充setAutoFillBackground(true)，会将m_pScrollArea填满

    //关闭垂直滚动条
    m_pScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //获取垂直滚动条（关掉了，但可以改一下布局）
    QScrollBar *pVScrollBar = m_pScrollArea->verticalScrollBar();
    //滚动条的范围变化会触发onVScrollBarMoved槽函数
    connect(pVScrollBar, &QScrollBar::rangeChanged,this, &ChatView::onVScrollBarMoved);

    //把垂直ScrollBar放到上边 而不是原来的并排
    QHBoxLayout *pHLayout_2 = new QHBoxLayout();
    pHLayout_2->addWidget(pVScrollBar, 0, Qt::AlignRight);
    pHLayout_2->setContentsMargins(0, 0, 0, 0);
    m_pScrollArea->setLayout(pHLayout_2);
    pVScrollBar->setHidden(true);//隐藏滚动条

    m_pScrollArea->setWidgetResizable(true);//允许内部的部件自动设置大小
    m_pScrollArea->installEventFilter(this);//安装事件过滤器
    initStyleSheet();
}

void ChatView::appendChatItem(QWidget *item)
{
    QVBoxLayout *vl = qobject_cast<QVBoxLayout *>(m_pScrollArea->widget()->layout());
    vl->insertWidget(vl->count()-1, item);//vl->count()-1  插入位置
    isAppended = true;
}

void ChatView::prependChatItem(QWidget *item)
{

}

void ChatView::insertChatItem(QWidget *before, QWidget *item)
{

}
bool ChatView::eventFilter(QObject *o, QEvent *e)
{
    /*if(e->type() == QEvent::Resize && o == )
    {

    }
    else */if(e->type() == QEvent::Enter && o == m_pScrollArea)
    {
        //滚动条大小为零，隐藏
        m_pScrollArea->verticalScrollBar()->setHidden(m_pScrollArea->verticalScrollBar()->maximum() == 0);
    }
    else if(e->type() == QEvent::Leave && o == m_pScrollArea)
    {
        //离开时隐藏滚动条
        m_pScrollArea->verticalScrollBar()->setHidden(true);
    }
    return QWidget::eventFilter(o, e);
}

//重写paintEvent支持子类绘制
void ChatView::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

//监听滚动区域变化的槽函数
void ChatView::onVScrollBarMoved(int min, int max)
{
    if(isAppended) //添加item可能调用多次
    {
        QScrollBar *pVScrollBar = m_pScrollArea->verticalScrollBar();//获取滚动条
        pVScrollBar->setSliderPosition(pVScrollBar->maximum());//设置滚动条的位置
        //500毫秒内可能调用多次
        QTimer::singleShot(500, [this]()
        {
           isAppended = false;
        });
    }
}

void ChatView::initStyleSheet()
{

}
