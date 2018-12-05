#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);

    this->setWindowIcon(QIcon(":/new/prefix1/logo/logo.ico"));
    connect(ui->exitbut,&QPushButton::clicked,this,&QWidget::close);
    connect(ui->minimize,&QPushButton::clicked,this,&QWidget::showMinimized);
    connect(ui->maximize,&QPushButton::clicked,this,[this]{
        if (this->isMaximized())
            this->showNormal();
        else
            this->showMaximized();
    });

    scene = new PaintScene();
    ui->graph->setScene(scene);
    ui->graph_2->setScene(scene);
    ui->label_4->hide();

    ui->matrix_1->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->matrix_1->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->matrix_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->matrix_2->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->matrix_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &MainWindow::slotTimer);
    timer->start(100);

    connect(ui->node,&QPushButton::clicked,this,[this]{ but_node_change();});
    connect(ui->edge,&QPushButton::clicked,this,[this]{ addedge(); });
    connect(ui->delete_2,&QPushButton::clicked,this,[this]{
        if(scene->selectedItems().size() > 0)
            for(int i=scene->selectedItems().size(); i>=0; i--)
                deleteitem();
    });
    connect(ui->build_matrix,&QPushButton::clicked,this,[this]{
        build_matrixes();
        if(scene->edges.size() >= 1)
            ui->status->setText("Matrixes built.");
    });
    connect(ui->clear,&QPushButton::clicked,this,[this]{
        clear();
        ui->status->setText("Cleared.");
    });
    connect(ui->clear_all,&QPushButton::clicked,this,[this]{
        clear();
        ui->label_cnn->setText(QString("CNN: "+QString::number(0)));
        ui->label_cen->setText(QString("CEN: "+QString::number(0)));
        ui->ways->setText("--NONE--");
        scene->nodes.clear();
        scene->edges.clear();
        scene->clear();
        scene->nums.clear();
        ui->status->setText("All cleared.");
    });
    connect(ui->build_eulerian,&QPushButton::clicked,this,[this]{
        if(graph_direction() == false)
        {
            init_matrixes(ui->matrix_1, ui->matrix_2, this->scene->nodes.size(), this->scene->edges.size());
            get_matrix_adj(this->scene->nodes,this->scene->edges, ui->matrix_1, graph_direction());
            euler(ui->matrix_1,this->scene->nodes.size(),this->ui->listWidget,ui->status);
        }
        else
            ui->status->setText("Error! Graph is directed.");
        ui->matrix_1->setColumnCount(0);
        ui->matrix_1->setRowCount(0);
        ui->matrix_1->clear();
        ui->matrix_2->setColumnCount(0);
        ui->matrix_2->setRowCount(0);
        ui->matrix_2->clear();
    });
    connect(ui->paint_graph,&QPushButton::clicked,this,[this]{
        if(scene->nodes.size() == 0 || scene->edges.size() == 0)
            return;
        QList<QPair<Node*, QColor>> colorNode = paint_graph(scene->nodes);
        QList<QPair<Edge*, QColor>> colorEdge = paint_edges(scene->edges);
        QEventLoop loop;
        QColor colorN = colorNode.at(0).second;
        QColor colorE = colorEdge.at(0).second;
        int colorN1 = 1, colorE1 = 1;

        for(int i=0; i<colorNode.size(); i++)
        {
            QTimer::singleShot(70,&loop,SLOT(quit()));
            loop.exec();
            colorNode.at(i).first->setBrush(colorNode.at(i).second);

            if(colorN != colorNode.at(i).second)
            {
                colorN = colorNode.at(i).second;
                colorN1++;
            }
        }
        for(int j=0; j<colorEdge.size(); j++)
        {
            QTimer::singleShot(70,&loop,SLOT(quit()));
            loop.exec();
            colorEdge.at(j).first->setPen(colorEdge.at(j).second);

            if(colorE != colorEdge.at(j).second)
            {
                colorE = colorEdge.at(j).second;
                colorE1++;
            }
        }
        ui->label_cnn->setText(QString("CNN: "+QString::number(colorN1)));
        ui->label_cen->setText(QString("CEN: "+QString::number(colorE1)));
        ui->status->setText("Graph painted.");
    });
    connect(ui->undo,&QPushButton::clicked,this,[this]{
        for(int i=0; i<this->scene->nodes.size(); i++)
        {
            this->scene->nodes.at(i)->setBrush(Qt::red);
            ui->status->setText("Undo done.");
        }
        scene->clearSelection();
        repaintedge();
        ui->label_cnn->setText(QString("CNN: "+QString::number(0)));
        ui->label_cen->setText(QString("CEN: "+QString::number(0)));
    });
    connect(scene,SIGNAL(changed(QList<QRectF>)),this,SLOT(editing()));
    connect(ui->slider,&QSlider::valueChanged,this,[this]{
        if(ui->slider->value() == 1)
        {
            ui->label_4->hide();
            ui->label_3->show();
            ui->graph->setDragMode(QGraphicsView::ScrollHandDrag);
            ui->graph_2->setDragMode(QGraphicsView::ScrollHandDrag);
            ui->status->setText("Setted Hand Drag mode");
        }
        else
        {
            ui->label_4->show();
            ui->label_3->hide();
            ui->graph->setDragMode(QGraphicsView::RubberBandDrag);
            ui->graph_2->setDragMode(QGraphicsView::RubberBandDrag);
            ui->status->setText("Setted Band Drag mode");
        }
    });
    connect(ui->build_ostov,&QPushButton::clicked,this,[this]{
        int digit = 0,digit1 = 0;
        for (int var = 0; var < ui->label_loop->text().length(); ++var)
            if (ui->label_loop->text().at(var).isDigit())
                digit = ui->label_loop->text().at(var).digitValue();
        for (int var = 0; var < ui->label_parallel->text().length(); ++var)
            if (ui->label_parallel->text().at(var).isDigit())
                digit1 = ui->label_parallel->text().at(var).digitValue();
        if(scene->nodes.size() == 0 || scene->edges.size() == 0 || digit >=1 || digit1 >=1)
            return;
        QList<Edge*> path = ostov_tree(scene->nodes,scene->edges);
        if(path.isEmpty())
        {
            ui->status->setText("Error ostov.");
            return;
        }
        if(path.size() == scene->edges.size())
            ui->status->setText("No need to build an ostov tree.");
        else
            ui->status->setText("Ostov built.");
        for(int i=scene->edges.size()-1;i >= 0; i--)
        {
            if(!path.contains(scene->edges.at(i)))
            {
                scene->edges.at(i)->setSelected(true);
                deleteitem();
            }
        }
    });
    connect(ui->push_set_text,&QPushButton::clicked,this,[this]{
        if (scene->selectedItems().size() > 0)
        {
            auto i = scene->selectedItems().at(0);
            if (i->type() == Edge::Type)
            {
                Edge *item = dynamic_cast<Edge*>(i);
                item->textEdge = QString::number(ui->edge_number->text().toInt());
                update();
                this->scene->clearSelection();
                ui->edge_number->clear();
                ui->status->setText("Edge text changed.");
            }
            else
                ui->status->setText("Error changing text.");
        }
    });
    connect(ui->build_bfs,&QPushButton::clicked,this,[this]{
        if (scene->edges.size() >= 1 && scene->nodes.size() >= 1){
            bfs(scene->nodes,ui->listWidget);
            ui->status->setText("BFS completed.");
        }
    });
    connect(ui->build_dfs,&QPushButton::clicked,this,[this]{
        if (scene->edges.size() >= 1 && scene->nodes.size() >= 1){
            dfs(scene->nodes,ui->listWidget);
            ui->status->setText("DFS completed.");
        }
    });
    connect(ui->find_dijkstra,&QPushButton::clicked,this,[this]{
        dijkstra_func();
    });
    connect(ui->find_dijkstra_2,&QPushButton::clicked,this,[this]{
        dijkstra_func();
    });
    connect(ui->back,&QPushButton::clicked,this,[this]{
        ui->stackedWidget->setCurrentIndex(0);
        ui->stackedHeader->setCurrentIndex(0);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::check_mult()
{
    if(scene->nodes.size() == 0)
        return;
    QList<Node*> nodes_copy = scene->nodes;
    int count = 0;
    for(int node = 0;node < nodes_copy.size();node++)
    {
        if(!nodes_copy.at(node)->edges().size())
        {
            nodes_copy.removeAt(node);
            node--;
            count++;
        }
    }
    ui->label_isol_count->setText(QString::number(count));
    if(nodes_copy.size() == 0)
        return;
    QList<Node*> visited = check_multigraph(0,nodes_copy);
    QList<Node*> temp;
    int cnt=1;

    if(nodes_copy.size() > visited.size())
    {
        while(!(nodes_copy.size() == visited.size()))
        {
            for(int i = 0;i < nodes_copy.size();i++)
            {
                if(!visited.contains(nodes_copy.at(i)))
                {
                    temp = check_multigraph(i,nodes_copy);
                    for(int j=0; j< temp.size(); j++)
                        visited.append(temp.at(j));
                    temp.clear();
                    cnt++;
                    break;
                }
            }
        }
    }
    ui->label_ngs_count->setText(QString::number(cnt));
}


QList<Node*> MainWindow::check_multigraph(int start,QList<Node*> nodes_copy)
{
    QList<Node*> visited;
    QList<Node*> temp;

    temp.append(nodes_copy.at(start));

    while(!temp.isEmpty())
    {
        Edge *cur_edge = nullptr;
        QSet<Node*> new_temp;
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
            visited.append(temp.at(i));
        }
        temp.clear();
        temp = QList<Node*>::fromSet(new_temp);
    }
    return visited;
}

void MainWindow::dijkstra_func()
{
    if(scene->nodes.size() < 2 || scene->edges.size() < 1)
    {
        ui->status->setText("Dijkstra error. To small number of nodes or edges.");
        return;
    }
    for(int node = 0;node < scene->nodes.size();node++)
        if(!scene->nodes.at(node)->edges().size())
        {
            ui->status->setText("Dijkstra error. Graph has an isolated node(-s).");
            return;
        }
    ui->stackedHeader->setCurrentIndex(1);
    ui->stackedWidget->setCurrentIndex(1);
    ui->matrix_3->clear();
    ui->matrix_3->setColumnCount(scene->nodes.size()+2);
    ui->matrix_3->setRowCount(scene->nodes.size());
    ui->matrix_3->setHorizontalHeaderItem(0,new QTableWidgetItem("Iteration"));
    ui->matrix_3->setHorizontalHeaderItem(1,new QTableWidgetItem("S"));
    ui->matrix_3->setHorizontalHeaderItem(2,new QTableWidgetItem("w"));
    for(int i = 3;i < ui->matrix_3->columnCount();i++)
        ui->matrix_3->setHorizontalHeaderItem(i, new QTableWidgetItem(QString("D[X" + QString::number(i-1) + "]")));
    find_dijkstra(scene->nodes,ui->matrix_3,ui->listWidget_2);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_2:
        addedge();
        break;
    case Qt::Key_3:
        if(ui->slider->value() == 1)
            ui->slider->setValue(0);
        else
            ui->slider->setValue(1);
        break;
//    case Qt:::
//        qDebug() << "Ctrl+C";
//        break;
    default:
        break;
    }
}

