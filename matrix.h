#ifndef MATRIX_H
#define MATRIX_H
#include <node.h>
#include <edge.h>

#include <QList>
#include <QPair>
#include <QString>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <vector>
#include <stack>
#include <QListWidget>

QList<QPair<QString, QString>> serialize(QList<Edge*> edges);
void init_matrixes(QTableWidget *&fwidget, QTableWidget *&swidget, int nodes, int edges);
void get_matrix_adj(QList<Node *> nodes, QList<Edge *> edges, QTableWidget *&widget, bool isdirected);
void get_matrix_inc(QList<Node*> nodes, QList<Edge*> edges, QTableWidget *&widget, bool isdirected);
void from_adj_to_inc(QTableWidget *fwidget, QTableWidget *&swidget, QList<Node*> nodes, QList<Edge*> edges, bool isdirected);
void from_inc_to_adj(QTableWidget *fwidget, QTableWidget *&swidget, QList<Node*> nodes, QList<Edge*> edges, bool isdirected);

#endif // MATRIX_H
