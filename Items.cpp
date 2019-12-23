#include <Items.h>
#include "arrow.h"
#include <QGraphicsScene>

int Vertex::vertexCount = 0;

Vertex::Vertex()
    : myColor(Qt::black)
{
    setToolTip(QString("Click and drag this vertex to move it!"));
    setCursor(Qt::OpenHandCursor);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    vertex = vertexCount;
    ++vertexCount;
}
Vertex::~Vertex()
{
    --vertexCount;
}
void Vertex::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    QPen pen;
    pen.setColor(myColor);
    pen.setWidth(3);
    painter->setPen(pen);
    painter->setBrush(QBrush(Qt::white));

    painter->drawEllipse(-15, -15, 30, 30);
    if (myDistance != -1)
    {
        painter->setBrush(Qt::NoBrush);
        if (myDistance < 10)//one digit
        {
            painter->drawText(-6, 8, QString::number(myDistance));
        }
        if (myDistance >= 10)//two digits
        {
            painter->drawText(-11, 8, QString::number(myDistance));
        }
    }
}
QRectF Vertex::boundingRect() const
{
    return QRectF(-15, -15, 30, 30);
}

void Vertex::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::ClosedHandCursor);
    QGraphicsItem::mousePressEvent(event);
}
void Vertex::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::OpenHandCursor);
    QGraphicsItem::mouseReleaseEvent(event);
}
void Vertex::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (QLineF(event->screenPos(), event->buttonDownScreenPos(Qt::LeftButton))
            .length() < QApplication::startDragDistance())
    {
        event->accept();
        return;
    }
    else
    {
        QGraphicsItem::mouseMoveEvent(event);
        event->accept();
    }
}
void Vertex::addArrow(Arrow *arrow)
{
    arrows.append(arrow);
}
void Vertex::removeArrow(Arrow *arrow)
{
    int index = arrows.indexOf(arrow);
    if (index != -1)
        arrows.removeAt(index);
}

void Vertex::removeArrows()
{
    foreach (Arrow *arrow, arrows)
    {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
}
QVariant Vertex::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange)
    {
        foreach (Arrow *arrow, arrows)
        {
            arrow->updatePosition();
        }
    }
    return value;
}
