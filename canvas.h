#ifndef CANVAS_H
#define CANVAS_H
#include <QPen>
#include <QVBoxLayout>
#include <QGraphicsScene>
#include <Items.h>
#include <QDebug>
#include <QMouseEvent>
#include <solver.h>

class myScene : public QGraphicsScene
{
    Q_OBJECT
public:
    enum Mode
    {
        waiting,
        drawingVertex,
        drawingEdge,
        starting,
        showPath
    };
    enum priorityQueue
    {
        AVLTree,
        RBTree
    };

public slots:
    void setStartItemColor(QColor color);
    void setEndItem(Vertex *item) { endItem = item; }
    void setProcessingWeight(int weight) { processingWeight = weight; }
    void randGenerate(int numOfVertices, int numOfEdges, int maxWeight);
signals:
    void requireWeight();
    void algorithmFinished();
    void startButtonUpAndSetWaitingMode();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:
    Mode myMode;
    Vertex *startItem{nullptr};
    Vertex *endItem{nullptr};
    QList<Vertex *> sceneVertices;
    QList<Arrow *> sceneArrows;
    Arrow *findArrow(Vertex *, Vertex *);
    void removeArrowsAttachedToOneVertexFromSceneArrows(Vertex *);
    int processingWeight{0};
    unvisited_set *setp{nullptr};
    abstractPrioritySet *treep{nullptr};
    priorityQueue myQueueType{priorityQueue::AVLTree};
    graph *gp{nullptr};
    solver *sceneSolver{nullptr};
    void resetVerticesColor();
    void rebuildGraph();
    void removeVertex(Vertex *vertexToBeRemoved);
    Vertex *getVertex(int vertex);
    QColor enum2Color(solver::color color)
    {
        switch (color)
        {
        case 0:
            return Qt::blue;
        case 1:
            return Qt::red;
        case 2:
            return Qt::green;
        case 3:
            return Qt::gray;
        case 4:
            return Qt::cyan;
        }
    }

public:
    myScene();
    void step();
    void clear();
    void resetArrowsColor();
    void setMode(Mode mode) { myMode = mode; }
    void setPriorityQueue(priorityQueue queue) { myQueueType = queue; }
};

#endif // CANVAS_H
