#ifndef EDGE_H
#define EDGE_H

#include "itemparent.h"
#include "node.h"
#include <QStyleOptionGraphicsItem>
#include <cmath>

class Node;

class Edge : public ItemParent
{
public:
    Edge(Node * beginNode, Node * endNode,bool ch,QString textArrow);
    ~Edge() override;

    enum { Type = ItemParent::Type + 2 };

    const uint id;
    Node * begin, *end;
    int type() const override { return Type; }

    Node *beginNode() const;
    Node *endNode() const;
    void adjust();
    QString textEdge;
protected:
    QPainterPath shape() const override;
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
private:
    static uint _idStatic;
    static int _offsAngle;
    QPointF beginPoint;
    QPointF endPoint;
    QPointF textPoint;
    qreal arrowSize;
    bool isDirected;
};

#endif // EDGE_H
