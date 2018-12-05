#ifndef NODE_H
#define NODE_H

#include "itemparent.h"
#include "edge.h"

#include <QGraphicsItem>
#include <QList>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

class Edge;

class Node : public ItemParent
{
public:
    Node();
    ~Node() override;
    void addEdge(Edge *edge);
    QRectF boundingRect() const override;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget *) override;
    void setTextContent(QString text); // !!!
    QPainterPath shape() const override;
    QList<Edge *> edges() const;
    void removeEdge(Edge *edge);

    enum { Type = ItemParent::Type + 1 };
    int type() const override { return Type; }
    enum { Radius = 17 };

    QString texttoint;
    QString textContent() const;
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
private:
    QList<Edge *> edgeList;
};

#endif // NODE_H
