#ifndef ITEMS_H
#define ITEMS_H
#include <QGraphicsItem>
#include <QPainter>
#include <QCursor>
#include <QApplication>
#include <QGraphicsSceneMouseEvent>
#include <QList>


QT_BEGIN_NAMESPACE
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
class QGraphicsSceneContextMenuEvent;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;
QT_END_NAMESPACE

class Arrow;
class Vertex : public QGraphicsItem
{
public:
    enum { Type = UserType + 1 };

    Vertex();
    ~Vertex()override;
    int type() const override { return Type; }
    void removeArrow(Arrow *arrow);
    void removeArrows();
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void setColor(const QColor &color) { myColor = color; }
    void addArrow(Arrow *arrow);
    int vertex{-1};
    static int vertexCount; //rememberto refresh!!!!!
    void setDistance(int distance){myDistance = distance;}

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    QColor myColor;
    int myDistance{-1};
    QList<Arrow *> arrows;
};
#endif // ITEMS_H