void MainWindow::slotTimer()
{
    timer->stop();
    scene->setSceneRect(0,0, ui->graph->width()-4, ui->graph->height()-4);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    timer->start(100);
    ui->clear_all->move(46,ui->groupBox->height()-33);
    ui->clear->move(8,ui->groupBox->height()-33);
    QWidget::resizeEvent(event);
}

void MainWindow::but_node_change()
{
    if(scene->v == 0)
    {
        scene->v = 1;
        ui->node->setStyleSheet("background-color:rgb(255,96,89);padding: 0 8px;border: 0px solid gray;border-radius: 10px;color:#ffffff;");
        ui->status->setText("Node mode ON.");
    }
    else
    {
        scene->v = 0;
        ui->node->setStyleSheet("background-color:rgb(0,199,77);padding: 0 8px;border: 0px solid gray;border-radius: 10px;color:#ffffff;");
        ui->status->setText("Node mode OFF.");
    }
}

void MainWindow::editing()
{
    int paral = 0, loop = 0,temp = 0;
    ui->label_node->setText(QString("Nodes: "+QString::number(scene->nodes.size())));
    ui->label_edge->setText(QString("Edges: "+QString::number(scene->edges.size())));

    for(int i=0; i< scene->nodes.size();i++)
    {
        for(int k=i+1; k< scene->nodes.size();k++)
        {
            for(int j=0; j<this->scene->edges.size(); j++)
            {
                if(scene->nodes.at(i)->edges().contains(scene->edges.at(j)) && scene->nodes.at(k)->edges().contains(scene->edges.at(j)))
                {
                    temp++;
                }
            }
            if(temp > 1)
                paral += temp;
            temp = 0;
        }
    }
    ui->label_parallel->setText(QString("Parallels: "+QString::number(paral)));
    for(int j=0; j<this->scene->edges.size(); j++)
    {
        if(scene->edges.at(j)->begin->texttoint == scene->edges.at(j)->end->texttoint)
            loop++;
    }
    ui->label_loop->setText(QString("Loops: "+QString::number(loop)));
    check_mult();
}


