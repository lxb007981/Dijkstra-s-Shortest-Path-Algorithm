#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <list>
#include <utility>
#include <climits>
#include <iostream>
class adjList
{
    friend class graph;

public:
	inline void setDistance(int newDistance) { distance = newDistance; }
	inline std::list<std::pair<int, int>>::const_iterator getNeighborIterator() const { return neighbors.begin(); }
	inline std::list<std::pair<int, int>>::const_iterator getEndNeighborIterator() const { return neighbors.end(); }
	inline int getDistance() const { return distance; }
	inline int getVertex() const { return vertex; }
	adjList(int vertex) : vertex(vertex){}
	inline int getPreviousVertex()const { return previousVertex; }
private:
    int vertex{-1};
    int distance{INT_MAX};
    int previousVertex{-1};
    std::list<std::pair<int, int>> neighbors;
    //adjList(int vertex, std::list<std::pair<int, int>> neighbors) : vertex(vertex), neighbors(neighbors) {}
    bool contains(int vertex); //search a vertex in one vertex's neighbors
    bool remove(int vertex);
};
class graph
{

public:
    std::vector<adjList> arrayOfList;
    graph();
    static int vertexCount;
    void addNode();
    void addEdge(int weight, int startVertex, int endVertex);
    void removeNode(int vertex);
    void removeEdge(int startVertex, int endVertex);
    void setTentativeDistance(int vertex, int newDistance);
    void setPreviousVertex(int operandVertex, int newPreviousVertex);
	void print()const;
    void resetTentativeDistanceAndPreviousVertex();

};
#endif // GRAPH_H
