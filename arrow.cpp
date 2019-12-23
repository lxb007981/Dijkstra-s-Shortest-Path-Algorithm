#include "arrow.h"
#include <QPen>
#include <QPainter>

Arrow::Arrow(Vertex *startItem, Vertex *endItem, QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
{
    myStartItem = startItem;
    myEndItem = endItem;
    myColor = Qt::black;
    setPen(QPen(myColor, 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    setCursor(Qt::OpenHandCursor);
}

void Arrow::updatePosition()
{
    QLineF line(mapFromItem(myStartItem, 0, 0), mapFromItem(myEndItem, 0, 0));
    setLine(line);
}
void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (myStartItem->collidesWithItem(myEndItem))
    {
        return;
    }

    QPen myPen = pen();
    myPen.setColor(myColor);
    painter->setPen(myPen);
    painter->setBrush(myColor);

    QLineF centerLine(myStartItem->pos(), myEndItem->pos());
    painter->drawLine(centerLine);
    painter->setBrush(Qt::NoBrush);
    myPen.setColor(Qt::black);
    painter->setPen(myPen);
    painter->drawText(centerLine.center(), QString::number(weight));
}

bool Arrow::equals(Arrow *a)
{
    if ((myStartItem == a->startItem() && myEndItem == a->endItem()) || (myEndItem == a->startItem() && myStartItem == a->endItem()))
        return true;
    else
        return false;
}
