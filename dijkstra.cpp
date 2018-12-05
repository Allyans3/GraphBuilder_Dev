#include "dijkstra.h"
#include <QDebug>
#define NSTART 3

int find_min(QTableWidget * matrix, int row)
{
    int min_col = NSTART;
    int min = -1;
    for(int col = NSTART;col < matrix->columnCount();col++)
    {
        if(matrix->item(row, col)->text().data() != QString("∞"))
        {
            if(min == -1)
            {
                min = matrix->item(row, col)->text().toInt();
                min_col = col;
            }
            else if(min > matrix->item(row, col)->text().toInt())
            {
                min = matrix->item(row, col)->text().toInt();
                min_col = col;
            }
        }
    }
    return min_col;
}

int find_min_weight(int col,QTableWidget *matrix)
{
    int cur = -1;
    for(int row=0; row <matrix->rowCount(); row++)
    {
        if(matrix->item(row,col) == nullptr || matrix->item(row,col)->text().data() == QString("∞"))
            continue;
        else
        {
            if(cur == -1)
            {
                cur = matrix->item(row, col)->text().toInt();
            }
            else if(cur > matrix->item(row, col)->text().toInt())
            {
                cur = matrix->item(row, col)->text().toInt();
            }
        }
    }
    return cur;
}

void dijkstra_steps(QListWidget *list,QTableWidget *matrix,int row,QList<Node*> nodes_copy,QList<Node*> visited,int min_col,int const_label)
{
    QString S = "";
    QString temp = "";
    int i=0, digit = 0;
    for(int n=0; n<nodes_copy.size();n++)
    {
        if(!visited.contains(nodes_copy.at(n)))
        {
            if(S == "" || temp == "")
            {
                for (int var = 0; var < nodes_copy.at(n)->textContent().length(); ++var)
                    if (nodes_copy.at(n)->textContent().at(var).isDigit())
                        digit = nodes_copy.at(n)->textContent().at(var).digitValue();
                S = QString(nodes_copy.at(n)->textContent());
                temp = QString(nodes_copy.at(n)->textContent() + "-" + QString(matrix->item(row,digit+1)->text()));
            }
            else
            {
                for (int var = 0; var < nodes_copy.at(n)->textContent().length(); ++var)
                    if (nodes_copy.at(n)->textContent().at(var).isDigit())
                        digit = nodes_copy.at(n)->textContent().at(var).digitValue();
                S = QString(S + "," + nodes_copy.at(n)->textContent());
                temp = QString(temp + "; " + nodes_copy.at(n)->textContent()+ "-" + QString(matrix->item(row,digit+1)->text()));
            }
            i++;
        }
    }
    if(S != "")
    {
        if(row == 0)
        {
            QListWidgetItem *newitem = new QListWidgetItem(list);
            newitem->setTextAlignment(Qt::AlignCenter);
            newitem->setFont(QFont("Times", 10, QFont::Bold));
            newitem->setText(QString("Step 0. The first node X1 has a static label 0. All other nodes has a temporary labels ∞."));
            list->addItem(newitem);
        }
        QListWidgetItem *newitem = new QListWidgetItem(list);
        newitem->setTextAlignment(Qt::AlignCenter);
        newitem->setFont(QFont("Times", 10, QFont::Bold));
        newitem->setText(QString("Step " + QString::number(row+1) + ". Take the node - " + nodes_copy.at(min_col)->textContent() + " and define the set of subsequent nodes: {" + S + "}. Temporary labels: " + temp + ". Set the node X" + QString::number(const_label-1) + " a constant label " + QString(matrix->item(row,(const_label))->text()) + " as it is minimal."));
        list->addItem(newitem);
    }
    else
    {
        QListWidgetItem *newitem = new QListWidgetItem(list);
        newitem->setTextAlignment(Qt::AlignCenter);
        newitem->setFont(QFont("Times", 10, QFont::Bold));
        newitem->setText(QString("Step " + QString::number(row+1) + ". Take the node - " + nodes_copy.at(min_col)->textContent() + ". This node already has a constant label."));
        list->addItem(newitem);
        QListWidgetItem *newitem1 = new QListWidgetItem(list);
        newitem1->setTextAlignment(Qt::AlignCenter);
        newitem1->setFont(QFont("Times", 10, QFont::Bold));
        newitem1->setText(QString("Tree of the shortest paths: " + QString(matrix->item(row,1)->text())));
        list->addItem(newitem1);
    }
}

void find_dijkstra(QList<Node*> nodes,QTableWidget *matrix,QListWidget *list)
{
    QList<Node*> visited;
    QList<Node*> nodes_copy = nodes;
    int cur=0, min_col = 0, weight = 0;
    QString S = "X1";

    list->clear();
    visited.append(nodes.first());
    nodes.removeAll(nodes.first());

    for(int row=0; row < matrix->rowCount(); row++)
    {
        matrix->setItem(row, 0, new QTableWidgetItem(QString::number(row+1)));
        matrix->item(row,0)->setTextAlignment(Qt::AlignCenter);

        S = "X1";
        for(int d=1; d<visited.size();d++)
            S = QString(S + "," + visited.at(d)->textContent());
        matrix->setItem(row, 1, new QTableWidgetItem(QString("{" + S + "}")));
        matrix->item(row,1)->setTextAlignment(Qt::AlignCenter);

        matrix->setItem(row, 2, new QTableWidgetItem(QString(nodes_copy.at(min_col)->textContent())));
        matrix->item(row,2)->setTextAlignment(Qt::AlignCenter);

        for(int col = NSTART; col < matrix->columnCount(); col++)
        {
            bool chida=0;
            for(int visit=0; visit < visited.size(); visit++)
            {
                if(visited.at(visit)->textContent() == nodes.at(col-NSTART)->textContent())
                {
                    matrix->setItem(row, col, new QTableWidgetItem(QString("∞")));
                    matrix->item(row,col)->setTextAlignment(Qt::AlignCenter);
                    chida = 1;
                    break;
                }
            }
            if(chida == 1)
                continue;
            for(int j=0; j < nodes_copy.at(min_col)->edges().size(); j++)
            {
                if(nodes_copy.at(min_col)->edges().at(j)->end == nodes.at(col-NSTART))
                {
                    cur = nodes_copy.at(min_col)->edges().at(j)->textEdge.toInt()+weight;
                    matrix->setItem(row, col, new QTableWidgetItem(QString::number(cur)));
                    matrix->setItem(row, col, new QTableWidgetItem(QString::number(find_min_weight(col,matrix))));
                    matrix->item(row,col)->setTextAlignment(Qt::AlignCenter);
                    break;
                }
                else
                {
                    if(find_min_weight(col,matrix)  == -1)
                        matrix->setItem(row, col, new QTableWidgetItem(QString("∞")));
                    else
                        matrix->setItem(row, col, new QTableWidgetItem(QString::number(find_min_weight(col,matrix))));
                    matrix->item(row,col)->setTextAlignment(Qt::AlignCenter);
                }
            }
            cur = 0;
        }
        dijkstra_steps(list,matrix,row,nodes_copy,visited,min_col,find_min(matrix,row));
        min_col = find_min(matrix,row)-2;
        if(matrix->item(row,(min_col+2))->text().data() == QString("∞"))
            weight = 0;
        else
            weight = matrix->item(row,(min_col+2))->text().toInt();
        visited.append(nodes_copy.at(min_col));
    }
}
