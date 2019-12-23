#include "unvisited_set.h"

unvisited_set::unvisited_set(int table_size) : table_size(table_size)
{
    hashTable = std::vector<std::list<int>>(table_size);
}

bool unvisited_set::search(int vertex) const
{
    int hash_value{hash_function(vertex)};
    if (hashTable[hash_value].empty())
    {
        return false;
    }
    else
    {
        for (std::list<int>::const_iterator iter{hashTable[hash_value].begin()}; iter != hashTable[hash_value].end(); ++iter)
        {
            if (vertex == *iter)
            {
                return true;
            }
        }
        return false;
    }
}
void unvisited_set::insert(int vertex)
{
    int hash_value{hash_function(vertex)};
    hashTable[hash_value].push_back(vertex);
}
void unvisited_set::remove(int vertex)
{
    int hash_value{hash_function(vertex)};
    hashTable[hash_value].remove(vertex);
}
void unvisited_set::print()const {
	std::cout << "Start printing." << std::endl;

	for (std::vector<std::list<int>>::const_iterator Iter{hashTable.begin()}; Iter != hashTable.end(); ++Iter) {
		std::cout << "Print Another has value" << std::endl;
		for (std::list<int>::const_iterator it{ Iter->begin() }; it != Iter->end(); ++it) {
			std::cout << *it << '\t';
		}
		std::cout << std::endl;
	}

}