
#ifndef _CLASS_H_
#define _CLASS_H_

#include <iomanip>
using namespace std;
enum RBTColor { RED, BLACK };

template <class  T, class  N>
class RBTNode {
public:
	RBTColor  color;  
	T         key;
	N         non_key;
	N         non_key2;
	RBTNode  *left;   
	RBTNode  *right;  
	RBTNode  *parent; 

	RBTNode(T value, N othervalue, N othervalue2, RBTColor c, RBTNode *p, RBTNode *l, RBTNode *r) :
		key(value),  non_key(othervalue), non_key2(othervalue2), color(c), parent(), left(l), right(r) {}
};

template <class T,class N>
class RBTree {

public:
	RBTNode<T, N> *mRoot;
	RBTree();
	~RBTree();

	int size(RBTNode<T, N>* x) const;
	RBTNode<T, N>* search(T key);
	T minimum();
	T maximum();
	RBTNode<T, N>* successor(RBTNode<T, N> *x);
	RBTNode<T, N>* predecessor(RBTNode<T, N> *x);
	void insert(T key, N non_key, N non_key2);
	void insertNode(RBTNode<T, N>* &root, RBTNode<T, N>* node);
	void remove(T key);
	void destroy();
	RBTNode<T, N>* maximumNode(RBTNode<T, N>* tree);
	RBTNode<T, N>* minimumNode(RBTNode<T, N>* tree);
	void removeNode(RBTNode<T, N>* &root, RBTNode<T, N> *node);

private:

	RBTNode<T, N>* search(RBTNode<T, N>* x, T key) const;
	void leftRotate(RBTNode<T, N>* &root, RBTNode<T, N>* x);
	void rightRotate(RBTNode<T, N>* &root, RBTNode<T, N>* y);
	void insertFixUp(RBTNode<T, N>* &root, RBTNode<T, N>* node);
	void removeFixUp(RBTNode<T, N>* &root, RBTNode<T, N> *node, RBTNode<T, N> *parent);
	void destroy(RBTNode<T, N>* &tree);
	void print(RBTNode<T, N>* tree, T key, int direction);

};

template <class T, class N>
RBTree<T, N>::RBTree() :mRoot(NULL)
{
	mRoot = NULL;
}

template <class T, class N>
RBTree<T, N>::~RBTree()
{
	destroy();
}


template <class T, class N>
RBTNode<T, N>* RBTree<T, N>::search(RBTNode<T, N>* x, T key) const
{
	if (x == NULL || x->key == key)
	{
		return x;
	}
	
	if (key < x->key)
		return search(x->left, key);
	else
		return search(x->right, key);
}

template <class T, class N>
RBTNode<T, N>* RBTree<T, N>::search(T key)
{
	return search(mRoot, key);
}
template <class T, class N>
RBTNode<T, N>* RBTree<T, N>::minimumNode(RBTNode<T, N>* tree)
{
	if (tree == NULL)
		return NULL;

	while (tree->left != NULL)
		tree = tree->left;
	return tree;
}

template <class T, class N>
T RBTree<T, N>::minimum()
{
	RBTNode<T,  N> *p = minimumNode(mRoot);
	if (p != NULL)
		return p->key;

	return (T)NULL;
}

template <class T, class N>
RBTNode<T, N>* RBTree<T, N>::maximumNode(RBTNode<T, N>* tree)
{
	if (tree == NULL)
		return NULL;

	while (tree->right != NULL)
		tree = tree->right;
	return tree;
}

template <class T, class N>
T RBTree<T, N>::maximum()
{
	RBTNode<T, N> *p = maximumNode(mRoot);
	if (p != NULL)
		return p->key;

	return (T)NULL;
}

template <class T, class N>
RBTNode<T, N>* RBTree<T, N>::successor(RBTNode<T, N> *x)
{
	if (x->right != NULL)
		return minimumNode(x->right);

	RBTNode<T, N>* y = x->parent;
	while ((y != NULL) && (x == y->right))
	{
		x = y;
		y = y->parent;
	}

	return y;
}

