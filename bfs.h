#ifndef BFS_H
#define BFS_H
#include <QSet>
#include <QDebug>
#include "edge.h"
#include <QListWidget>

void update_temp(QList<Node*> &temp, QList<Node*> &visited, int &lenght,QListWidget *&column);
void bfs(QList<Node*> nodes, QListWidget *column);

Node * update_dfs(QList<Node*> &visited,QList<Node*> &nodes);
void dfs(QList<Node*> nodes, QListWidget *column);

#endif // BFS_H
