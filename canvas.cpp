#include <canvas.h>
#include <QPainter>
#include <arrow.h>
#include <QRandomGenerator>
myScene::myScene()
{
    myMode = waiting;
    gp = new graph;
}

void myScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    switch (myMode)
    {
    case waiting:
    {
        QGraphicsItem *itemToRemove{nullptr};
        itemToRemove = itemAt(event->scenePos(), QTransform());
        if (itemToRemove && event->buttons() == Qt::RightButton &&!sceneSolver) // has item
        {
            Vertex *itemToRemoveVertexCast = qgraphicsitem_cast<Vertex *>(itemToRemove);
            Arrow *itemToRemoveArrowCast = qgraphicsitem_cast<Arrow *>(itemToRemove);
            if (itemToRemoveVertexCast)
            {
                removeVertex(itemToRemoveVertexCast);
            }
            else if (itemToRemoveArrowCast)
            {
                itemToRemoveArrowCast->startItem()->removeArrow(itemToRemoveArrowCast);
                itemToRemoveArrowCast->endItem()->removeArrow(itemToRemoveArrowCast);
                removeItem(itemToRemoveArrowCast);
                int index = sceneArrows.indexOf(itemToRemoveArrowCast);
                if (index != -1) //found such arrow
                {
                    sceneArrows.removeAt(index);
                }
                gp->removeEdge(itemToRemoveArrowCast->startItem()->vertex, itemToRemoveArrowCast->endItem()->vertex);
                delete itemToRemoveArrowCast;
            }
/*
            foreach (Arrow *iter, sceneArrows)
            {
                qDebug() << iter->startItem()->vertex << "--" << iter->endItem()->vertex;
            }
            qDebug() << "sceneArrows.size()" << sceneArrows.size();
            foreach (Vertex *iter, sceneVertices)
            {
                qDebug() << iter->vertex;
            }
            qDebug() << "sceneVertices.size()" << sceneVertices.size();
            */
            rebuildGraph();

            event->accept();
        }
        else
        {
            QGraphicsScene::mousePressEvent(event);
        }
        break;
    }
    case drawingVertex:
    {
        QGraphicsItem *itemToAdd{nullptr};
        itemToAdd = itemAt(event->scenePos(), QTransform());
        if (event->buttons() == Qt::LeftButton && !itemToAdd)
        {
            Vertex *dot = new Vertex();
            sceneVertices.append(dot);
            dot->setPos(event->scenePos());
            addItem(dot);
            gp->addNode();
            event->accept();
        }
        else
        {
            event->accept();
        }
        break;
    }
    case drawingEdge:
    {
        if (event->button() != Qt::LeftButton)
        {
            event->accept();
            return;
        }
        QGraphicsItem *toBeCast{nullptr};
        toBeCast = itemAt(event->scenePos(), QTransform());
        if (toBeCast)
        {
            if (!startItem)
            {
                startItem = qgraphicsitem_cast<Vertex *>(toBeCast);
                if (!startItem) //cast fail, cast to another type of item
                {
                    event->accept();
                    return;
                }
                else
                {
                    startItem->setColor(Qt::yellow);
                    update();
                }
            }
            else
            {
                startItem->setColor(Qt::yellow);
                endItem = qgraphicsitem_cast<Vertex *>(toBeCast);
                if (!endItem || startItem == endItem) //cast fail, or start point == end point
                {
                    startItem->setColor(Qt::black);
                    update();
                    startItem = endItem = nullptr;
                    event->accept();
                    return;
                }
                if (startItem != endItem && startItem->type() == Vertex::Type && endItem->type() == Vertex::Type)
                {
                    Arrow *newArrow = new Arrow(startItem, endItem);
                    foreach (Arrow *iter, sceneArrows)
                        if (iter->equals(newArrow))
                        {
                            delete newArrow;
                            startItem->setColor(Qt::black);
                            update();
                            startItem = endItem = nullptr;
                            event->accept();
                            return;
                        }

                    startItem->setColor(Qt::black);
                    startItem->addArrow(newArrow);
                    endItem->addArrow(newArrow);
                    newArrow->setZValue(-1000.0);
                    addItem(newArrow);
                    newArrow->updatePosition();
                    sceneArrows.append(newArrow);
                    emit requireWeight();
                    newArrow->setWeight(processingWeight);
                    gp->addEdge(processingWeight, startItem->vertex, endItem->vertex);
                    update();
                    startItem = endItem = nullptr;
                }
                event->accept();
            }
            event->accept();
        }
        else
        {
            event->accept();
            return;
        }

        break;
    }
    case starting:
    {
        if (sceneSolver)
        {
            QGraphicsScene::mousePressEvent(event);
        }
        else
        {
            QGraphicsItem *itemSelected{nullptr};
            itemSelected = itemAt(event->scenePos(), QTransform());
            if (itemSelected && event->buttons() == Qt::LeftButton) // has item
            {
                Vertex *itemSelectedVertexCast = qgraphicsitem_cast<Vertex *>(itemSelected);
                if (itemSelectedVertexCast)
                {
                    setp = new unvisited_set(20);
                    switch (myQueueType)
                    {
                    case priorityQueue::AVLTree:
                        treep = new AVL();
                        break;
                    case priorityQueue::RBTree:
                        treep = new class RBT();
                        break;
                    }
                    sceneSolver = new solver(itemSelectedVertexCast->vertex, treep, gp, setp);
                    bool result = sceneSolver->step();
                    while (result)
                    {
                        result = sceneSolver->step();
                    }
                    itemSelectedVertexCast->setColor(Qt::blue);
                    QGraphicsScene::mousePressEvent(event);
                }
            }
        }
        emit startButtonUpAndSetWaitingMode();
        break;
    }
    case showPath:
    {
        if (!sceneSolver || !sceneSolver->paint.empty())
        {
            QGraphicsScene::mousePressEvent(event);
        }
        else
        {

            QGraphicsItem *itemSelected{nullptr};
            itemSelected = itemAt(event->scenePos(), QTransform());
            if (itemSelected && event->buttons() == Qt::LeftButton) // has item
            {
                Vertex *itemSelectedVertexCast = qgraphicsitem_cast<Vertex *>(itemSelected);
                if (itemSelectedVertexCast)
                {
                    resetArrowsColor();
                    std::queue<std::pair<int, int>> path = sceneSolver->returnPath(itemSelectedVertexCast->vertex);
                    while (!path.empty())
                    {
                        int startVertexIndex = path.front().first;
                        int endVertexIndex = path.front().second;
                        path.pop();
                        Arrow *toPaint = findArrow(sceneVertices.at(startVertexIndex), sceneVertices.at(endVertexIndex));
                        toPaint->setColor(Qt::blue);
                    }
                }
            }
            QGraphicsScene::mousePressEvent(event);
        }

        break;
    }
    }
}
void myScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);
    update();
}

