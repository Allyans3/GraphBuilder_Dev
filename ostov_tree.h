#ifndef OSTOV_TREE_H
#define OSTOV_TREE_H
#include <QListWidget>
#include "edge.h"


void del_isolated(QList<Edge *> &edges, QList<Node *> nodes);
bool compare(const Edge* v1, const Edge* v2);
Edge * nearest_path(QList<Edge *> edges, QList<Node *> temp, QList<Node *> nodes);
QList<Edge*> ostov_tree(QList<Node*> nodes, QList<Edge*> edges);

#endif // OSTOV_TREE_H
