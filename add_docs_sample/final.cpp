//------------------------------------------------------------------------------
// File:      avl.cpp
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
//------------------------------------------------------------------------------
#ifndef AVL_CPP
#define AVL_CPP
#include "avl.h"
#include <cstdlib>
namespace FOOBAR_AVL
{
	//--------------------------------------------------------------------------
	// Class:       avl
	// Method:      insert(T d, node<T>* &cur)
	// Description: inserts value into tree
	// Programmers: Jacob O'Bryant
	// Version:     1.2
	// Environment: Intel Pentium PC, Arch Linux, gcc 4.7
	// Calls:       rebalance, insert
	// Called by:   insert
	// Parameters:  T d: value to add to tree     
	//              node<T>* &cur: node to check for insertion
    // History:     05.31.2013 JO wrote v1.0
    //              06.01.2013 JO removed no-longer-needed code
    //              06.04.2013 JO rewrote to rebalance as it inserts
	//--------------------------------------------------------------------------
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

	//--------------------------------------------------------------------------
	// Class:       avl
	// Method:      popnode(node<T>* &cur)
	// Description: pops a given node
	// Programmers: Terry Rogers
	// Version:     1.0
	// Environment: Intel Pentium PC, Arch Linux, gcc 4.7
	// Calls:       rebalance
	// Parameters:  node<T>* &cur: node to pop
	// Returns:     returns data of popped node
	// History:     6.9.13 TR completed 1.0
	//--------------------------------------------------------------------------
	template <class T>
	T avl<T>::popnode(node<T>* &cur)
	{
		T data = bst<T>::popnode(cur);
		rebalance(bst<T>::root);
		return data;
	}

	//--------------------------------------------------------------------------
	// Class:       avl
	// Method:      poplow(node<T>* &cur)
	// Description: pops out the leftmost child of cur
	// Programmers: Terry Rogers
	// Version:     1.0
	// Environment: Intel Pentium PC, Arch Linux, gcc 4.7
	// Calls:       rebalance
	// Parameters:  node<T>* &cur: node to pop
	// Returns:     returns data of popped node
	// History:     6.9.13 TR completed 1.0
	//--------------------------------------------------------------------------
	template <class T>
	T avl<T>::poplow(node<T>* &cur)
	{
		T data = bst<T>::poplow(cur);
		rebalance(bst<T>::root);
		return data;
	}

	//--------------------------------------------------------------------------
	// Class:       avl
	// Method:      pophigh(node<T> *&cur)
	// Description: pops out the rightmost child of cur
	// Programmers: Terry Rogers
	// Version:     1.0
	// Environment: Intel Pentium PC, Arch Linux, gcc 4.7
	// Calls:       rebalance
	// Parameters:  node<T> *&cur: node to pop
	// Returns:     returns data of popped node
	// History:     6.9.13 TR completed 1.0
	//--------------------------------------------------------------------------
	template <class T>
	T avl<T>::pophigh(node<T> *&cur)
	{
		T data = bst<T>::pophigh(cur);
		rebalance(bst<T>::root);
		return data;
	}

	//--------------------------------------------------------------------------
	// Class:       avl
	// Method:      popfirst(const T& d, node<T>* np)
	// Description: pops first node matching d
	// Programmers: Terry Rogers
	// Version:     1.0
	// Environment: Intel Pentium PC, Arch Linux, gcc 4.7
	// Calls:       rebalance
	// Parameters:  const T& d:  value to locate
	//              node<T>* np: node to check
	// Returns:     true if d is located. false otherwise.
	// History:     6.9.13 TR completed 1.0
	//--------------------------------------------------------------------------
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

	//--------------------------------------------------------------------------
	// Class:       avl
	// Method:      rotateRight(node<T> *nodeN)
	// Description: performs right rotation for balancing
	// Programmers: Jacob O'Bryant
	// Version:     1.1
	// Environment: Intel Pentium PC, Arch Linux, gcc 4.7
	// Called by:   rotateRightLeft, rotateLeftRight, rebalance
	// Parameters:  node<T> *nodeN: node to rotate
	// Returns:     returns rotated node
    // History:     05.31.2013 JO wrote v1.0
    //              06.04.2013 JO added setHeight calls
	//--------------------------------------------------------------------------
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

