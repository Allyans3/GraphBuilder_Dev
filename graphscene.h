#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H
#include <QGraphicsView>
#include <QGraphicsScene>
#include "node.h"
#include <QRandomGenerator>

class GraphScene : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphScene(QWidget * parent = nullptr);
public slots:
    void shuffle();
    void zoomIn();
    void zoomOut();
protected:
    void keyPressEvent(QKeyEvent *event) override;
#if QT_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent *event) override;
#endif
    void scaleView(qreal scaleFactor);
};

#endif // GRAPHSCENE_H
