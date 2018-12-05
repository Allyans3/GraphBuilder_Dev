#include "eulerian_path.h"

void degreeforeuler(QTableWidget *&fwidget, int nodes,vector<vector<int>> &c,vector <int> &degree)
{
    for(int q=0;q< nodes; q++)
        for(int w=0;w < nodes; w++)
            c[q][w] = fwidget->item(q,w)->text().toInt();

    for (int i = 0; i < nodes; i++)
        for (int j = 0; j < nodes; j++)
            if (fwidget->item(i,j)->text().toInt() >=1)
                degree[i] += fwidget->item(i,j)->text().toInt();
}

void euler (QTableWidget *fwidget, int nodes, QListWidget *column, QLabel *ui_status)
{
    column->clear();
    vector<vector<int>> c(nodes,vector<int>(nodes));
    vector <int> degree (nodes,0);
    degreeforeuler(fwidget,nodes,c,degree);

    int count = 0;
    for (int h = 0; h < degree.size(); h++)
    {
        if(degree[h] == 0)
        {
            ui_status->setText("Error! Graph has isolated nodes.");
            return;
        }
        if (degree[h] % 2 != 0)
        {
            count++;
            if (count == 1)
            {
                ui_status->setText("Error! Node or nodes has an odd number of edges.");
                return;
            }
        }
    }

    int first = 0;
    while (!degree[first])  ++first;

    stack<int> st;
    st.push (first);
    vector<int> res;
    while (!st.empty())
    {
        int v = st.top();
        int t;
        for (t=0; t<nodes; ++t)
            if (c[v][t])
                break;
        if (t == nodes)
        {
            res.push_back (v);
            st.pop();
        }
        else
        {
            --c[v][t];
            --c[t][v];
            st.push (t);
        }
    }

    for(int re=0;re < res.size()-1;re++)
    {
        QListWidgetItem *newitem = new QListWidgetItem(column);
        newitem->setTextAlignment(Qt::AlignCenter);
        newitem->setFont(QFont("Times", 10, QFont::Bold));
        newitem->setText(QString("X"+QString::number(res[re]+1)+" - "+"X"+QString::number(res[re+1]+1)));
        column->addItem(newitem);
        ui_status->setText("Eulerial path built.");
    }
}
