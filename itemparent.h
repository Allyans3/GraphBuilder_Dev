#ifndef ITEMPARENT_H
#define ITEMPARENT_H
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

class ItemParent : public QAbstractGraphicsShapeItem
{
public:
    ItemParent();
    enum { Type = UserType + 1 };
private:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // ITEMPARENT_H
