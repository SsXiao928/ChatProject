#include "chatitembase.h"


ChatItemBase::ChatItemBase(ChatRole role, QWidget *parent)
    : QWidget(parent)
    , m_role(role)
{
    m_pNameLabel    = new QLabel();
    m_pNameLabel->setObjectName("chat_user_name");
    QFont font("Microsoft YaHei");
    font.setPointSize(9);//字体大小
    m_pNameLabel->setFont(font);//设置label的字体
    m_pNameLabel->setFixedHeight(20);//设置高度
    //图标
    m_pIconLabel    = new QLabel();
    m_pIconLabel->setScaledContents(true);
    m_pIconLabel->setFixedSize(42, 42);

    //聊天气泡
    m_pBubble       = new QWidget();
    //网格布局
    QGridLayout *pGLayout = new QGridLayout();
    pGLayout->setVerticalSpacing(3);    //垂直间距
    pGLayout->setHorizontalSpacing(3);//水平间距
    pGLayout->setContentsMargins(3, 3, 3, 3);//组件和边界的间距

    //QSizePolicy::Expanding 可扩充 QSizePolicy::Minimum 最小高40 宽20
    QSpacerItem*pSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    if(m_role == ChatRole::Self)
    {
        //自己发的消息
        m_pNameLabel->setContentsMargins(0,0,8,0);//左上右下的距离
        m_pNameLabel->setAlignment(Qt::AlignRight);//右对齐
        pGLayout->addWidget(m_pNameLabel, 0,1, 1,1);//第零行第一列、占一行占一列
        pGLayout->addWidget(m_pIconLabel, 0, 2, 2,1, Qt::AlignTop);//第零行第二列，占两行一列， 顶部对齐
        pGLayout->addItem(pSpacer, 1, 0, 1, 1);//
        pGLayout->addWidget(m_pBubble, 1,1, 1,1);
        //设置比例
        pGLayout->setColumnStretch(0, 2);//第零列占的比例是2
        pGLayout->setColumnStretch(1, 3);//第一列占的比例是3
    }else{
        //其他用户发的消息
        m_pNameLabel->setContentsMargins(8,0,0,0);
        m_pNameLabel->setAlignment(Qt::AlignLeft);
        pGLayout->addWidget(m_pIconLabel, 0, 0, 2,1, Qt::AlignTop);
        pGLayout->addWidget(m_pNameLabel, 0,1, 1,1);
        pGLayout->addWidget(m_pBubble, 1,1, 1,1);
        pGLayout->addItem(pSpacer, 2, 2, 1, 1);
        pGLayout->setColumnStretch(1, 3);
        pGLayout->setColumnStretch(2, 2);
    }
    this->setLayout(pGLayout);
}

void ChatItemBase::setUserName(const QString &name)
{
    m_pNameLabel->setText(name);
}

void ChatItemBase::setUserIcon(const QPixmap &icon)
{
    m_pIconLabel->setPixmap(icon);
}

//定制化实现气泡widget
//设置气泡对话框
void ChatItemBase::setWidget(QWidget *w)
{
    QGridLayout *pGLayout = (qobject_cast<QGridLayout *>)(this->layout());
    pGLayout->replaceWidget(m_pBubble, w);
    delete m_pBubble;
    m_pBubble = w;
}
