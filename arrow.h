#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsLineItem>

#include "Items.h"

QT_BEGIN_NAMESPACE
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
QT_END_NAMESPACE

class Arrow : public QGraphicsLineItem
{
public:
    enum { Type = UserType + 4 };

    Arrow(Vertex *startItem, Vertex *endItem,
      QGraphicsItem *parent = 0);
    int type() const override { return Type; }
    void setColor(const QColor &color) { myColor = color; }
    Vertex *startItem() const { return myStartItem; }
    Vertex *endItem() const { return myEndItem; }

    void updatePosition();
    bool equals(Arrow* a);
    int getWeight()const{return weight;}
    int setWeight(int newWeight){weight = newWeight;}

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

private:
    Vertex *myStartItem{nullptr};
    Vertex *myEndItem{nullptr};
    QColor myColor;
    int weight{0};
};

#endif // ARROW_H
