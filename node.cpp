#include "node.h"

Node::Node() : ItemParent()
{
    setFlag(ItemIsSelectable);
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
}

Node::~Node()
{
    foreach (Edge *edge, edgeList)
    {
        delete edge;
    }
//    edgeList.clear(); //!!!
}

void Node::addEdge(Edge *edge)
{
    edgeList << edge;
}

QList<Edge *> Node::edges() const
{
    return edgeList;
}

void Node::setTextContent(QString text)
{
    texttoint = text;
    update();
}

QString Node::textContent() const
{
    return texttoint;
}

QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(-Radius,
                    -Radius,
                    Radius*2,
                    Radius*2);
    return path;
}

QRectF Node::boundingRect() const
{
    return shape().boundingRect();
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setBrush((option->state & QStyle::State_Selected ? Qt::yellow: this->brush()));
    painter->setPen(QPen(QColor(46,53,61)));
    painter->drawEllipse(-Radius,-Radius, Radius*2, Radius*2);
    painter->setFont(QFont("Times", 13, QFont::Bold));
    painter->drawText(boundingRect(), Qt::AlignCenter,textContent());
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        foreach (Edge *edge, edgeList)
            edge->adjust();
        break;
    default:
        break;
    };
    return QGraphicsItem::itemChange(change, value);
}

void Node::removeEdge(Edge *edge)
{
    QMutableListIterator<Edge *> iter(edgeList);
    while (iter.hasNext())
    {
        Edge *e = iter.next();
        if (e == edge)
        {
            iter.remove();
            break;
        }
    }
}
