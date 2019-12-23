#ifndef RHT_H
#define RHT_H

#include "abstractPrioritySet.h"
#include <vector>

enum COLOR
{
    RED,
    BLACK
};

class RBT_Node
{
public:
    myPair val;
    COLOR color;
    RBT_Node *left, *right, *parent;
    std::vector<RBT_Node *> duplicates;
    RBT_Node(myPair val) : val(val)
    {
        parent = left = right = nullptr;

        color = RED;
    }

    RBT_Node *uncle()
    {
        // If no parent or grandparent, then no uncle
        if (parent == nullptr or parent->parent == nullptr)
            return nullptr;

        if (parent->isOnLeft())
            return parent->parent->right;
        else
            return parent->parent->left;
    }


    bool isOnLeft() { return this == parent->left; }

    RBT_Node *sibling()
    {
        // sibling nullptr if no parent
        if (parent == nullptr)
            return nullptr;

        if (isOnLeft())
            return parent->right;

        return parent->left;
    }

    bool hasRedChild()
    {
        return (left != nullptr && left->color == RED) ||
               (right != nullptr && right->color == RED);
    }
};

class RBT : public abstractPrioritySet
{
    RBT_Node *root;

    void leftRotate(RBT_Node *x);
    void rightRotate(RBT_Node *x);
    void swapColors(RBT_Node *x1, RBT_Node *x2);
    void swapValues(RBT_Node *u, RBT_Node *v);
    void fixDoubleRed(RBT_Node *x);
    void deleteNode(RBT_Node *v);
    void fixDoubleBlack(RBT_Node *x);
    void printBack(RBT_Node *node, int depth) const;
    RBT_Node *successor(RBT_Node *x);
    RBT_Node *BSTreplace(RBT_Node *x);  
    RBT_Node *getRoot() { return root; }
    RBT_Node *search(myPair n);

public:
    RBT() { root = nullptr; }
    ~RBT() override;
    void insert(const myPair &) override;
    void remove(const myPair &) override;
    bool is_empty() const override;
    void print(int depth = 0) const override;
    const myPair &find_min_value() const override;
};
#endif // RHT_H
