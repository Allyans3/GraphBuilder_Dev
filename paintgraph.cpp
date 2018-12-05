#include "paintgraph.h"

bool compare(const QPair<Node*,int> &v1, const QPair<Node*,int> &v2)
{
    return v1.second > v2.second;
}

QList<QPair<Node*, int>> node_degres(QList<Node*> nodes)
{
    QList<QPair<Node*, int>> degre_list;
    int deg;
    for(int i = 0;i < nodes.size();i++)
    {
        deg = 0;
        for(int j = 0;j < nodes.at(i)->edges().size();j++)
        {
            if(nodes.at(i)->edges().at(j)->end == nodes.at(i)->edges().at(j)->begin)
            {
                deg+= 2;
            }
            else
            {
                deg++;
            }

        }
        degre_list.append(qMakePair(nodes.at(i), deg));
    }
    std::sort(degre_list.begin(), degre_list.end(), compare);
    return  degre_list;
}

bool adj(Node * check, QList<Node *> node)
{
    for(int i = 0;i < node.size();i++)
    {
        for(int j = 0;j < check->edges().size();j++)
        {
            if(check->edges().at(j)->begin == node.at(i) ||
                   check->edges().at(j)->end == node.at(i))
            {
                return false;
            }
        }
    }
    return  true;
}

QList<QPair<Node*, QColor>> paint_graph(QList<Node*> nodes)
{
    QList<QPair<Node*, int>> degre_list = node_degres(nodes);
    QList<Node*> group;

    QColor color = QColor::fromRgb(QRandomGenerator::global()->generate());

    QList<QPair<Node*, QColor>> result;

    // iteration cycle
    for(int node = 0;node < degre_list.size();)
    {
        group.append(degre_list.at(0).first);
        degre_list.removeAt(0);
        for(int i = 0;i < group.size();i++)
        {
            for(int j = 0;j < degre_list.size();j++)
            {
                // check for adj
                if(adj(degre_list.at(j).first, group))
                {
                    group.append(degre_list.at(j).first);
                    degre_list.removeAt(j);
                    j = 0;
                }
            }
        }

        for(int res = 0;res < group.size();res++)
        {
            result.append(qMakePair(group.at(res), color));
        }

        color = QColor::fromRgb(QRandomGenerator::global()->generate());
        group.clear();
    }
    return  result;
}

bool adj_edg(QList<Edge*> edges, Edge* check)
{
    for(int i = 0;i < edges.size();i++)
    {
        if(check->end == edges.at(i)->begin || check->end == edges.at(i)->end ||
                check->begin == edges.at(i)->begin || check->begin == edges.at(i)->end)
        {
            return false;
        }
    }

    return true;
}


QList<QPair<Edge*, QColor>> paint_edges(QList<Edge*> edges)
{
    QList<Edge*> group;
    QColor color = QColor::fromRgb(QRandomGenerator::global()->generate());
    QList<QPair<Edge*, QColor>> result;

    while(!edges.isEmpty())
    {
        group.append(edges.at(0));
        edges.removeAt(0);

        for(int i = 0;i < group.size();i++)
        {
            for(int j = 0;j < edges.size();j++)
            {
                if(adj_edg(group, edges.at(j)))
                {
                    group.append(edges.at(j));
                    edges.removeAt(j);
                    j = 0;
                }
            }
        }

        for(int res = 0; res < group.size();res++)
            result.append(qMakePair(group.at(res), color));

        color = QColor::fromRgb(QRandomGenerator::global()->generate());
        group.clear();
    }
    return  result;
}
