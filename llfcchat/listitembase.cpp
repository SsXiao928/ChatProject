#include "listitembase.h"
#include <QStyleOption>
#include <QPainter>

ListItemBase::ListItemBase(QWidget *parent) : QWidget(parent)
{

}

void ListItemBase::SetItemType(ListItemType itemType)
{
    _itemType = itemType;
}

ListItemType ListItemBase::GetItemType()
{
    return _itemType;
}

void ListItemBase::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;//样式选项
    opt.initFrom(this);//样式选项绑定本窗口
    QPainter p(this);//
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
