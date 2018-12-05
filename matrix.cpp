#include "matrix.h"

QList<QPair<QString, QString>> serialize(QList<Edge*> edges)
{
    QList<QPair<QString, QString>> tuples;

    for(int i = 0;i < edges.size();i++)
    {
        tuples.append(qMakePair(edges.at(i)->begin->texttoint, edges.at(i)->end->texttoint));
    }

    return tuples;
}

void init_matrixes(QTableWidget *&fwidget, QTableWidget *&swidget, int nodes, int edges)
{
    fwidget->setRowCount(nodes);
    fwidget->setColumnCount(nodes);

    for(int row = 0;row < nodes;row++)
    {
        for(int col = 0;col < nodes;col++)
        {
            fwidget->setItem(row, col, new QTableWidgetItem("0"));
            fwidget->item(row,col)->setTextAlignment(Qt::AlignCenter);
        }

    }

    swidget->setRowCount(nodes);
    swidget->setColumnCount(edges);

    for(int row = 0;row < nodes;row++)
    {
        for(int col = 0;col < edges;col++)
        {
            swidget->setItem(row, col, new QTableWidgetItem("0"));
            swidget->item(row,col)->setTextAlignment(Qt::AlignCenter);
        }
    }

}

// adj matrix

void get_matrix_adj(QList<Node*> nodes, QList<Edge*> edges, QTableWidget *&widget, bool isdirected)
{

    QList<QPair<QString, QString>> ser_edges = serialize(edges);
    widget->setRowCount(nodes.size());
    widget->setColumnCount(nodes.size());
    for(int i = 0;i < nodes.size();i++)
    {
        widget->setHorizontalHeaderItem(i, new QTableWidgetItem(nodes.at(i)->texttoint));
        widget->setVerticalHeaderItem(i, new QTableWidgetItem(nodes.at(i)->texttoint));

    }

    for(int i = 0;i < ser_edges.size();i++)
    {
        int row = -1;
        int column = -1;
        for(int j = 0;j < nodes.size();j++)
        {
            if(widget->horizontalHeaderItem(j)->text() == ser_edges.at(i).first)
            {
                row = j;
            }
            if(widget->verticalHeaderItem(j)->text() == ser_edges.at(i).second)
            {
                column = j;
            }
        }
        if(row != -1  && column != -1)
        {
            int num = widget->item(row, column)->text().toInt();

            num++;
            widget->item(row, column)->setText(QString::number(num));
            if(!isdirected)
            {
                widget->item(column, row)->setText(QString::number(num));
                widget->item(column, row)->setTextColor(Qt::white);
                widget->item(column, row)->setBackgroundColor(Qt::gray);

            }
            widget->item(row, column)->setBackgroundColor(Qt::gray);
            widget->item(row, column)->setTextColor(Qt::white);
        }
    }
}


void get_matrix_inc(QList<Node*> nodes, QList<Edge*> edges, QTableWidget *&widget, bool isdirected)
{
    QList<QPair<QString, QString>> ser_edges = serialize(edges);
    widget->setRowCount(nodes.size());
    widget->setColumnCount(ser_edges.size());
    for(int i = 0;i < nodes.size();i++)
    {
        widget->setVerticalHeaderItem(i, new QTableWidgetItem(nodes.at(i)->texttoint));
    }

    for(int i = 0; i < ser_edges.size();i++)
    {
        widget->setHorizontalHeaderItem(i, new QTableWidgetItem(ser_edges.at(i).first+ser_edges.at(i).second));
    }

    for(int i = 0; i < nodes.size();i++)
    {
        for(int j = 0;j < ser_edges.size();j++)
        {
            if(nodes.at(i)->texttoint == ser_edges.at(j).first || nodes.at(i)->texttoint == ser_edges.at(j).second)
            {
                if(isdirected)
                {
                    if(ser_edges.at(j).second == ser_edges.at(j).first)
                    {
                        widget->item(i, j)->setText("2");
                    }
                    else if(nodes.at(i)->texttoint == ser_edges.at(j).first)
                    {
                        widget->item(i, j)->setText("1");
                    }
                    else
                    {
                        widget->item(i, j)->setText("-1");
                    }
                    widget->item(i, j)->setBackgroundColor(Qt::gray);
                }
                else
                {
                    if(ser_edges.at(j).second == ser_edges.at(j).first)
                    {
                        widget->item(i, j)->setText("2");
                    }
                    else
                    {
                        widget->item(i, j)->setText("1");
                        widget->item(i, j)->setBackgroundColor(Qt::gray);
                    }

                }
            }
        }
    }
}



