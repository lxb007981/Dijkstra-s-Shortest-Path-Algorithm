#ifndef UNVISITED_SET_H
#define UNVISITED_SET_H
#include <vector>
#include <list>
#include <iostream>
class unvisited_set
{
private:
    int table_size;
    std::vector<std::list<int>>hashTable;
    int hash_function (int key)const
    {
        return key % table_size;
    }


public:
    unvisited_set(int table_size);
    bool search(int vertex) const;
    void insert(int vertex);
    void remove(int vertex);
	void print()const;
};
#endif // UNVISITED_SET_H
