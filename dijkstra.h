#ifndef DIJKSTRA_H
#define DIJKSTRA_H
#include "edge.h"
#include <QTableWidget>
#include <QListWidget>

int find_min_weight(int col, QTableWidget *matrix);
int find_min(QTableWidget * matrix, int row);
void dijkstra_steps(QListWidget *list, QTableWidget *matrix, int row, QList<Node*> nodes_copy, QList<Node *> visited, int min_col, int const_label);
void find_dijkstra(QList<Node*> nodes, QTableWidget *matrix, QListWidget *list);

#endif // DIJKSTRA_H