template <class T, class N>
RBTNode<T, N>* RBTree<T, N>::predecessor(RBTNode<T, N> *x)
{
	if (x->left != NULL)
		return maximumNode(x->left);

	RBTNode<T, N >* y = x->parent;
	while ((y != NULL) && (x == y->left))
	{
		x = y;
		y = y->parent;
	}

	return y;
}

template <class T, class N>
void RBTree<T, N>::leftRotate(RBTNode<T, N>* &root, RBTNode<T, N>* x)
{
	RBTNode<T, N > *y = x->right;
	x->right = y->left;
	if (y->left != NULL)
		y->left->parent = x;
	y->parent = x->parent;

	if (x->parent == NULL)
	{
		root = y;  
	}
	else
	{
		if (x->parent->left == x)
			x->parent->left = y;    
		else
			x->parent->right = y;   
	}
	y->left = x;
	x->parent = y;
}
template <class T, class N>
void RBTree<T, N>::rightRotate(RBTNode<T, N>* &root, RBTNode<T, N>* y)
{
	RBTNode<T, N > *x = y->left;

	y->left = x->right;
	if (x->right != NULL)
		x->right->parent = y;

	x->parent = y->parent;

	if (y->parent == NULL)
	{
		root = x;            
	}
	else
	{
		if (y == y->parent->right)
			y->parent->right = x;    
		else
			y->parent->left = x;    
	}

	
	x->right = y;

	y->parent = x;
}

template <class T, class N>
void RBTree<T, N>::insertFixUp(RBTNode<T, N>* &root, RBTNode<T, N>* node)
{
	RBTNode<T, N> *parent, *gparent;
	while ((parent = node->parent ) && parent->color == RED)
	{
		gparent = parent->parent;
		if (parent == gparent->left)
		{
			{
				RBTNode<T, N> *uncle = gparent->right;
				if (uncle && uncle->color == RED)
				{
					uncle->color = BLACK;
					parent->color = BLACK;
					gparent->color = RED;
					node = gparent;
					continue;
				}
			}

			if (parent->right == node)
			{
				RBTNode<T, N> *tmp;
				leftRotate(root, parent);
				tmp = parent;
				parent = node;
				node = tmp;
			}

			parent->color = BLACK;
			gparent->color = RED;
			rightRotate(root, gparent);
		}
		else
		{
			{
				RBTNode<T, N> *uncle = gparent->left;
				if (uncle && uncle->color == RED)
				{
					uncle->color = BLACK;
					parent->color = BLACK;
					gparent->color = RED;
					node = gparent;
					continue;
				}
			}

			if (parent->left == node)
			{
				RBTNode<T, N> *tmp;
				rightRotate(root, parent);
				tmp = parent;
				parent = node;
				node = tmp;
			}

			parent->color = BLACK;
			gparent->color = RED;
			leftRotate(root, gparent);
		}
	}

	root->color = BLACK;
}

template <class T, class N>
void RBTree<T, N>::insertNode(RBTNode<T, N>* &root, RBTNode<T, N>* node)
{
	RBTNode<T, N> *y = NULL;
	RBTNode<T, N> *x = root;

	while (x != NULL)
	{
		y = x;
		if (node->key < x->key)
			x = x->left;
		else
			x = x->right;
	}

	node->parent = y;
	if (y != NULL)
	{
		if (node->key < y->key)
			y->left = node;
		else
			y->right = node;
	}
	else
		root = node;

	node->color = RED;

	insertFixUp(root, node);
}

template <class T, class N>
void RBTree<T, N>::insert(T key, N non_key, N non_key2)
{
	RBTNode<T, N> *z = NULL;
	if ((z = new RBTNode<T, N>(key, non_key, non_key2, BLACK, NULL, NULL, NULL)) == NULL)
		return;

	insertNode(mRoot, z);
}

