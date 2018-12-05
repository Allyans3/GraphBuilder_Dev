#include "edge.h"
#include "QDebug"

const qreal PI = atan(1) * 4;
uint Edge::_idStatic = 0;
int Edge::_offsAngle = 5;

Edge::Edge(Node * beginNode, Node * endNode, bool ch, QString textArrow): ItemParent(),
    id(_idStatic++),textEdge(textArrow),arrowSize(10)
{
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setFlag(ItemSendsGeometryChanges);
    isDirected = ch;
    begin = beginNode;
    end = endNode;
    begin->addEdge(this);
    if(begin != end)
        end->addEdge(this);
    adjust();
}

Edge::~Edge()
{
    begin->removeEdge(this);
    if (begin != end)
        end->removeEdge(this);
}

Node *Edge::beginNode() const
{
    return begin;
}

Node *Edge::endNode() const
{
    return end;
}

QPainterPath Edge::shape() const{
    QPainterPath path;
    if (begin != end) {
        QLineF line = QLineF(beginPoint.x(), beginPoint.y(), endPoint.x(), endPoint.y());
        qreal radAngle = line.angle() * M_PI / 180;
        qreal selectionOffset = 4;
        qreal dx = selectionOffset * sin(radAngle);
        qreal dy = selectionOffset * cos(radAngle);
        QPointF offset1 = QPointF(dx, dy);
        path.moveTo(line.p1() + offset1);
        path.lineTo(line.p1() - offset1);
        path.lineTo(line.p2() - offset1);
        path.lineTo(line.p2() + offset1);
    } else {
        path.addEllipse(begin->pos()+ QPointF(Node::Radius, -Node::Radius),Node::Radius+2, Node::Radius+2);
    }
    return path;
}

QRectF Edge:: boundingRect() const
{
    if (!end || !begin)
        return QRectF();
    if(end != begin)
    {
        QPolygonF pText;
        qreal x = textPoint.x();
        qreal y = textPoint.y();
        pText << QPointF(x, y)
              << QPointF(x, y - 18)
              << QPointF(x + 8 * textEdge.size(), y - 18)
              << QPointF(x + 8 * textEdge.size(), y + 2)
              << QPointF(x, y + 2);
        return shape().boundingRect().united(pText.boundingRect());
    }
    return shape().boundingRect();
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    double angle;
    QPointF peak, destArrowP1, destArrowP2;
    painter->setPen(QPen((option->state & QStyle::State_Selected ? Qt::yellow : this->pen().color() ), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    if(begin != end)
    {
        QLineF line(beginPoint,endPoint);
        painter->drawLine(line);
        if(isDirected == true)
        {
            angle = std::atan2(-line.dy(), line.dx());
            destArrowP1 = endPoint + QPointF(sin(angle - M_PI / 1.8) * qMin(arrowSize, line.length()),
                                             cos(angle - M_PI / 1.8) * qMin(arrowSize, line.length()));
            destArrowP2 = endPoint + QPointF(sin(angle - M_PI + M_PI / 1.8) * qMin(arrowSize, line.length()),
                                             cos(angle - M_PI + M_PI / 1.8) * qMin(arrowSize, line.length()));
            peak = line.p2();
        }
    }
    else
    {
        painter->drawArc(static_cast<int>(begin->pos().x()),static_cast<int>(begin->pos().y()) - 2 * Node::Radius,
                         2 * Node::Radius,
                         2 * Node::Radius,
                         16 * -70, 16 * 270);
        if(isDirected == true)
        {
            angle = 1.07*M_PI;

            destArrowP1 = endPoint + QPointF(sin(angle - M_PI / 1.8) * arrowSize,
                                             cos(angle - M_PI / 1.8) * arrowSize);
            destArrowP2 = endPoint + QPointF(sin(angle - M_PI + M_PI / 1.8)* arrowSize,
                                             cos(angle - M_PI + M_PI / 1.8) * arrowSize);
            peak = endPoint;
        }
    }
    painter->setBrush((option->state & QStyle::State_Selected ? Qt::cyan: Qt::black));
    painter->drawPolygon(QPolygonF() << peak << destArrowP1 << destArrowP2);
    painter->setFont(QFont("Times", 13, QFont::Bold));
    painter->drawText(this->textPoint, this->textEdge);
}

void Edge::adjust()
{
    if (!end || !begin)
        return;
    if (begin != end)
    {
        bool offs = false;  // смещение с центра
        QLineF line(mapFromItem(begin, 0, 0), mapFromItem(end, 0, 0));
        qreal length = line.length();
        qreal angl = line.angle();
        for (auto edg : begin->edges()) {
            if (edg->beginNode() == end) {
                offs = true;
                if(id > edg->id) {
                    edg->adjust();
                }
                break;
            }
        }
        prepareGeometryChange();
        if (!offs) {
            if (length > qreal(2 * Node::Radius)) {
                QPointF edgeOffset((line.dx() * Node::Radius) / length, (line.dy() * Node::Radius) / length);
                beginPoint = line.p1() + edgeOffset;
                endPoint = line.p2() - edgeOffset;
            } else {
                beginPoint = endPoint = line.p1();
            }
        } else {
            if (length > qreal(2 * Node::Radius)) {
                beginPoint.setX(line.p1().x() + Node::Radius * cos((angl + _offsAngle) * PI / 180));
                beginPoint.setY(line.p1().y() - Node::Radius * sin((angl + _offsAngle) * PI / 180));
                endPoint.setX(line.p2().x() + Node::Radius * cos((angl - 180 - _offsAngle) * PI / 180));
                endPoint.setY(line.p2().y() - Node::Radius * sin((angl - 180 - _offsAngle) * PI / 180));
            } else {
                beginPoint = endPoint = line.p1();
            }
        }
        // Нахождение точки для текста
        QLineF line1(beginPoint, endPoint);
        line1.setLine(line1.x1(),line1.y1()-7,line1.x2(),line1.y2()-7);
        textPoint = line1.center();
//        if ((line1.angle() > 290 && line1.angle() <= 360) || (line1.angle() >= 0 && line1.angle() <= 30)
//                || (line1.angle() > 105 && line1.angle() <= 210)) {
//            line1.setLine(line1.x1(),line1.y1()-7,line1.x2(),line1.y2()-7);
//            textPoint = line1.center();
//        }
//        else if (line1.angle() >= 31 && line1.angle() <= 104) {
//            line1.setLine(line1.x1(),line1.y1()+7,line1.x2(),line1.y2()+7);
//            textPoint = line1.center();
//        }
    } else {        // source == dest
        textPoint = QPointF(boundingRect().center().x() - Node::Radius / 2, boundingRect().center().y());
        prepareGeometryChange();
        beginPoint = mapFromItem(begin, 0, -Node::Radius);
        endPoint = mapFromItem(begin, Node::Radius, 0);
    }
}
