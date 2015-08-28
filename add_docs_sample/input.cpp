#ifndef AVL_CPP
#define AVL_CPP
#include "avl.h"
#include <cstdlib>
namespace FOOBAR_AVL
{
	template <class T>
	void avl<T>::insert(T d, node<T>* &cur)
	{
		if (cur == nullptr)
		{                       
			cur = new node<T>(d);
			if (bst<T>::isempty())
            {
				bst<T>::root = cur;
            }
		}
		else
		{
			if (d == cur->value())
            {
                return;
            }
            else if (d < cur->value())
            {
				insert(d, cur->left);
            }
			else
            {
				insert(d, cur->right);
            }
            cur->shallowSetHeight();
            rebalance(cur);
		}
	}

	template <class T>
	T avl<T>::popnode(node<T>* &cur)
	{
		T data = bst<T>::popnode(cur);
		rebalance(bst<T>::root);
		return data;
	}

	template <class T>
	T avl<T>::poplow(node<T>* &cur)
	{
		T data = bst<T>::poplow(cur);
		rebalance(bst<T>::root);
		return data;
	}

	template <class T>
	T avl<T>::pophigh(node<T> *&cur)
	{
		T data = bst<T>::pophigh(cur);
		rebalance(bst<T>::root);
		return data;
	}

	template <class T>
	bool avl<T>::popfirst(const T& d, node<T>* np)
	{
		if(bst<T>::popfirst(d, np))
		{
			rebalance(bst<T>::root);
			return true;
		}
		return false;
	}

	template <class T>
	node<T>* avl<T>::rotateRight(node<T> *nodeN) 
	{ 
        node<T>* tmp = nodeN->left->right;
        nodeN->left->right = nodeN;
        nodeN = nodeN->left;
        nodeN->right->left = tmp;
        nodeN->right->shallowSetHeight();
        nodeN->shallowSetHeight();
		return nodeN; 
	}	

	template <class T>
	node<T>* avl<T>::rotateLeft(node<T> *nodeN) 
	{ 
        node<T>* tmp = nodeN->right->left;
        nodeN->right->left = nodeN;
        nodeN = nodeN->right;
        nodeN->left->right = tmp;
        nodeN->left->shallowSetHeight();
        nodeN->shallowSetHeight();
		return nodeN; 
	}

	template <class T>
	node<T>* avl<T>::rotateRightLeft(node<T> *nodeN)
	{ 
        nodeN->left = rotateLeft(nodeN->left);
        return rotateRight(nodeN);
	}

	template <class T>
	node<T>* avl<T>::rotateLeftRight(node<T> *nodeN) 
	{ 
        nodeN->right = rotateRight(nodeN->right);
		return rotateLeft(nodeN);
	}

	template <class T>
	void avl<T>::rebalance(node<T> *&nodeN) 
	{ 
        int diff;
		if(nodeN == nullptr)
			return;

		rebalance(nodeN->left);
		rebalance(nodeN->right);

        if ((diff = getHeightDifference(nodeN)) < -1)
        {
            if (getHeightDifference(nodeN->left) > 0)
            {
                nodeN = rotateRightLeft(nodeN);
            }
            else
            {
                nodeN = rotateRight(nodeN);
            }
        }
        else if (diff > 1)
        {
            if (getHeightDifference(nodeN->right) < 0)
            {
                nodeN = rotateLeftRight(nodeN);
            }
            else
            {
                nodeN = rotateLeft(nodeN);
            }
        }
	}

	template <class T>
	int avl<T>::getHeightDifference(const node<T> *const nodeN)const 
	{
        if (nodeN == NULL)
        {
            return 0;
        }
        int lheight = (nodeN->left == NULL) ? 0 :
            nodeN->left->getHeight();
        int rheight = (nodeN->right == NULL) ? 0 :
            nodeN->right->getHeight();
		return rheight - lheight; 
	}

}
#endif
