#ifndef PAINTSCENE_H
#define PAINTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

#include "edge.h"
#include "node.h"

class PaintScene : public QGraphicsScene
{
public:
    explicit PaintScene(QObject *parent = nullptr);
    ~PaintScene();

    QList<Node*> nodes;
    QList<Edge*> edges;
    QList<int> nums;

    bool v = 0;

private:
    Node* item;

private:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
protected:
//    virtual void wheelEvent(QWheelEvent* event);

};

#endif // PAINTSCENE_H
