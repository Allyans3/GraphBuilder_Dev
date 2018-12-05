#include "itemparent.h"
#include <QDebug>

ItemParent::ItemParent() : QAbstractGraphicsShapeItem()
{

}

void ItemParent::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        qDebug() << "RightButton";

        scene()->clearSelection();
        setSelected(true);
    }
    QGraphicsItem::mousePressEvent(event);
}
