#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QResizeEvent>

#include "paintscene.h"
#include "matrix.h"
#include "paintgraph.h"
#include "eulerian_path.h"
#include "ostov_tree.h"
#include "bfs.h"
#include "graphscene.h"
#include "dijkstra.h"
#include <QGraphicsColorizeEffect>

namespace Ui {
class MainWindow;
}

class GraphScene;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void but_node_change();
public slots:
    void dijkstra_func();
    void addedge();
    void deleteitem();
    void repaintedge();
    void build_matrixes();
    bool graph_direction();
    bool isDuplicateEdge(Edge * edge);
    void clear();
    void editing();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void checkResizableField(QMouseEvent *event);

private:
    Ui::MainWindow *ui;
    PaintScene *scene;
    QTimer *timer;
    bool ch = 0;
    Edge* rebro = nullptr;
    Node* begin = nullptr,*end = nullptr;
    QPoint mpos = QPoint(-1, -1);
    QPoint respos = QPoint(-1,-1);
    QPoint initpos = QPoint(-1,-1);
    int numberr = 0;

private:
    void resizeEvent(QResizeEvent * event);

private slots:
    void slotTimer();

protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // MAINWINDOW_H
