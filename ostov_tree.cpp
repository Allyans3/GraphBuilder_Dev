#include "ostov_tree.h"

bool compare(const Edge* v1, const Edge* v2)
{
    return v1->textEdge.toInt()  < v2->textEdge.toInt();
}

Edge * nearest_path(QList<Edge *> edges, QList<Node *> temp, QList<Node *> nodes)
{
    for(int edge = 0;edge < edges.size();edge++)
    {
        for(int node = 0;node < temp.size();node++)
        {
            if(temp.at(node) == edges.at(edge)->begin)
            {
                if(nodes.contains(edges.at(edge)->end))
                    return edges.at(edge);
            }
            else if(temp.at(node) == edges.at(edge)->end)
            {
                if(nodes.contains(edges.at(edge)->begin))
                    return edges.at(edge);
            }
        }
    }
    return nullptr;
}

QList<Edge*> ostov_tree(QList<Node*> nodes, QList<Edge*> edges)
{
    QList<Edge*> result;
    QList<Node*> temp;

    std::sort(edges.begin(), edges.end(), compare);

    result.append(edges.at(0));

    temp.append(edges.at(0)->begin);
    temp.append(edges.at(0)->end);

    nodes.removeAll(edges.at(0)->begin);
    nodes.removeAll(edges.at(0)->end);

    edges.removeAt(0);

    Edge * min = nullptr;

    while(!nodes.empty())
    {
        min = nearest_path(edges, temp, nodes);

        if(!min)
            break;

        result.append(min);

        temp.append(min->begin);
        temp.append(min->end);

        nodes.removeAll(min->begin);
        nodes.removeAll(min->end);

        edges.removeAll(min);
    }
    if(!min)
        result.clear();
    return result;
}
