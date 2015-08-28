//------------------------------------------------------------------------
// File:      input.cpp
// Functions: void insert(T d, node<T>* &cur)
//            T popnode(node<T>* &cur)
//            T poplow(node<T>* &cur)
//            T pophigh(node<T> *&cur)
//            bool popfirst(const T& d, node<T>* np)
//            node<T>* rotateRight(node<T> *nodeN)
//            node<T>* rotateLeft(node<T> *nodeN)
//            node<T>* rotateRightLeft(node<T> *nodeN)
//            node<T>* rotateLeftRight(node<T> *nodeN)
//            void rebalance(node<T> *&nodeN)
//            int getHeightDifference(const node<T> *const nodeN)
//------------------------------------------------------------------------
#ifndef AVL_CPP
#define AVL_CPP
#include "avl.h"
#include <cstdlib>
namespace FOOBAR_AVL
{
	//--------------------------------------------------------------------
	// Class:       avl
	// Method:      insert(T d, node<T>* &cur)
	// Description: 
	// Programmers: Jacob O'Bryant
	// Version:     1.0
	// Environment: Intel Pentium PC, Arch Linux, gcc 4.8
	// Input:       
	// Output:      
	// Calls:       insert, rebalance
	// Called by:   insert
	// Parameters:  T d:           
	//              node<T>* &cur: 
	// Throws:      
	// History:     
	//--------------------------------------------------------------------
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

	//--------------------------------------------------------------------
	// Class:       avl
	// Method:      popnode(node<T>* &cur)
	// Description: 
	// Programmers: Jacob O'Bryant
	// Version:     1.0
	// Environment: Intel Pentium PC, Arch Linux, gcc 4.8
	// Input:       
	// Output:      
	// Calls:       rebalance
	// Parameters:  node<T>* &cur: 
	// Returns:     
	// Throws:      
	// History:     
	//--------------------------------------------------------------------
	template <class T>
	T avl<T>::popnode(node<T>* &cur)
	{
		T data = bst<T>::popnode(cur);
		rebalance(bst<T>::root);
		return data;
	}

	//--------------------------------------------------------------------
	// Class:       avl
	// Method:      poplow(node<T>* &cur)
	// Description: 
	// Programmers: Jacob O'Bryant
	// Version:     1.0
	// Environment: Intel Pentium PC, Arch Linux, gcc 4.8
	// Input:       
	// Output:      
	// Calls:       rebalance
	// Parameters:  node<T>* &cur: 
	// Returns:     
	// Throws:      
	// History:     
	//--------------------------------------------------------------------
	template <class T>
	T avl<T>::poplow(node<T>* &cur)
	{
		T data = bst<T>::poplow(cur);
		rebalance(bst<T>::root);
		return data;
	}

	//--------------------------------------------------------------------
	// Class:       avl
	// Method:      pophigh(node<T> *&cur)
	// Description: 
	// Programmers: Jacob O'Bryant
	// Version:     1.0
	// Environment: Intel Pentium PC, Arch Linux, gcc 4.8
	// Input:       
	// Output:      
	// Calls:       rebalance
	// Parameters:  node<T> *&cur: 
	// Returns:     
	// Throws:      
	// History:     
	//--------------------------------------------------------------------
	template <class T>
	T avl<T>::pophigh(node<T> *&cur)
	{
		T data = bst<T>::pophigh(cur);
		rebalance(bst<T>::root);
		return data;
	}

	//--------------------------------------------------------------------
	// Class:       avl
	// Method:      popfirst(const T& d, node<T>* np)
	// Description: 
	// Programmers: Jacob O'Bryant
	// Version:     1.0
	// Environment: Intel Pentium PC, Arch Linux, gcc 4.8
	// Input:       
	// Output:      
	// Calls:       rebalance
	// Parameters:  const T& d:  
	//              node<T>* np: 
	// Returns:     
	// Throws:      
	// History:     
	//--------------------------------------------------------------------
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

	//--------------------------------------------------------------------
	// Class:       avl
	// Method:      rotateRight(node<T> *nodeN)
	// Description: 
	// Programmers: Jacob O'Bryant
	// Version:     1.0
	// Environment: Intel Pentium PC, Arch Linux, gcc 4.8
	// Input:       
	// Output:      
	// Called by:   rotateRightLeft, rotateLeftRight, rebalance
	// Parameters:  node<T> *nodeN: 
	// Returns:     
	// Throws:      
	// History:     
	//--------------------------------------------------------------------
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

	//--------------------------------------------------------------------
	// Class:       avl
	// Method:      rotateLeft(node<T> *nodeN)
	// Description: 
	// Programmers: Jacob O'Bryant
	// Version:     1.0
	// Environment: Intel Pentium PC, Arch Linux, gcc 4.8
	// Input:       
	// Output:      
	// Called by:   rotateRightLeft, rotateLeftRight, rebalance
	// Parameters:  node<T> *nodeN: 
	// Returns:     
	// Throws:      
	// History:     
	//--------------------------------------------------------------------
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

	//--------------------------------------------------------------------
	// Class:       avl
	// Method:      rotateRightLeft(node<T> *nodeN)
	// Description: 
	// Programmers: Jacob O'Bryant
	// Version:     1.0
	// Environment: Intel Pentium PC, Arch Linux, gcc 4.8
	// Input:       
	// Output:      
	// Calls:       rotateRight, rotateLeft
	// Called by:   rebalance
	// Parameters:  node<T> *nodeN: 
	// Returns:     
	// Throws:      
	// History:     
	//--------------------------------------------------------------------
	template <class T>
	node<T>* avl<T>::rotateRightLeft(node<T> *nodeN)
	{ 
        nodeN->left = rotateLeft(nodeN->left);
        return rotateRight(nodeN);
	}

	//--------------------------------------------------------------------
	// Class:       avl
	// Method:      rotateLeftRight(node<T> *nodeN)
	// Description: 
	// Programmers: Jacob O'Bryant
	// Version:     1.0
	// Environment: Intel Pentium PC, Arch Linux, gcc 4.8
	// Input:       
	// Output:      
	// Calls:       rotateRight, rotateLeft
	// Called by:   rebalance
	// Parameters:  node<T> *nodeN: 
	// Returns:     
	// Throws:      
	// History:     
	//--------------------------------------------------------------------
	template <class T>
	node<T>* avl<T>::rotateLeftRight(node<T> *nodeN) 
	{ 
        nodeN->right = rotateRight(nodeN->right);
		return rotateLeft(nodeN);
	}

	//--------------------------------------------------------------------
	// Class:       avl
	// Method:      rebalance(node<T> *&nodeN)
	// Description: 
	// Programmers: Jacob O'Bryant
	// Version:     1.0
	// Environment: Intel Pentium PC, Arch Linux, gcc 4.8
	// Input:       
	// Output:      
	// Calls:       getHeightDifference, rotateRightLeft, rebalance, rotateLeft, rotateRight, rotateLeftRight
	// Called by:   insert, popnode, poplow, pophigh, popfirst, rebalance
	// Parameters:  node<T> *&nodeN: 
	// Throws:      
	// History:     
	//--------------------------------------------------------------------
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

	//--------------------------------------------------------------------
	// Class:       avl
	// Method:      getHeightDifference(const node<T> *const nodeN)
	// Description: 
	// Programmers: Jacob O'Bryant
	// Version:     1.0
	// Environment: Intel Pentium PC, Arch Linux, gcc 4.8
	// Input:       
	// Output:      
	// Called by:   rebalance
	// Parameters:  const node<T> *const nodeN: 
	// Returns:     
	// Throws:      
	// History:     
	//--------------------------------------------------------------------
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
