#ifndef LISTITEMBASE_H
#define LISTITEMBASE_H
#include <QWidget>
#include "global.h"

class ListItemBase : public QWidget
{
    Q_OBJECT
public:
    //explicit表示够早时必须显示构造不能默认构造
    explicit ListItemBase(QWidget *parent = nullptr);
    void SetItemType(ListItemType itemType);

    ListItemType GetItemType();

protected:

private:
    ListItemType _itemType;

public slots:
    virtual void paintEvent(QPaintEvent* event) override;

signals:


};

#endif // LISTITEMBASE_H