void MainWindow::clear()
{
    ui->comboBox_2->setItemData(0, 33, Qt::UserRole -1);
    ui->comboBox_2->setItemData(1, 33, Qt::UserRole -1);
    ui->matrix_1->setColumnCount(0);
    ui->matrix_1->setRowCount(0);
    ui->matrix_2->setColumnCount(0);
    ui->matrix_2->setRowCount(0);
    ui->matrix_1->clear();
    ui->matrix_2->clear();
    ui->edge->setStyleSheet(" border: 0px solid gray;border-radius: 10px;padding: 0 8px;background-color:rgb(0,199,77);color:#ffffff");
    ui->edge->setFixedWidth(59);
    ui->edge->setText(QString("   Edge"));
    begin = nullptr;
    ui->listWidget->clear();
}

void MainWindow::repaintedge()
{
    for(int j=0; j<this->scene->edges.size(); j++)
    {
        this->scene->edges.at(j)->setPen(QPen(QColor(Qt::white)));
    }

    for(int j=0; j<this->scene->edges.size(); j++)
    {
        for(int t=0; t<this->scene->edges.size(); t++)
        {
            if(j == t)
                continue;
            if((scene->edges.at(j)->begin->texttoint == scene->edges.at(t)->begin->texttoint && scene->edges.at(j)->end->texttoint == scene->edges.at(t)->end->texttoint) ||
                    (scene->edges.at(j)->begin->texttoint == scene->edges.at(t)->end->texttoint && scene->edges.at(j)->end->texttoint == scene->edges.at(t)->begin->texttoint))
            {

                this->scene->edges.at(j)->setPen(QPen(QColor(Qt::green)));
            }
            else
            {
                if(scene->edges.at(j)->pen().color() != Qt::green)
                    this->scene->edges.at(j)->setPen(QPen(QColor(Qt::white)));
            }
        }
    }
}