void from_adj_to_inc(QTableWidget *fwidget, QTableWidget *&swidget, QList<Node*> nodes, QList<Edge*> edges, bool isdirected)
{
    Q_UNUSED(edges);
    int endpoint;
    int edge = 0;
    if(!isdirected)
    {
        endpoint = 0;
    }
    else
    {
        endpoint = nodes.size() - 1;
    }
    for(int i = 0;i < nodes.size();i++)
    {
        swidget->setVerticalHeaderItem(i, new QTableWidgetItem(nodes.at(i)->texttoint));
        for(int j = 0;j <= endpoint;j++)
        {
            if(fwidget->item(i, j)->text().toInt() != 0)
            {
                QString header_text = fwidget->verticalHeaderItem(i)->text() + fwidget->horizontalHeaderItem(j)->text();
                // for directed
                if(isdirected)
                {
                    // if loop
                    if(i == j)
                    {
                        for(int num = 0;num < fwidget->item(i, j)->text().toInt();num++)
                        {
                            swidget->setHorizontalHeaderItem(edge, new QTableWidgetItem(header_text));
                            swidget->item(i, edge)->setText("2");
                            swidget->item(i, edge)->setBackgroundColor(Qt::gray);

                            edge++;
                        }
                    }
                    // if not loop
                    else
                    {
                        for(int num = 0;num < fwidget->item(i, j)->text().toInt();num++)
                        {
                            swidget->setHorizontalHeaderItem(edge, new QTableWidgetItem(header_text));
                            swidget->item(i, edge)->setText("1");
                            swidget->item(j, edge)->setText("-1");
                            swidget->item(j, edge)->setBackgroundColor(Qt::gray);
                            swidget->item(i, edge)->setBackgroundColor(Qt::gray);
                            edge++;
                        }
                    }
                }
                // for undirected
                else
                {
                    for(int num = 0;num < fwidget->item(i, j)->text().toInt();num++)
                    {
                        swidget->setHorizontalHeaderItem(edge, new QTableWidgetItem(header_text));
                        swidget->item(i, edge)->setText("1");
                        swidget->item(j, edge)->setText("1");
                        swidget->item(i, edge)->setBackgroundColor(Qt::gray);
                        swidget->item(j, edge)->setBackgroundColor(Qt::gray);
                        edge++;
                    }
                }
            }
        }
        if(!isdirected)
        {
            endpoint++;
        }
    }
}


void from_inc_to_adj(QTableWidget *fwidget, QTableWidget *&swidget, QList<Node*> nodes, QList<Edge*> edges, bool isdirected)
{
    // coor for beging and end
    int begin_c = -1;
    int end_c = -1;
    int num;
    for(int i = 0;i < nodes.size();i++)
    {
        swidget->setHorizontalHeaderItem(i, new QTableWidgetItem(nodes.at(i)->texttoint));
        swidget->setVerticalHeaderItem(i, new QTableWidgetItem(nodes.at(i)->texttoint));
    }

    for(int i = 0;i < edges.size();i++)
    {
        for(int j = 0;j < nodes.size();j++)
        {
            if(fwidget->item(j, i)->text().toInt() != 0)
            {
                if(isdirected)
                {
                    if(fwidget->item(j, i)->text().toInt() == 2)
                    {
                        num = fwidget->item(j, i)->text().toInt();
                        num++;
                        swidget->item(j, j)->setText(QString::number(num));
                        swidget->item(j, j)->setBackgroundColor(Qt::gray);
                        continue;
                    }
                    else if(fwidget->item(j, i)->text().toInt() == 1)
                    {
                        begin_c = j;
                    }
                    else if(fwidget->item(j, i)->text().toInt() == -1)
                    {
                        end_c = j;
                    }
                }
                else
                {
                    if(fwidget->item(j, i)->text().toInt() == 2)
                    {
                        num = fwidget->item(j, i)->text().toInt();
                        num++;
                        swidget->item(j, j)->setText(QString::number(num));
                        swidget->item(j, j)->setBackgroundColor(Qt::gray);
                        continue;
                    }
                    else if(fwidget->item(j, i)->text().toInt() == 1)
                    {
                        // find begin and end coor
                        if(begin_c != -1)
                        {
                            end_c = j;
                        }
                        else
                        {
                            begin_c = j;
                        }
                    }
                }
            }
            if( begin_c != -1 && end_c != -1)
            {
                num = swidget->item(begin_c, end_c)->text().toInt();
                num++;
                swidget->item(begin_c, end_c)->setText(QString::number(num));
                swidget->item(begin_c, end_c)->setBackgroundColor(Qt::gray);
                if(!isdirected)
                {
                    swidget->item(end_c, begin_c)->setText(QString::number(num));
                    swidget->item(end_c, begin_c)->setBackgroundColor(Qt::gray);
                }
                begin_c = -1;
                end_c = -1;
            }
        }
    }
}
