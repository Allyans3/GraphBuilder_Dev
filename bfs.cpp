#include "bfs.h"

void update_temp(QList<Node*> &temp, QList<Node*> &visited, int &lenght, QListWidget *&column)
{
    Edge *cur_edge = nullptr;
    QSet<Node*> new_temp;
    lenght++;
    for(int i = 0;i < temp.size();i++)
    {
        for(int edge = 0;edge < temp.at(i)->edges().size();edge++)
        {
            cur_edge = temp.at(i)->edges().at(edge);
            if(temp.at(i) == cur_edge->end)
            {
                if(!visited.contains(temp.at(i)) && !visited.contains(cur_edge->begin)
                         && !temp.contains(cur_edge->begin))
                {
                    new_temp.insert(cur_edge->begin);
                }
            }
            else if(temp.at(i) == cur_edge->begin)
            {
                if(!visited.contains(temp.at(i)) && !visited.contains(cur_edge->end)
                        && !temp.contains(cur_edge->end))
                {
                    new_temp.insert(cur_edge->end);
                }
            }
        }
    }

    for(int i = 0;i < temp.size();i++)
    {
        QListWidgetItem *newitem = new QListWidgetItem(column);
        newitem->setTextAlignment(Qt::AlignCenter);
        newitem->setFont(QFont("Times", 10, QFont::Bold));
        newitem->setText(QString(temp.at(i)->texttoint+" - " + QString::number(lenght)));
        column->addItem(newitem);
        visited.append(temp.at(i));
    }
    temp.clear();
    temp = QList<Node*>::fromSet(new_temp);
}

void bfs(QList<Node*> nodes,QListWidget *column)
{
    QList<Node*> visited;
    QList<Node*> temp;
    int lenght = -1;

    temp.append(nodes.at(0));
    column->clear();

    while(!temp.isEmpty())
    {
        update_temp(temp, visited, lenght, column);
    }
}

Node* update_dfs(QList<Node*> &visited, QList<Node*> &nodes)
{
    if(nodes.empty())
        return nullptr;
    Node * last = visited.last();

    for(int i = 0;i < last->edges().size();i++)
    {
        if(last->edges().at(i)->end == last)
        {
            if(!visited.contains(last->edges().at(i)->begin) &&
                    nodes.contains(last->edges().at(i)->begin))
            {
                return last->edges().at(i)->begin;
            }
        }
        else if(last->edges().at(i)->begin == last)
        {

            if(!visited.contains(last->edges().at(i)->end) &&
                    nodes.contains(last->edges().at(i)->end))
            {
                return last->edges().at(i)->end;
            }
        }
    }
    return nullptr;
}

void dfs(QList<Node*> nodes,QListWidget *column)
{
    for(int node = 0;node < nodes.size();node++)
    {
        if(!nodes.at(node)->edges().size())
        {
            nodes.removeAt(node);
            node--;
        }
    }
    QList<Node*> visited;

    visited.append(nodes.first());
    column->clear();

    QListWidgetItem *newitem = new QListWidgetItem(column);
    newitem->setTextAlignment(Qt::AlignCenter);
    newitem->setFont(QFont("Times", 10, QFont::Bold));
    newitem->setText(QString(nodes.first()->texttoint+" - " + QString::number(1)));
    column->addItem(newitem);
    nodes.removeFirst();

    int step = 2;
    Node * cur = nullptr;

    while(!nodes.empty())
    {
        cur = update_dfs(visited, nodes);
        if(!cur)
            visited.removeLast();
        else
        {
            QListWidgetItem *newitem = new QListWidgetItem(column);
            newitem->setTextAlignment(Qt::AlignCenter);
            newitem->setFont(QFont("Times", 10, QFont::Bold));
            newitem->setText(QString(cur->texttoint+" - " + QString::number(step)));
            column->addItem(newitem);
            visited.append(cur);
            nodes.removeAll(cur);

            step++;
        }
    }
}