void MainWindow::deleteitem()
{
    if (scene->selectedItems().size() > 0)
    {
        begin = nullptr;
        auto i = scene->selectedItems().at(0);
        if (i->type() == Node::Type)
        {
            Node* n = qgraphicsitem_cast<Node*>(i);
            scene->nodes.removeAll(n);
            for(int j= 0;j < scene->edges.size();)
            {
                if(n->texttoint == scene->edges.at(j)->begin->texttoint || n->texttoint == scene->edges.at(j)->end->texttoint)
                    scene->edges.removeAt(j);
                else
                    j++;
            }
            int digit = 0;
            for (int var = 0; var < n->textContent().length(); ++var)
                if (n->textContent().at(var).isDigit())
                    digit = n->textContent().at(var).digitValue();
            for(int h = 0; h < scene->nums.size(); h++)
                if(scene->nums.at(h) == digit)
                    scene->nums.removeAt(h);
            ui->status->setText("Node - X" + QString::number(digit) + " deleted.");
        }
        else if (i->type() == Edge::Type)
        {
            Edge *e = qgraphicsitem_cast<Edge*>(i);
            this->scene->edges.removeAll(e);
            ui->status->setText("Edge deleted.");
        }
        scene->removeItem(i);
        delete i;
    }

    repaintedge();
    if(scene->edges.size() == 0)
    {
        ui->comboBox_2->setItemData(0, 33, Qt::UserRole -1);
        ui->comboBox_2->setItemData(1, 33, Qt::UserRole -1);
    }
}