	//--------------------------------------------------------------------------
	// Class:       avl
	// Method:      rotateLeft(node<T> *nodeN)
	// Description: performs left rotation for balancing
	// Programmers: Jacob O'Bryant
	// Version:     1.1
	// Environment: Intel Pentium PC, Arch Linux, gcc 4.7
	// Called by:   rotateRightLeft, rotateLeftRight, rebalance
	// Parameters:  node<T> *nodeN: node to rotate
	// Returns:     returns rotated node
    // History:     05.31.2013 JO wrote v1.0
    //              06.04.2013 JO added setHeight calls  
	//--------------------------------------------------------------------------
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

	//--------------------------------------------------------------------------
	// Class:       avl
	// Method:      rotateRightLeft(node<T> *nodeN)
	// Description: Performs right left rotation for balancing
	// Programmers: Jacob O'Bryant
	// Version:     1.0
	// Environment: Intel Pentium PC, Arch Linux, gcc 4.7
	// Calls:       rotateRight, rotateLeft
	// Called by:   rebalance
	// Parameters:  node<T> *nodeN: node to rotate
	// Returns:     returns rotated node
    // History:     05.31.2013 JO wrote v1.0
	//--------------------------------------------------------------------------
	template <class T>
	node<T>* avl<T>::rotateRightLeft(node<T> *nodeN)
	{ 
        nodeN->left = rotateLeft(nodeN->left);
        return rotateRight(nodeN);
	}

	//--------------------------------------------------------------------------
	// Class:       avl
	// Method:      rotateLeftRight(node<T> *nodeN)
	// Description: Performs left right rotaion for balancing.
	// Programmers: Jacob O'Bryant
	// Version:     1.0
	// Environment: Intel Pentium PC, Arch Linux, gcc 4.7
	// Calls:       rotateRight, rotateLeft
	// Called by:   rebalance
	// Parameters:  node<T> *nodeN: node to rotate
	// Returns:     returns rotated node
    // History:     05.31.2013 JO wrote v1.0 
	//--------------------------------------------------------------------------
	template <class T>
	node<T>* avl<T>::rotateLeftRight(node<T> *nodeN) 
	{ 
        nodeN->right = rotateRight(nodeN->right);
		return rotateLeft(nodeN);
	}

	//--------------------------------------------------------------------------
	// Class:       avl
	// Method:      rebalance(node<T> *&nodeN)
	// Description: recursively balances tree when diff too large
	// Programmers: Jacob O'Bryant, Terry Rogers
	// Version:     1.3
	// Environment: Intel Pentium PC, Arch Linux, gcc 4.7
	// Calls:       rebalance, rotateLeft, getHeightDifference, rotateRight, 
	//					rotateLeftRight, rotateRightLeft
	// Called by:   insert, popnode, poplow, pophigh, popfirst, rebalance
	// Parameters:  node<T> *&nodeN: node to balance from
    // History:     05.31.2013 JO wrote v1.0
    //              06.01.2013 JO fixed to reset nodeN to the new root
    //              06.04.2013 JO changed to only balance the given node
    //                         (not children)
	//				06.09.2013 TR changed to balance recursively downward. 
	//--------------------------------------------------------------------------
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

	//--------------------------------------------------------------------------
	// Class:       avl
	// Method:      getHeightDifference(const node<T> *const nodeN)
	// Description: returns height difference
	// Programmers: Jacob O'Bryant
	// Version:     1.0
	// Environment: Intel Pentium PC, Arch Linux, gcc 4.7
	// Called by:   rebalance
	// Parameters:  const node<T> *const nodeN: node to check diff
	// Returns:     returns height difference
    // History:     05.31.2013 JO wrote v1.0
	//--------------------------------------------------------------------------
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
