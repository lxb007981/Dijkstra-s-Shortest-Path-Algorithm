#include "AVL.h"
#include <QDebug>
AVL::~AVL()
{
    while (root)
        root = removeInternal(root, root->value);
}
int AVL::height(AVL_Node *node) const
{
    return is_emptyInternal(node) ? 0 : node->height;
}
int AVL::getBalance(AVL_Node *node) const
{
    return is_emptyInternal(node) ? 0 : height(node->left) - height(node->right);
}

AVL_Node *AVL::rotate_left(AVL_Node *node) //not modify the root
{
    AVL_Node *y = node->right;
    AVL_Node *T2 = y->left;

    // Perform rotation
    y->left = node;
    node->right = T2;

    // Update heights
    node->height = max(height(node->left), height(node->right)) + 1;
    node->height = max(height(y->left), height(y->right)) + 1;

    // Return new root
    return y;
}

// A utility function to left
// rotate subtree rooted with node
AVL_Node *AVL::rotate_right(AVL_Node *y)
{
    AVL_Node *x = y->left;
    AVL_Node *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    // Return new root
    return x;
}

void AVL::insert(const myPair &data)
{
    root = insertInternal(root, data);
}

AVL_Node *AVL::insertInternal(AVL_Node *node, const myPair &x)
{
    /* 1. Perform the normal BST rotation */
    if (!node)
        return new AVL_Node(x);

    if (x < node->value)
        node->left = insertInternal(node->left, x);
    else if (x > node->value)
        node->right = insertInternal(node->right, x);
    else
    { //deal with equal key
        node->duplicates.push_back(new AVL_Node(x));
        return node;
    }

    /* 2. Update height of this ancestor node */
    node->height = 1 + max(height(node->left),
                           height(node->right));

    /* 3. Get the balance factor of this
        ancestor node to check whether
        this node became unbalanced */
    int balance = getBalance(node);


    // Left Left Case
    if (balance > 1 && x < node->left->value)
        return rotate_right(node);

    // Right Right Case
    if (balance < -1 && x > node->right->value)
        return rotate_left(node);

    // Left Right Case
    if (balance > 1 && x > node->left->value)
    {
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }

    // Right Left Case
    if (balance < -1 && x < node->right->value)
    {
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }

    /* return the (unchanged) node pointer */
    return node;
}

AVL_Node *AVL::find_min(AVL_Node *node) const
{
    if (is_emptyInternal(node->left))
        return node;
    return find_min(node->left);
}
const myPair &AVL::find_min_value() const
{
    return find_min(root)->value;
}
bool AVL::is_empty() const
{
    return is_emptyInternal(root);
}

bool AVL::is_emptyInternal(AVL_Node *node) const
{
    return node == nullptr;
}
void AVL::print(int depth) const
{
    printInternal(root, depth);
}

void AVL::printInternal(AVL_Node *node, int depth) const
{
    if (is_emptyInternal(node)) // Base case
        return;
    printInternal(node->right, depth + 1); // Recursion: right sub-tree
    QString debugString;
    for (int j = 0; j < depth; j++) // Print the node value
        debugString += "    ";
    debugString += QString::number(node->value.distance);
    qDebug() << debugString;

    printInternal(node->left, depth + 1); // Recursion: left sub-tree
}

void AVL::remove(const myPair &data)
{
    root = removeInternal(root, data);
}

AVL_Node *AVL::removeInternal(AVL_Node *root, const myPair &key)
{ // STEP 1: PERFORM STANDARD BST DELETE
    if (!root)
        return root;


    if (key < root->value)
        root->left = removeInternal(root->left, key);


    else if (key > root->value)
        root->right = removeInternal(root->right, key);

    else if (!root->duplicates.empty())
    {
        if (root->value.vertex == key.vertex)
        {
            root->value.vertex = root->duplicates.back()->value.vertex;
            delete root->duplicates.back();
            root->duplicates.pop_back();
            return root;
        }
        else
        {

            for (std::vector<AVL_Node *>::const_iterator iter{root->duplicates.begin()};
                 iter != root->duplicates.end();
                 ++iter)
            {
                if ((*iter)->value.vertex == key.vertex)
                {
                    delete (*iter);
                    root->duplicates.erase(iter);
                    return root;
                }
            }
            return root;
        }
    }

    // if key is same as root's key, then
    // This is the node to be deleted
    else
    {
        // node with only one child or no child
        if (!root->left ||
            !root->right)
        {
            AVL_Node *temp = root->left ? //whether it has root->left
                                 root->left
                                        : root->right; //temp = the only child, or nullptr

            // No child case
            if (!temp)
            {
                temp = root;
                root = nullptr;
            }
            else               // One child case
                *root = *temp; // Copy the contents of
                               // the non-empty child
            delete temp;
        }
        else
        {
            // node with two children: Get the inorder
            // successor (smallest in the right subtree)
            AVL_Node *temp = find_min(root->right);

            // Copy the inorder successor's
            // data to this node
            root->value = temp->value;

            // Delete the inorder successor
            root->right = removeInternal(root->right,
                                         temp->value);
        }
    }

    // If the tree had only one node
    // then return
    if (!root)
        return root;

    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
    root->height = 1 + max(height(root->left),
                           height(root->right));

    // STEP 3: GET THE BALANCE FACTOR OF
    // THIS NODE (to check whether this
    // node became unbalanced)
    int balance = getBalance(root);

    // If this node becomes unbalanced,
    // then there are 4 cases

    // Left Left Case
    if (balance > 1 &&
        getBalance(root->left) >= 0)
        return rotate_right(root);

    // Left Right Case
    if (balance > 1 &&
        getBalance(root->left) < 0)
    {
        root->left = rotate_left(root->left);
        return rotate_right(root);
    }

    // Right Right Case
    if (balance < -1 &&
        getBalance(root->right) <= 0)
        return rotate_left(root);

    // Right Left Case
    if (balance < -1 &&
        getBalance(root->right) > 0)
    {
        root->right = rotate_right(root->right);
        return rotate_left(root);
    }

    return root;
}
