#include "graph.h"
#include <QDebug>
int graph::vertexCount = 0;
graph::graph()
{
    vertexCount = 0;
}
bool adjList::contains(int vertex)
{
    for (std::list<std::pair<int, int>>::const_iterator iter{neighbors.begin()}; iter != neighbors.end(); ++iter)
    {
        if ((*iter).first == vertex)
        {
            return true;
        }
    }
    return false;
}
bool adjList::remove(int vertex)
{
    for (std::list<std::pair<int, int>>::const_iterator iter{neighbors.begin()}; iter != neighbors.end(); ++iter)
    {
        if ((*iter).first == vertex)
        {
            neighbors.erase(iter);
            return true;
        }
    }
    return false;
}
void graph::addNode()
{
    adjList listWithVertex(vertexCount);
    arrayOfList.push_back(listWithVertex);
    ++vertexCount;
}
void graph::addEdge(int weight, int startVertex, int endVertex)
{
    arrayOfList[startVertex].neighbors.push_back(std::make_pair(endVertex, weight));
    arrayOfList[endVertex].neighbors.push_back(std::make_pair(startVertex, weight));
}
void graph::removeNode(int vertex)
{
    arrayOfList.erase(arrayOfList.begin() + vertex);
    for (std::vector<adjList>::iterator iter{arrayOfList.begin()}; iter != arrayOfList.end(); ++iter)
    {
        (*iter).remove(vertex);
    }
    --vertexCount;
}
void graph::removeEdge(int startVertex, int endVertex)
{
    arrayOfList[startVertex].remove(endVertex);
    arrayOfList[endVertex].remove(startVertex);
}
void graph::setTentativeDistance(int vertex, int newDistance)
{
    arrayOfList[vertex].setDistance(newDistance);
}
void graph::setPreviousVertex(int operandVertex, int newPreviousVertex)
{
    arrayOfList[operandVertex].previousVertex = newPreviousVertex;
}

void graph::print() const
{
    qDebug() << "Printing graph";
    for (std::vector<adjList>::const_iterator iter{arrayOfList.begin()};
         iter != arrayOfList.end();
         ++iter)
    {
        qDebug() << iter->getVertex() << '\t';
        for (std::list<std::pair<int, int>>::const_iterator it = iter->getNeighborIterator();
             it != iter->getEndNeighborIterator();
             ++it)
        {
            qDebug() << "v: " << it->first << '\t' << "w: " << it->second << '\t';
        }
        qDebug();
    }
}
void graph::resetTentativeDistanceAndPreviousVertex()
{
    for (std::vector<adjList>::iterator iter{arrayOfList.begin()}; iter != arrayOfList.end(); ++iter)
    {
        iter->distance = INT_MAX;
        iter->previousVertex = -1;
    }
}
