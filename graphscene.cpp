#include "graphscene.h"
#include <QDebug>

GraphScene::GraphScene(QWidget *parent)
    : QGraphicsView(parent)
{
    setRenderHint(QPainter::Antialiasing);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    setCacheMode(QGraphicsView::CacheBackground);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setDragMode(QGraphicsView::ScrollHandDrag);
    scale(qreal(1), qreal(1));
}

void GraphScene::keyPressEvent(QKeyEvent *event)
{
    Node *selectedItem = nullptr;
    if (scene()->selectedItems().size() == 1) {
        selectedItem = dynamic_cast<Node*> (scene()->selectedItems().at(0));
    }
    switch (event->key()) {
    case Qt::Key_Up:
        if (selectedItem)
            selectedItem->moveBy(0, -20);
        break;
    case Qt::Key_Down:
        if (selectedItem)
            selectedItem->moveBy(0, 20);
        break;
    case Qt::Key_Left:
        if (selectedItem)
            selectedItem->moveBy(-20, 0);
        break;
    case Qt::Key_Right:
        if (selectedItem)
            selectedItem->moveBy(20, 0);
        break;
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    case Qt::Key_Space:
        shuffle();
        break;
    default:
        break;
    }
    QGraphicsView::keyPressEvent(event);
}

#if QT_CONFIG(wheelevent)
void GraphScene::wheelEvent(QWheelEvent *event)
{
    scaleView(pow(double(2), -event->delta() / 240.0));
}
#endif

void GraphScene::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.5 || factor > 8)
        return;

    scale(scaleFactor, scaleFactor);
}

void GraphScene::shuffle()
{
    foreach (QGraphicsItem *item, scene()->items()) {
        if (qgraphicsitem_cast<Node *>(item))
            item->setPos(17 + QRandomGenerator::global()->bounded(this->width()-25), 17 + QRandomGenerator::global()->bounded(this->height()-25));
    }
}

void GraphScene::zoomIn()
{
    scaleView(qreal(1.2));
}

void GraphScene::zoomOut()
{
    scaleView(1 / qreal(1.2));
}