void myScene::setStartItemColor(QColor color)
{
    if (startItem)
    {
        startItem->setColor(color);
        startItem = nullptr;
        update();
    }
}
Arrow *myScene::findArrow(Vertex *startItem, Vertex *endItem)
{
    foreach (Arrow *iter, sceneArrows)
        if ((iter->startItem() == startItem && iter->endItem() == endItem) || (iter->endItem() == startItem && iter->startItem() == endItem))
        {
            return iter;
        }
    return nullptr;
}
void myScene::step()
{
    if (!sceneSolver)
    {
        return;
    }
    resetArrowsColor();
    if (sceneSolver->paint.empty())
    {
        emit algorithmFinished();
        return;
    }
    std::tuple<int, int, solver::color> currentPaint = sceneSolver->paint.front();
    sceneSolver->paint.pop(); //afterwards is new one
    int processingVertexIndex = std::get<0>(currentPaint);
    Vertex *processingVertex = getVertex(processingVertexIndex);
    switch (std::get<2>(currentPaint))
    {
    case solver::color::drawEdge:
    {
        Arrow *arrowToPaint = findArrow(processingVertex, getVertex(std::get<1>(currentPaint)));
        arrowToPaint->setColor(Qt::blue);
        break;
    }
    case solver::color::red:
    case solver::color::green:
    case solver::color::cyan:
    case solver::color::gray:
    case solver::color::blue:

    {
        QColor color = enum2Color(std::get<2>(currentPaint));
        processingVertex->setColor(color);
        processingVertex->setDistance(std::get<1>(currentPaint));
        break;
    }
    }
    update();
}
void myScene::clear()
{

    if (sceneSolver)
    {
        delete sceneSolver;
        sceneSolver = nullptr;
    }
    if (gp)
    {
        gp->resetTentativeDistanceAndPreviousVertex();
    }
    resetArrowsColor();
    resetVerticesColor();
}
void myScene::resetArrowsColor()
{
    foreach (Arrow *iter, sceneArrows)
    {
        iter->setColor(Qt::black);
    }
    update();
}
void myScene::resetVerticesColor()
{
    foreach (Vertex *iter, sceneVertices)
    {
        iter->setColor(Qt::black);
        iter->setDistance(-1);
    }
    update();
}
void myScene::rebuildGraph()
{
    int tempVertexIndex{0};
    foreach (Vertex *iter, sceneVertices)
    {
        iter->vertex = tempVertexIndex;
        ++tempVertexIndex;
    }
    delete gp;
    gp = new graph;
    gp->vertexCount = 0;
    for (int i{0}; i < tempVertexIndex; ++i)
    {
        gp->addNode();
    }
    foreach (Arrow *iter, sceneArrows)
    {
        gp->addEdge(iter->getWeight(), iter->startItem()->vertex, iter->endItem()->vertex);
    }
}
void myScene::removeArrowsAttachedToOneVertexFromSceneArrows(Vertex *deletedVertex)
{
    for (QList<Arrow *>::iterator iter{sceneArrows.begin()}; iter != sceneArrows.end();)
    {
        if ((*iter)->startItem() == deletedVertex || (*iter)->endItem() == deletedVertex)
        {
            iter = sceneArrows.erase(iter);
        }
        else
        {
            iter++;
        }
    }
}
void myScene::randGenerate(int numOfVertices, int numOfEdges, int maxWeight)
{
    if (sceneSolver)
    {
        delete sceneSolver;
        sceneSolver = nullptr;
    }
    //remove vertices
    int counter = Vertex::vertexCount;
    for (int i{0}; i < counter; ++i)
    {
        if (sceneVertices.isEmpty())
        {
            break;
        }
        Vertex *vertexToRemove{sceneVertices.front()};

        sceneVertices.pop_front();
        removeVertex(vertexToRemove);
    }
    //clear graph
    if (gp)
    {
        delete gp;
    }
    //build new graph
    gp = new graph;
    gp->vertexCount = 0;

    for (int i{0}; i < numOfVertices; ++i)
    {
        int xcor = QRandomGenerator::global()->bounded(-477, 1273);
        int ycor = QRandomGenerator::global()->bounded(-249, 546);
        Vertex *dot = new Vertex();
        sceneVertices.append(dot);
        dot->setPos(xcor, ycor);
        addItem(dot);

        gp->addNode();
    }
    for (int i{0}; i < numOfEdges; ++i)
    {
        int startItemIndex = QRandomGenerator::global()->bounded(0, sceneVertices.size());
        int endItemIndex = QRandomGenerator::global()->bounded(0, sceneVertices.size());
        if (startItemIndex == endItemIndex)
        {
            --i;
            continue;
        }
        int weight = QRandomGenerator::global()->bounded(1, maxWeight + 1);
        startItem = getVertex(startItemIndex);
        endItem = getVertex(endItemIndex);
        Arrow *newArrow = new Arrow(startItem, endItem);
        bool Equal{false};
        foreach (Arrow *iter, sceneArrows)
        {
            if (iter->equals(newArrow))
            {
                delete newArrow;
                --i;
                Equal = true;
                break;
            }
        }
        if (Equal)
        {
            continue;
        }


        startItem->addArrow(newArrow);
        endItem->addArrow(newArrow);
        newArrow->setZValue(-1000.0);
        addItem(newArrow);
        newArrow->updatePosition();
        sceneArrows.append(newArrow);
        newArrow->setWeight(weight);
        gp->addEdge(weight, startItem->vertex, endItem->vertex);
        startItem = endItem = nullptr;
    }
    update();
}
void myScene::removeVertex(Vertex *vertexToBeRemoved)
{
    removeArrowsAttachedToOneVertexFromSceneArrows(vertexToBeRemoved);
    vertexToBeRemoved->removeArrows();
    int index = sceneVertices.indexOf(vertexToBeRemoved);
    if (index != -1) //found such arrow
    {
        sceneVertices.removeAt(index);
    }
    removeItem(vertexToBeRemoved);//remove from scene
    delete vertexToBeRemoved;
}
Vertex *myScene::getVertex(int vertex)
{
    foreach (Vertex *iter, sceneVertices)
    {
        if (iter->vertex == vertex)
        {
            return iter;
        }
    }
    return nullptr;
}
