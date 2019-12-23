#ifndef RHT_H
#define RHT_H

#include "abstractPrioritySet.h"
#include <vector>
#include <queue>
using namespace std;

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

        // RBT_Node is created during insertion
        // RBT_Node is red at insertion
        color = RED;
    }

    // returns pointer to uncle
    RBT_Node *uncle()
    {
        // If no parent or grandparent, then no uncle
        if (parent == nullptr or parent->parent == nullptr)
            return nullptr;

        if (parent->isOnLeft())
            // uncle on right
            return parent->parent->right;
        else
            // uncle on left
            return parent->parent->left;
    }

    // check if node is left child of parent
    bool isOnLeft() { return this == parent->left; }

    // returns pointer to sibling
    RBT_Node *sibling()
    {
        // sibling nullptr if no parent
        if (parent == nullptr)
            return nullptr;

        if (isOnLeft())
            return parent->right;

        return parent->left;
    }

    // moves node down and moves given node in its place
    void moveDown(RBT_Node *newParent)
    {
        if (parent != nullptr)
        {
            if (isOnLeft())
            {
                parent->left = newParent;
            }
            else
            {
                parent->right = newParent;
            }
        }
        newParent->parent = parent;
        parent = newParent;
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

    // left rotates the given node
    void leftRotate(RBT_Node *x);
    void rightRotate(RBT_Node *x);
    void swapColors(RBT_Node *x1, RBT_Node *x2);
    void swapValues(RBT_Node *u, RBT_Node *v);
    // fix red red at given node
    void fixRedRed(RBT_Node *x);

    // find node that do not have a left child
    // in the subtree of the given node
    RBT_Node *successor(RBT_Node *x);

    // find node that replaces a deleted node in BST
    RBT_Node *BSTreplace(RBT_Node *x);

    // deletes the given node
    void deleteNode(RBT_Node *v);

    void fixDoubleBlack(RBT_Node *x);
    RBT_Node *getRoot() { return root; }

    // searches for given value
    // if found returns the node (used for delete)
    // else returns the last node while traversing (used in insert)
    RBT_Node *search(myPair n);
    void printBack(RBT_Node *node, int depth) const;

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
