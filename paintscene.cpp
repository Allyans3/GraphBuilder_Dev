#include "paintscene.h"
#include <QDebug>

PaintScene::PaintScene(QObject *parent) : QGraphicsScene(parent)
{

}

PaintScene::~PaintScene()
{

}

void PaintScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem * itemm = itemAt(event->scenePos(),QTransform());

    if (!itemm)
    {
        this->clearSelection();
        int next = 1;
        if(v == 1)
        {
            for(int i = 0; i<nums.size();i++)
            {
                if(nums.at(i) == next)
                    next++;
                else
                    break;
            }
            nums.append(next);
            std::sort(nums.begin(),nums.end());
            QString num = QString("X") + QString::number(next);
            item = new Node;
            item->setTextContent(num);
            item->setPos(event->scenePos().x(),event->scenePos().y());
            item->setBrush(Qt::red);
            addItem(item);
            next++;
            nodes.append(item);
        }
    }
    else
    {
        QGraphicsScene::mousePressEvent(event);
    }
}