template <class T, class N>
void RBTree<T, N>::removeFixUp(RBTNode<T, N>* &root, RBTNode<T, N> *node, RBTNode<T, N> *parent)
{
	RBTNode<T, N> *other;

	while ((!node || node->color == BLACK) && node != root)
	{
		if (parent->left == node)
		{
			other = parent->right;
			if (other->color == RED)
			{
				other->color = BLACK;
				parent->color = RED;
				leftRotate(root, parent);
				other = parent->right;
			}
			if ((!other->left || other->left->color == BLACK) &&
				(!other->right || other->right->color == BLACK))
			{
				other->color = RED;
				node = parent;
				parent = node->parent;
			}
			else
			{
				if (!other->right || other->right->color == BLACK)
				{
					other->left->color = BLACK;
					other->color = RED;
					rightRotate(root, other);
					other = parent->right;
				}
				other->color = parent->color;
				parent->color = BLACK;
				other->right->color = BLACK;
				leftRotate(root, parent);
				node = root;
				break;
			}
		}
		else
		{
			other = parent->left;
			if (other->color == RED)
			{
				other->color = BLACK;
				parent->color = RED;
				rightRotate(root, parent);
				other = parent->left;
			}
			if ((!other->left || other->left->color == BLACK) &&
				(!other->right || other->right->color == BLACK))
			{
				other->color = RED;
				node = parent;
				parent = node->parent;
			}
			else
			{
				if (!other->left || other->left->color == BLACK)
				{
					other->right->color = BLACK;
					other->color = RED;
					leftRotate(root, other);
					other = parent->left;
				}
				other->color = parent->color;
				parent->color = BLACK;
				other->left->color = BLACK;
				rightRotate(root, parent);
				node = root;
				break;
			}
		}
	}
	if (node)
		node->color = BLACK;
}

template <class T, class N>
void RBTree<T, N>::removeNode(RBTNode<T, N>* &root, RBTNode<T, N> *node)
{
	RBTNode<T, N> *child, *parent;
	RBTColor color;

	if ((node->left != NULL) && (node->right != NULL))
	{
		RBTNode<T, N> *replace = node;

		replace = replace->right;
		while (replace->left != NULL)
			replace = replace->left;

		if (node->parent)
		{
			if (node->parent->left == node)
				node->parent->left = replace;
			else
				node->parent->right = replace;
		}
		else
			root = replace;

		child = replace->right;
		parent = replace->parent;
		color = replace->color;

		if (parent == node)
		{
			parent = replace;
		}
		else
		{
			if (child)
				child->parent = parent;
			parent->left = child;

			replace->right = node->right;
			node->right->parent = replace;
		}

		replace->parent = node->parent;
		replace->color = node->color;
		replace->left = node->left;
		node->left->parent = replace;

		if (color == BLACK)
			removeFixUp(root, child, parent);

		delete node;
		return;
	}

	if (node->left != NULL)
		child = node->left;
	else
		child = node->right;

	parent = node->parent;
	color = node->color;

	if (child)
		child->parent = parent;

	if (parent)
	{
		if (parent->left == node)
			parent->left = child;
		else
			parent->right = child;
	}
	else
		root = child;

	if (color == BLACK)
		removeFixUp(root, child, parent);
	delete node;
}

template <class T, class N>
void RBTree<T, N>::remove(T key)
{
	RBTNode<T, N> *node;

	if ((node = search(mRoot, key)) != NULL)
		removeNode(mRoot, node);
}

template <class T, class N>
void RBTree<T, N>::destroy(RBTNode<T, N>* &tree)
{
	if (tree == NULL)
		return;

	if (tree->left != NULL)
		return destroy(tree->left);
	if (tree->right != NULL)
		return destroy(tree->right);

	delete tree;
	tree = NULL;
}

template <class T, class N>
void RBTree<T, N>::destroy()
{
	destroy(mRoot);
}


template <class T, class N>
int RBTree<T, N>::size(RBTNode<T, N>* x) const
{
	if (x == NULL) { 
		return 0;
	}
	else { 
		return size(x->left) + size(x->right) + 1;
	}
}

#endif
