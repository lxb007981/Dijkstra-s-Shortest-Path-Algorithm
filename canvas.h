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
    priorityQueue myQueueType{priorityQueue::AVLTree};
    Vertex *startItem{nullptr};
    Vertex *endItem{nullptr};
    QList<Vertex *> sceneVertices;
    QList<Arrow *> sceneArrows;
    Arrow *findArrow(Vertex *, Vertex *);
    int processingWeight{0};
    unvisited_set *setp{nullptr};
    abstractPrioritySet *treep{nullptr};
    graph *gp{nullptr};
    solver *sceneSolver{nullptr};

    void removeArrowsAttachedToOneVertexFromSceneArrows(Vertex *);
    void resetVerticesColor();
    void rebuildGraph();
    void removeVertex(Vertex *vertexToBeRemoved);
    Vertex *getVertex(int vertex);
    QColor enum2Color(solver::color color);

public:
    myScene();
    void step();
    void clear();
    void resetArrowsColor();
    void setMode(Mode mode) { myMode = mode; }
    void setPriorityQueue(priorityQueue queue) { myQueueType = queue; }
};

#endif // CANVAS_H
