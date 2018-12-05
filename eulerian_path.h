#ifndef EULERIAN_PATH_H
#define EULERIAN_PATH_H

#include <QTableWidget>
#include <vector>
#include <stack>
#include <QListWidget>
#include <QLabel>
using namespace std;

void euler(QTableWidget *fwidget, int nodes, QListWidget *column,QLabel *ui_status);
void degreeforeuler(QTableWidget *&fwidget, int nodes, vector<vector<int> > &c, vector < int > &degree);

#endif // EULERIAN_PATH_H
