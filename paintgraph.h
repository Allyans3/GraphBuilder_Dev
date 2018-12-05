#ifndef PAINTGRAPH_H
#define PAINTGRAPH_H
#include <QColor>
#include "edge.h"
#include "node.h"
#include <QRandomGenerator>
#include <QDebug>

bool compare(const QPair<Node*,int> &v1, const QPair<Node*,int> &v2);
QList<QPair<Node*, int>> node_degres(QList<Node*> nodes);
bool adj(Node * check, QList<Node *> node);
QList<QPair<Node*, QColor>> paint_graph(QList<Node*> nodes);
bool adj_edg(QList<Edge*> edges, Edge* check);
QList<QPair<Edge*, QColor>> paint_edges(QList<Edge*> edges);

#endif // PAINTGRAPH_H
