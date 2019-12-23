#include "RBT.h"
//#include <QDebug>
RBT::~RBT() {
	while (root) 
	{
		remove(root->val);
	}
}
const myPair& RBT::find_min_value() const 
{
	RBT_Node* temp{ root };
	while (temp->left) {
		temp = temp->left;
	}
	return temp->val;
}

void RBT::leftRotate(RBT_Node* x) //move x out, move x's right child in
{                                 //assign x's right child's left child to x->right
	RBT_Node* rightChild{ x->right };
	if (x == root)
		root = rightChild;

	if (x->parent != nullptr)
	{	
		if (x == x->parent->left) //x is on the left
			x->parent->left = rightChild;
		else
			x->parent->right = rightChild;
	}
	rightChild->parent = x->parent;
	x->parent = rightChild;

	x->right = rightChild->left;
	if (rightChild->left != nullptr)
		rightChild->left->parent = x;


	rightChild->left = x;
}
void RBT::rightRotate(RBT_Node* x) //move x out, move x's left child in
{                                  //assign x's left child's right child to x->left
	RBT_Node* leftChild{ x->left };
	if (x == root)
		root = leftChild;
	if (x->parent != nullptr)
	{
		if (x == x->parent->left) //x is on the left
			x->parent->left = leftChild;
		else
			x->parent->right = leftChild;
	}
		leftChild->parent = x->parent;
			x->parent = leftChild;
	
	x->left = leftChild->right;
	if (leftChild->right != nullptr)
		leftChild->right->parent = x;
	leftChild->right = x;
}





void RBT::swapColors(RBT_Node* x1, RBT_Node* x2)
{
	COLOR temp;
	temp = x1->color;
	x1->color = x2->color;
	x2->color = temp;
}
void RBT::swapValues(RBT_Node* u, RBT_Node* v)
{
	myPair temp;
	std::vector<RBT_Node*> duplicatesTemp;

	temp = u->val;
	duplicatesTemp = u->duplicates;

	u->val = v->val;
	u->duplicates = v->duplicates;

	v->val = temp;
	v->duplicates = duplicatesTemp;
}
void RBT::fixRedRed(RBT_Node* x)
{
	// if x is root color it black and return
	if (x == root)
	{
		x->color = BLACK;
		return;
	}

	// initialize parent, grandparent, uncle
	RBT_Node* parent = x->parent, * grandparent = parent->parent,
		* uncle = x->uncle();

	if (parent->color != BLACK)
	{
		if (uncle != nullptr && uncle->color == RED)
		{
			// uncle red, perform recoloring and recurse
			parent->color = BLACK;
			uncle->color = BLACK;
			grandparent->color = RED;
			fixRedRed(grandparent);
		}
		else
		{
			// Else perform LR, LL, RL, RR
			if (parent->isOnLeft())
			{
				if (x->isOnLeft())
				{
					// for left right
					swapColors(parent, grandparent);
				}
				else
				{
					leftRotate(parent);
					swapColors(x, grandparent);
				}
				// for left left and left right
				rightRotate(grandparent);
			}
			else
			{
				if (x->isOnLeft())
				{
					// for right left
					rightRotate(parent);
					swapColors(x, grandparent);
				}
				else
				{
					swapColors(parent, grandparent);
				}

				// for right right and right left
				leftRotate(grandparent);
			}
		}
	}
}
RBT_Node* RBT::successor(RBT_Node* x)//find leftmost node from x
{
	RBT_Node* temp = x;

	while (temp->left != nullptr)
		temp = temp->left;

	return temp;
}
RBT_Node* RBT::BSTreplace(RBT_Node* x) //find x's child which is to replace x
{
	// when node have 2 children
	if (x->left != nullptr && x->right != nullptr)
		return successor(x->right);//min value node of right subtree 

	// when leaf
	if (x->left == nullptr && x->right == nullptr)
		return nullptr;

	// when single child
	if (x->left != nullptr)
		return x->left;
	else
		return x->right;
}
void RBT::deleteNode(RBT_Node* v)
{
	RBT_Node* u = BSTreplace(v);

	// True when u and v are both black, or v is a black leaf
	bool uvBlack = ((u == nullptr || u->color == BLACK) && (v->color == BLACK));
	RBT_Node* parent = v->parent;

	if (u == nullptr)
	{
		// u is nullptr therefore v has no child and is a leaf
		if (v == root)
		{
			// v is root, making root nullptr
			root = nullptr;
		}
		else
		{
			if (uvBlack)
			{
				// u and v both black (u is nullptr, means it's black)
				// v is leaf, fix double black at v
				fixDoubleBlack(v);
			}
			else
			{
				// v is red, u is nullptr
				if (v->sibling() != nullptr)
					// sibling is not nullptr, make it red"
					v->sibling()->color = RED;
			}

			// delete v from the tree
			if (v->isOnLeft())
			{
				parent->left = nullptr;
			}
			else
			{
				parent->right = nullptr;
			}
		}
		delete v;
		return;
	}

	if (v->left == nullptr || v->right == nullptr)		// v has 1 child 					
	{								//v must be black and the child must be red
		if (v == root)
		{
			// v is root, assign the value of u to v, and delete u
			v->val = u->val;
			v->duplicates = u->duplicates;
			v->left = v->right = nullptr;
			delete u;
		}
		else
		{
			// Detach v from tree and move u up
			if (v->isOnLeft())
			{
				parent->left = u;
			}
			else
			{
				parent->right = u;
			}
			delete v;
			u->parent = parent;
			if (uvBlack)
			{
				// u and v both black, fix double black at u
				fixDoubleBlack(u);
			}
			else
			{
				// u or v red, color u black
				u->color = BLACK;
			}
		}
		return;
	}

	// v has 2 children, swap values with successor and recurse
	swapValues(u, v);
	deleteNode(u);
}
void RBT::fixDoubleBlack(RBT_Node* x)
{
	if (x == root)
		// Reached root
		return;

	RBT_Node* sibling = x->sibling(), * parent = x->parent;
	if (sibling == nullptr)
	{
		// No sibiling, double black pushed up
		fixDoubleBlack(parent);
	}
	else
	{
		if (sibling->color == RED)
		{
			// Sibling red
			parent->color = RED;
			sibling->color = BLACK;
			if (sibling->isOnLeft())
			{
				// left case
				rightRotate(parent);
			}
			else
			{
				// right case
				leftRotate(parent);
			}
			fixDoubleBlack(x);
		}
		else
		{
			// Sibling black
			if (sibling->hasRedChild())
			{
				// at least 1 red children
				if (sibling->left != nullptr and sibling->left->color == RED)
				{
					if (sibling->isOnLeft())
					{
						// left left
						sibling->left->color = sibling->color;
						sibling->color = parent->color;
						rightRotate(parent);
					}
					else
					{
						// right left
						sibling->left->color = parent->color;
						rightRotate(sibling);
						leftRotate(parent);
					}
				}
				else
				{
					if (sibling->isOnLeft())
					{
						// left right
						sibling->right->color = parent->color;
						leftRotate(sibling);
						rightRotate(parent);
					}
					else
					{
						// right right
						sibling->right->color = sibling->color;
						sibling->color = parent->color;
						leftRotate(parent);
					}
				}
				parent->color = BLACK;
			}
			else
			{
				// 2 black children
				sibling->color = RED;
				if (parent->color == BLACK)
					fixDoubleBlack(parent);
				else
					parent->color = BLACK;
			}
		}
	}
}
RBT_Node* RBT::search(myPair n)
{
	RBT_Node* temp = root;
	while (temp != nullptr)
	{
		if (n < temp->val)
		{
			if (temp->left == nullptr)
			{
				//return nullptr;//not found
				break;
			}
			else
				temp = temp->left;
		}
		else if (n == temp->val)
		{
			break;
		}
		else//n > temp->val
		{
			if (temp->right == nullptr)//not found
			{
				//return nullptr;
				break;
			}
			else
				temp = temp->right;
		}
	}

	return temp;
}
void RBT::insert(const myPair& n)
{
	RBT_Node* newNode = new RBT_Node(n);
	if (root == nullptr)
	{
		// when root is nullptr
		// simply insert value at root
		newNode->color = BLACK;
		root = newNode;
	}
	else
	{
		RBT_Node* temp = search(n);

		if (temp->val == n)
		{
			// return if value already exists
			temp->duplicates.push_back(new RBT_Node(n));

			return;
		}

		// if value is not found, search returns the node
		// where the value is to be inserted

		// connect new node to correct node
		newNode->parent = temp;

		if (n < temp->val)
			temp->left = newNode;
		else
			temp->right = newNode;

		// fix red red voilaton if exists
		fixRedRed(newNode);
	}
}

