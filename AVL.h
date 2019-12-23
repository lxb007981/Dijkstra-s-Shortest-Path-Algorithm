#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H
#include "abstractPrioritySet.h"
#include <vector>

inline int max(int a, int b)
{
	return (a > b) ? a : b;
}
// An AVL tree node  
class AVL_Node
{
public:
	myPair value;
	AVL_Node* left;
	AVL_Node* right;
	int height;
	std::vector<AVL_Node*> duplicates;
	AVL_Node(const myPair& x) : value(x), height(1), left(nullptr), right(nullptr) {}


};

class AVL:public abstractPrioritySet {
private:
	AVL_Node* root{ nullptr };


	AVL_Node* rotate_left(AVL_Node* node);      // Single left or anti-clockwise rotation
	AVL_Node* rotate_right(AVL_Node*);     // Single right or clockwise rotation
	int getBalance(AVL_Node* node) const;     // Find the balance factor of tree
	int height(AVL_Node* node)const;
	AVL_Node* removeInternal(AVL_Node* root, const myPair& key);
	AVL_Node* find_min(AVL_Node* node) const;
	bool is_emptyInternal(AVL_Node* node)const;
	AVL_Node* insertInternal(AVL_Node* node, const myPair& x);
	void printInternal(AVL_Node* node, int depth = 0) const ;



public:
	~AVL() override;
	void insert(const myPair&)override;
	void remove(const myPair&)override;
	const myPair& find_min_value() const override;
	void print(int depth = 0) const override;
	bool is_empty()const override;
};

#endif // PRIORITY_QUEUE_H