void MainWindow::addedge()
{
    if (scene->selectedItems().size() > 0)
    {
        if(ui->comboBox_2->currentText() == "Directed")
        {
            ch = 1;
            ui->ways->setText("Directed");
            ui->comboBox_2->setItemData(1, 0, Qt::UserRole -1);
        }
        else if(ui->comboBox_2->currentText() == "Undirected")
        {
            ch = 0;
            ui->ways->setText("Undirected");
            ui->comboBox_2->setItemData(0, 0, Qt::UserRole -1);
        }
        if (begin){
            end = qgraphicsitem_cast<Node*>(scene->selectedItems().at(0));
            ui->edge->setStyleSheet(" border: 0px solid gray;border-radius: 10px;padding: 0 8px;background-color:rgb(0,199,77);color:#ffffff");
            ui->edge->setFixedWidth(59);
            ui->edge->setText(QString("   Edge"));
        }
        else {
            ui->edge->setStyleSheet(" border: 0px solid gray;border-radius: 10px;padding: 0 8px;background-color:rgb(255,189,46);color:#ffffff");
            ui->edge->setFixedWidth(75);
            for(int i=0; i<scene->nodes.size(); i++)
                if(scene->selectedItems().at(0) == scene->nodes.at(i))
                    ui->edge->setText(QString(scene->nodes.at(i)->texttoint));
            begin = qgraphicsitem_cast<Node*>(scene->selectedItems().at(0));
            this->scene->clearSelection();
            return;
        }
        if(begin != end)
        {
            if(dynamic_cast<Node*>(begin) and dynamic_cast<Node*>(end))
            {
                if(ui->edge_number->text().isEmpty())
                    rebro = new Edge(begin, end,ch,QString(QString("")));
                else
                    rebro = new Edge(begin, end,ch,QString(QString::number(ui->edge_number->text().toInt())));
                if(isDuplicateEdge(rebro))
                {
                    this->rebro->setPen(QPen(QColor(Qt::green)));
                }
                else
                {
                    this->rebro->setPen(QPen(QColor(Qt::white)));
                }
                this->scene->edges.append(rebro);
                this->scene->addItem(rebro);
                begin = nullptr;
                end = nullptr;
                ui->status->setText("Edge added.");
            }
        }
        else if(begin == end)
        {
            if(dynamic_cast<Node*>(begin) and dynamic_cast<Node*>(end))
            {
                if(ui->edge_number->text().isEmpty())
                    rebro = new Edge(begin, end,ch,QString(QString("")));
                else
                    rebro = new Edge(begin, end,ch,QString(QString::number(ui->edge_number->text().toInt())));
                if(isDuplicateEdge(rebro))
                {
                    this->rebro->setPen(QPen(QColor(Qt::green)));
                }
                else
                {
                    this->rebro->setPen(QPen(QColor(Qt::white)));
                }
                this->scene->edges.append(rebro);
                this->scene->addItem(rebro);
                begin = nullptr;
                end = nullptr;
                ui->status->setText("Edge added.");
            }
        }
        this->scene->clearSelection();
        ui->edge_number->clear();
    }
}

bool MainWindow::graph_direction()
{
    if(ui->comboBox_2->currentText() == "Directed")
        return true;
    else
        return false;
}

void MainWindow::build_matrixes()
{
    if(this->scene->edges.size() > 0)
    {
        if(ui->comboBox_3->currentIndex() == 0)
        {
            init_matrixes(ui->matrix_1, ui->matrix_2, this->scene->nodes.size(), this->scene->edges.size());
            get_matrix_adj(this->scene->nodes,this->scene->edges, ui->matrix_1, graph_direction());
            from_adj_to_inc(ui->matrix_1, ui->matrix_2, this->scene->nodes, this->scene->edges,  graph_direction());
        }
        else
        {
            init_matrixes(ui->matrix_2, ui->matrix_1, this->scene->nodes.size(), this->scene->edges.size());
            get_matrix_inc(this->scene->nodes,this->scene->edges, ui->matrix_1, graph_direction());
            from_inc_to_adj(ui->matrix_1, ui->matrix_2, this->scene->nodes, this->scene->edges,  graph_direction());
        }
    }
}