void RBT::remove(const myPair&key)
{
	if (root == nullptr)
		// Tree is empty
		return;
	RBT_Node* v = search(key);// , * u;
	if (v->val == key)//found the node to be deleted
	{
		if (!v->duplicates.empty())
		{
			if (v->val.vertex == key.vertex)
			{
				v->val.vertex = v->duplicates.back()->val.vertex;
				delete v->duplicates.back();
				v->duplicates.pop_back();
				return;
			}
			else
			{
				for (std::vector<RBT_Node*>::const_iterator iter{ v->duplicates.begin() };
					iter != v->duplicates.end();
					++iter)
				{
					if ((*iter)->val.vertex == key.vertex)
					{
						delete (*iter);
						v->duplicates.erase(iter);
						return;
					}
				}
			}
		}
		else deleteNode(v);
	}
}
std::string color2string(COLOR color)
{
	if (color == BLACK)
	{
		return std::string("B");
	}
	else
	{
		return std::string("R");
	}
}
void RBT::printBack(RBT_Node* node, int depth) const
{
	if (!node) // Base case
		return;
	printBack(node->right, depth + 1); // Recursion: right sub-tree

	for (int j = 0; j < depth; j++) // Print the node value
		std::cout << '\t';
	std::cout << node->val << color2string(node->color);
	if (!node->duplicates.empty())
	{
		std::cout<< "("<< node->duplicates.size() + 1<<")";
	}
	std::cout << std::endl;

	printBack(node->left, depth + 1); // Recursion: left sub-tree
}
void RBT::print(int depth) const 
{
	printBack(root, depth);
}	
bool RBT::is_empty()const {
	return root == nullptr;
}
