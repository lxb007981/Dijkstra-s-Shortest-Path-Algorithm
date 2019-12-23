#include <solver.h>
#include <QDebug>
solver::solver(int sourceVertex, abstractPrioritySet *priorityQueue,
               graph *graphP,
               unvisited_set *unvisited_setP)
    : sourceVertex(sourceVertex), priorityQueue(priorityQueue), graphP(graphP), unvisited_setP(unvisited_setP)
{
    for (std::vector<adjList>::const_iterator iter{graphP->arrayOfList.begin()};
         iter != graphP->arrayOfList.end();
         ++iter)
    {
        unvisited_setP->insert(iter->getVertex());
    }
    graphP->setTentativeDistance(sourceVertex, 0);
    priorityQueue->insert(myPair(sourceVertex, 0));
}
solver::~solver()
{
    delete priorityQueue;
    delete unvisited_setP;
}
bool solver::step()
{
    if (priorityQueue->is_empty())
    {
        return false;
    }

    myPair currentVertex = priorityQueue->find_min_value();
    paint.push(std::make_tuple(currentVertex.vertex, currentVertex.distance, blue));
    qDebug() << "current vertex is (" << currentVertex.vertex << ", " << currentVertex.distance << ")";
    priorityQueue->remove(currentVertex);
    qDebug() << "remove current distance from tree " << currentVertex.distance << "vertex" << currentVertex.vertex;
    qDebug() << "Start Printing";
    priorityQueue->print();
    unvisited_setP->remove(currentVertex.vertex);

    for (std::list<std::pair<int, int>>::const_iterator iter = graphP->arrayOfList[currentVertex.vertex].getNeighborIterator();
         iter != graphP->arrayOfList[currentVertex.vertex].getEndNeighborIterator();
         ++iter) //iterate through all neighbors of current node
    {
        if (!unvisited_setP->search(iter->first))
        { //not belongs to unvisited_set, i.e. visited
            continue;
        }
        int newTentativeDistance{iter->second + currentVertex.distance};
        int previousTentativeDistance{graphP->arrayOfList[iter->first].getDistance()};
        paint.push(std::make_tuple(currentVertex.vertex, iter->first, drawEdge));
        if (newTentativeDistance < previousTentativeDistance)
        {

            graphP->setTentativeDistance(iter->first, newTentativeDistance);
            priorityQueue->remove(myPair(iter->first, previousTentativeDistance));
            qDebug() << "remove from tree (" << iter->first << ", " << previousTentativeDistance << ")";

            priorityQueue->insert(myPair(iter->first, newTentativeDistance));
            qDebug() << "insert to tree (" << iter->first << ", " << newTentativeDistance << ")";
            paint.push(std::make_tuple(iter->first, newTentativeDistance, green));
            paint.push(std::make_tuple(iter->first, newTentativeDistance, cyan));

            graphP->setPreviousVertex(iter->first, currentVertex.vertex);
        }
        else
        {
            paint.push(std::make_tuple(iter->first, previousTentativeDistance, red));
            paint.push(std::make_tuple(iter->first, previousTentativeDistance, cyan));
        }
    }
    paint.push(std::make_tuple(currentVertex.vertex, currentVertex.distance, gray));
    qDebug() << "Start Printing";
    priorityQueue->print();
    return true;
}

std::queue<std::pair<int, int>> solver::returnPath(int destVertex) const
{
    std::queue<std::pair<int, int>> path;
    if (unvisited_setP->search(destVertex))
    {
        return path;
    }
    if (destVertex == sourceVertex)
    {
        return path;
    }
    int temp{destVertex};
    int previousVertex{graphP->arrayOfList[destVertex].getPreviousVertex()};

    while (previousVertex != sourceVertex)
    {
        path.push(std::make_pair(temp, previousVertex));
        temp = previousVertex;
        previousVertex = graphP->arrayOfList[previousVertex].getPreviousVertex();
    }
    path.push(std::make_pair(temp, previousVertex));
    return path;
}

void solver::printPath(int destVertex) const
{
    if (destVertex == -1)
        return;
    if (destVertex == sourceVertex)
        return;
    int previousVertex{graphP->arrayOfList[destVertex].getPreviousVertex()};
    qDebug() << previousVertex << '\t';
    printPath(previousVertex);
}