bool MainWindow::isDuplicateEdge(Edge * edge)
{
    for(int i = 0;i < scene->edges.size();i++)
    {
        if((scene->edges.at(i)->begin->texttoint == edge->begin->texttoint && scene->edges.at(i)->end->texttoint == edge->end->texttoint) ||
                (scene->edges.at(i)->begin->texttoint == edge->end->texttoint && scene->edges.at(i)->end->texttoint == edge->begin->texttoint))
        {
            return true;
        }
    }

    return false;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->pos().y() <= 35 && event->pos().y() >= 4)
        mpos = event->pos();
    else if(event->pos().y() <= 3)
    {
        numberr = 1;
        respos = event->pos();
    }
    else if(event->pos().x() <=3)
    {
        numberr = 2;
        respos = event->pos();
    }
    else if(event->pos().x() >= (this->width() - 3))
    {
        numberr = 3;
        respos = event->pos();
    }
    else if(event->pos().y() >= (this->height() - 4))
    {
        numberr = 4;
        respos = event->pos();
    }
    checkResizableField(event);
    initpos = event->pos();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (mpos.x() >= 0 && event->buttons() && Qt::LeftButton)
    {
        QPoint diff = event->pos() - mpos;
        QPoint newpos = this->pos() + diff;
        this->move(newpos);
    }
    else if(respos.x() >= 0 && event->buttons() && Qt::LeftButton)
    {
        QPoint diff = event->pos() - respos;
        if(numberr == 3)
        {
            respos = event->pos();
            this->setGeometry(this->x(), this->y(), respos.x(), this->height());
        }
        else if(numberr == 4)
        {
            respos = event->pos();
            this->setGeometry(this->x(), this->y(), this->width(), respos.y());
        }
        else if(numberr == 2)
        {
            int widt = this->width()-diff.x();
            if(widt <= this->minimumWidth())
                return;
            else
                this->setGeometry(this->x()+diff.x(), this->y(),widt,this->height());
        }
        else if(numberr == 1)
        {
            int heigh = this->height()-diff.y();
            if(heigh <= this->minimumHeight())
                return;
            else
                this->setGeometry(this->x(), this->y()+diff.y(),this->width(),this->height()-diff.y());
        }
    }
    checkResizableField(event);
}

void MainWindow::checkResizableField(QMouseEvent *event)
{
    QPointF position = event->screenPos();
    qreal x = this->x();
    qreal y = this->y();
    qreal width = this->width();
    qreal height = this->height();

    QRectF rectTop(x + 3, y, width - 5, 3);
    QRectF rectBottom(x + 3, y + height - 4, width - 3, height-4);
    QRectF rectLeft(x, y + 3, 3, height - 4);
    QRectF rectRight(x + width - 2, y + 3, width-3, height - 4);
    QRectF rectInterface(x + 4, y + 4, width - 4, height - 5);

    if (rectTop.contains(position))
        setCursor(Qt::SizeVerCursor);
    else if (rectBottom.contains(position))
        setCursor(Qt::SizeVerCursor);
    else if (rectLeft.contains(position))
        setCursor(Qt::SizeHorCursor);
    else if (rectRight.contains(position))
        setCursor(Qt::SizeHorCursor);
    else if (rectInterface.contains(position))
        setCursor(QCursor());
    else
        setCursor(QCursor());
}

void MainWindow::mouseReleaseEvent(QMouseEvent *)
{
    mpos = QPoint(-1, -1);
    respos = QPoint(-1, -1);
    initpos = QPoint(-1,-1);
    setCursor(QCursor());
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->pos().y() <= 35 && event->pos().y() >= 4)
    {
        if (this->isMaximized())
            this->showNormal();
        else
            this->showMaximized();
    }
    return QWidget::mouseDoubleClickEvent(event);
}
