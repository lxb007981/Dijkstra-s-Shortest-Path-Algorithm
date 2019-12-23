#ifndef SOLVER_H
#define SOLVER_H

#include "graph.h"
#include "unvisited_set.h"
#include "RBT.h"
#include "AVL.h"
#include <queue>
#include <tuple>
class solver
{
public:
	solver(int sourceVertex = -1,
        abstractPrioritySet* priorityQueue = nullptr,
		graph* graphP = nullptr,
		unvisited_set* unvisited_setP = nullptr);
    ~solver();
    enum color{blue, red, green, gray, cyan, drawEdge};
	bool step();
	void printPath(int destVertex)const;
    std::queue<std::pair<int, int>> returnPath(int destVertex)const;
    std::queue<std::tuple<int, int, color>> paint;  //processing vertex,
                                                    //changeWeight/the other vertex of an edge,
                                                    //changeColor/ draw line sign
private:
	int sourceVertex{ -1 };
	graph* graphP{ nullptr };
	unvisited_set* unvisited_setP{ nullptr };
    abstractPrioritySet* priorityQueue{ nullptr }; //vertex, tentativeDistance

};
#endif // SOLVER_H

