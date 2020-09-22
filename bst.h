#ifndef BST_H
#define BST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <utility>


template <typename Key, typename Value>
class Node
{
public:
    Node(const Key& key, const Value& value, Node<Key, Value>* parent);
    virtual ~Node();

    const std::pair<const Key, Value>& getItem() const;
    std::pair<const Key, Value>& getItem();
    const Key& getKey() const;
    const Value& getValue() const;
    Value& getValue();

    virtual Node<Key, Value>* getParent() const;
    virtual Node<Key, Value>* getLeft() const;
    virtual Node<Key, Value>* getRight() const;

    void setParent(Node<Key, Value>* parent);
    void setLeft(Node<Key, Value>* left);
    void setRight(Node<Key, Value>* right);
    void setValue(const Value &value);

protected:
    std::pair<const Key, Value> item_;
    Node<Key, Value>* parent_;
    Node<Key, Value>* left_;
    Node<Key, Value>* right_;
};


template<typename Key, typename Value>
Node<Key, Value>::Node(const Key& key, const Value& value, Node<Key, Value>* parent) :
    item_(key, value),
    parent_(parent),
    left_(NULL),
    right_(NULL)
{

}


template<typename Key, typename Value>
Node<Key, Value>::~Node()
{

}


template<typename Key, typename Value>
const std::pair<const Key, Value>& Node<Key, Value>::getItem() const
{
    return item_;
}


template<typename Key, typename Value>
std::pair<const Key, Value>& Node<Key, Value>::getItem()
{
    return item_;
}


template<typename Key, typename Value>
const Key& Node<Key, Value>::getKey() const
{
    return item_.first;
}


template<typename Key, typename Value>
const Value& Node<Key, Value>::getValue() const
{
    return item_.second;
}

template<typename Key, typename Value>
Value& Node<Key, Value>::getValue()
{
    return item_.second;
}

template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getParent() const
{
    return parent_;
}


template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getLeft() const
{
    return left_;
}


template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getRight() const
{
    return right_;
}


template<typename Key, typename Value>
void Node<Key, Value>::setParent(Node<Key, Value>* parent)
{
    parent_ = parent;
}


template<typename Key, typename Value>
void Node<Key, Value>::setLeft(Node<Key, Value>* left)
{
    left_ = left;
}

template<typename Key, typename Value>
void Node<Key, Value>::setRight(Node<Key, Value>* right)
{
    right_ = right;
}


template<typename Key, typename Value>
void Node<Key, Value>::setValue(const Value& value)
{
    item_.second = value;
}

//BST implementation
template <typename Key, typename Value>
class BinarySearchTree
{
public:
    BinarySearchTree(); 
    virtual ~BinarySearchTree(); 
    virtual void insert(const std::pair<const Key, Value>& keyValuePair); //TODO
    virtual void remove(const Key& key); 
    void clear(); 
    bool isBalanced() const; 
    bool empty() const;

public:

    class iterator  
    {
    public:
        iterator();

        std::pair<const Key,Value>& operator*() const;
        std::pair<const Key,Value>* operator->() const;

        bool operator==(const iterator& rhs) const;
        bool operator!=(const iterator& rhs) const;

        iterator& operator++();

    protected:
        friend class BinarySearchTree<Key, Value>;
        iterator(Node<Key,Value>* ptr);
        Node<Key, Value> *current_;
    };

public:
    iterator begin() const;
    iterator end() const;
    iterator find(const Key& key) const;
    Value& operator[](const Key& key);
    Value const & operator[](const Key& key) const;

protected:
    Node<Key, Value>* internalFind(const Key& k) const; 
    Node<Key, Value> *getSmallestNode() const;  
    static Node<Key, Value>* predecessor(Node<Key, Value>* current); 
    virtual void nodeSwap( Node<Key,Value>* n1, Node<Key,Value>* n2) ;

    // helper functions 
	void insertHelper(const std::pair<const Key, Value> &keyValuePair, Node<Key, Value>* curr); 
	void removeHelper(const Key& delKey, Node<Key, Value>* curr);
	void clearHelper(Node<Key, Value>* specificNode); 
	Node<Key, Value>* smallNodeHelper( Node<Key, Value>* specificNode ) const;
	int heightHelper(  Node<Key, Value>* specificNode)const;
	bool balanceHelper( Node<Key, Value>* specificNode)const ;
	static Node<Key, Value>* successor(Node<Key, Value>* current); 

protected:
    Node<Key, Value>* root_;
};


template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator(Node<Key,Value> *ptr)
{
    
	current_ = ptr;
}


template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator() 
{
	current_ = NULL;

}


template<class Key, class Value>
std::pair<const Key,Value> &
BinarySearchTree<Key, Value>::iterator::operator*() const
{
    return current_->getItem();
}


template<class Key, class Value>
std::pair<const Key,Value> *
BinarySearchTree<Key, Value>::iterator::operator->() const
{
    return &(current_->getItem());
}

template<class Key, class Value>
bool
BinarySearchTree<Key, Value>::iterator::operator==(
    const BinarySearchTree<Key, Value>::iterator& rhs) const
{
	return rhs.current_ == current_;
}


template<class Key, class Value>
bool
BinarySearchTree<Key, Value>::iterator::operator!=(
    const BinarySearchTree<Key, Value>::iterator& rhs) const
{
    
	return rhs.current_ != current_;

}



template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator&
BinarySearchTree<Key, Value>::iterator::operator++()
{
    
	current_ = successor(current_);
	return *this;

}

//to get succesor of the passed in node
template<class Key, class Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::successor(Node<Key, Value>* current)
{

    if(current->getRight() != NULL){
		//right child exists
		//succesor is the left most node of the right subtree
		current = current->getRight();	
		while( current->getLeft() != NULL  ){
			current = current->getLeft();
		}

		return current;
	}
	else{
		//walk up ancestor chain until you traverse the first left child pointer
		Node<Key, Value>* parentNode  = current->getParent();
		if(parentNode == NULL){
			return NULL;
		}
		while( (current == parentNode->getRight() )  && (parentNode != NULL )  ){

					current = parentNode;
					if(parentNode->getParent() == NULL){
					
						return NULL;
					}
					parentNode = parentNode->getParent();														
		}
		
		if( (current == current->getParent()->getLeft() )){
			return parentNode;
		}
		

			return NULL;

	}
	return NULL;
}



template<class Key, class Value>
BinarySearchTree<Key, Value>::BinarySearchTree() 
{

	root_ = NULL;
}
template<typename Key, typename Value>
BinarySearchTree<Key, Value>::~BinarySearchTree()
{

	clear();

}


template<class Key, class Value>
bool BinarySearchTree<Key, Value>::empty() const
{
    return root_ == NULL;
}


template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::begin() const
{
    BinarySearchTree<Key, Value>::iterator begin(getSmallestNode());
    return begin;
}

template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::end() const
{
    BinarySearchTree<Key, Value>::iterator end(NULL);
    return end;
}


template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::find(const Key & k) const
{
    Node<Key, Value> *curr = internalFind(k);
    BinarySearchTree<Key, Value>::iterator it(curr);
    return it;
}


template<class Key, class Value>
Value& BinarySearchTree<Key, Value>::operator[](const Key& key)
{
    Node<Key, Value> *curr = internalFind(key);
    if(curr == NULL) throw std::out_of_range("Invalid key");
    return curr->getValue();
}
template<class Key, class Value>
Value const & BinarySearchTree<Key, Value>::operator[](const Key& key) const
{
    Node<Key, Value> *curr = internalFind(key);
    if(curr == NULL) throw std::out_of_range("Invalid key");
    return curr->getValue();
}


template<class Key, class Value>
void BinarySearchTree<Key, Value>::insert(const std::pair<const Key, Value> &keyValuePair)
{
	
	if(root_ == NULL){
		//first thing in the tree
		Node<Key, Value>* insertNode = new Node<Key, Value>( keyValuePair.first, keyValuePair.second, NULL );
		root_ = insertNode;
	
		return;
	}
	insertHelper(keyValuePair, root_);

}

//recursive helper function when inserting into BST
//insert function calls this
template<class Key, class Value>
void BinarySearchTree<Key, Value>::insertHelper(const std::pair<const Key, Value> &keyValuePair, Node<Key, Value>* curr){

	if(curr->getKey() == keyValuePair.first){
			curr->setValue( keyValuePair.second );
	}
	else if( curr->getKey() > keyValuePair.first  ){
		
		if(curr->getLeft() == NULL){
			Node<Key, Value>* insertNode = new Node<Key, Value>( keyValuePair.first, keyValuePair.second, curr );
			curr->setLeft(insertNode);
		}
		else{
			insertHelper( keyValuePair, curr->getLeft()  );
		}
	}
	else{
		if(curr->getRight() == NULL  ){
			Node<Key, Value>* insertNode = new Node<Key, Value>( keyValuePair.first, keyValuePair.second, curr );
			curr->setRight(insertNode);
		}
		else{
			insertHelper( keyValuePair, curr->getRight()  );

		}

	}

}



template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::remove(const Key& key)
{
	Node<Key, Value>* curr = root_;
	if(root_ == NULL){
		return;
	}
	if(root_->getKey() == key){
		if( root_->getLeft() == NULL && root_->getRight() == NULL  ){
			root_ = NULL;
			delete curr;
			
		}
		else if( curr->getLeft() == NULL && curr->getRight() != NULL  ){
			//right node present
			root_ = curr->getRight();
			root_->setParent(NULL);
			delete curr;
		
		}
		else if( curr->getLeft() != NULL && curr->getRight() == NULL  ){
			//left node present
			root_ = curr->getLeft();
			root_->setParent(NULL);
			delete curr;
			
		}
		else{
			//curr has 2 child nodes
			//swap with predecessor, then delete node
			Node<Key, Value>* pred = predecessor(curr);
			nodeSwap(pred, curr); //swapping pred node and curr node
			//curr is now a leaf node
			if(curr->getParent()->getRight() == curr){
				
				if(curr->getLeft() != NULL){
					//the original pred had a left child
					
					curr->getParent()->setRight(curr->getLeft());
					curr->getLeft()->setParent( curr->getParent() );
				}
				else{
					curr->getParent()->setRight(NULL);
				}
			}
			else{
				if(curr->getLeft() != NULL){
					curr->getParent()->setLeft(curr->getLeft()); 
					curr->getLeft()->setParent( curr->getParent() );
				}
				else{
					curr->getParent()->setLeft(NULL) ;
				}
			}
			root_ = pred;
			root_->setParent(NULL);
			delete curr;
		}
		
		return;
	}
	removeHelper( key, curr  );
	
	
}

template<class Key, class Value>
void BinarySearchTree<Key, Value>::removeHelper(const Key& delKey, Node<Key, Value>* curr){
	if(curr->getKey() == delKey ){
		if( curr->getLeft() == NULL && curr->getRight() != NULL  ){
			//curr only has right child
			if( (curr->getParent() )->getLeft() == curr){
				(curr->getParent() )->setLeft( curr->getRight()  );		
			}
			else{
				//curr is parent's right
				curr->getParent()->setRight( curr->getRight() ) ;				
			}
			curr->getRight()->setParent(  curr->getParent()  );
			delete curr;
		}
		else if( curr->getRight() == NULL && curr->getLeft() != NULL  ){
			//curr only has left child
			if( (curr->getParent() )->getLeft() == curr){
				(curr->getParent() )->setLeft(curr->getLeft());		
			}
			else{
				//curr is parent's right
				curr->getParent()->setRight( curr->getLeft()  );				
			}
			curr->getLeft()->setParent( curr->getParent() );
			delete curr;
		}
		else if( curr->getRight() == NULL && curr->getLeft() == NULL  ){
			//curr has no children / it is a leaf node
			
			if(curr->getParent()->getRight() == curr){
				curr->getParent()->setRight( NULL );
			}
			else{
				curr->getParent()->setLeft(NULL);
			}
			delete curr;
		}
		else{
			//curr has 2 child nodes
			//swap with predecessor, then delete node
			Node<Key, Value>* pred = predecessor(curr);
			nodeSwap(pred, curr); //swapping pred node and curr node
			//curr is now near bottom of tree
			if(curr->getParent()->getRight() == curr){
				
				if(curr->getLeft() != NULL){
					curr->getParent()->setRight(curr->getLeft()); 
					curr->getLeft()->setParent( curr->getParent() );
				}
				else{
					curr->getParent()->setRight(NULL);
				}
			}
			else{
				if(curr->getLeft() != NULL){
					curr->getParent()->setLeft(curr->getLeft()); 
					curr->getLeft()->setParent( curr->getParent() );
				}
				else {
					curr->getParent()->setLeft(NULL) ;
				}
				
			}
			delete curr;

		}
		return;
	}

	//if curr is not the node to remove

	if(curr->getLeft() == NULL && curr->getRight() == NULL){
		//the key is not in the tree, so do nothing
		return;
	}

	//need to recurse more
	if( curr->getKey() < delKey  ){
		if(curr->getRight() == NULL){
			return;
		}
		removeHelper(delKey, curr->getRight());
	}
	else{
		if(curr->getLeft() == NULL){
			return;
		}
		//curr's key is less than the argument key
		removeHelper(delKey, curr->getLeft());
	}

}


template<class Key, class Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::predecessor(Node<Key, Value>* current)
{
    
	if(current == NULL){
		return current;
	}

	//if left child exists, the pred is the right most node of the left child subtree
	if(current->getLeft() != NULL){
		//go to left subtree
		current = current->getLeft();
		while(current->getRight() != NULL){
			//go to bottom most right node
			current = current->getRight();
		}
		//current is now the most bottom right node in left subtree
		return current;
	}	

	//no left child exists
	Node<Key, Value>* parentNode = current->getParent();
	if(parentNode == NULL){
		//help with what to do
		return NULL;
	}
	while(   (current != current->getParent()->getRight() ) && (parentNode != NULL)){
		//current is not the right node of its parent
		current = parentNode;
		parentNode = parentNode->getParent();
	}
	//parent node's right child is current. Parent node is the pred of original argument 
	return parentNode;

}



template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clear()
{
	if(root_ == NULL){
		return;
	}
	if(root_->getLeft() == NULL && root_->getRight() == NULL){
		//only root node in tree
		delete root_;
		return;
	}
	clearHelper(root_);
	root_ = NULL; 

}

//helper function to clear
//post order recursive tree traversal
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clearHelper(Node<Key, Value>* specificNode){
	if(specificNode == NULL){
		//go off end of tree
		return; 
	}
	clearHelper(specificNode->getLeft() );
	clearHelper(specificNode->getRight() );
	
	delete specificNode;
}



template<typename Key, typename Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::getSmallestNode() const
{
	if(root_ == NULL){
		return NULL;
	}
	if(root_->getLeft()  == NULL){
		return root_;
	}


	
	return smallNodeHelper(root_);

}

template<typename Key, typename Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::smallNodeHelper( Node<Key, Value>* specificNode ) const{
	if(  specificNode->getLeft() == NULL  ){
		//at smallest node
		return specificNode;

	}
	return smallNodeHelper(specificNode->getLeft() );
}

template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::internalFind(const Key& key) const
{

	Node<Key, Value>* curr = root_;
	
	if(root_ == NULL){
		return NULL;
	}

	while( key != curr->getKey() ){
		//iterate through tree 
		if( curr->getKey() > key  ){
			if( curr->getLeft() == NULL  ){
				break;
			}
			curr = curr->getLeft();
		}
		else{
			//curr's key is less than the argument key
			if( curr->getRight() == NULL  ){
				break;
			}
			curr = curr->getRight();
		}

	}
	//either found the node or the node doesnt exist
	if(key == curr->getKey() ){
		//found node
		return curr;
	}

	
	//at end of tree and did not find the node
	return NULL;
	
}


//checks balance by checking if every node is balanced
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isBalanced() const
{
	return balanceHelper(root_);


}

template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::balanceHelper( Node<Key, Value>* specificNode) const{
	if(specificNode == NULL){
		//no nodes in tree
		return true;
	}
	int heightLeft = heightHelper(specificNode->getLeft() );
	int heightRight = heightHelper(specificNode->getRight() );

	if( (  (heightLeft == heightRight) ||  ( (heightLeft-heightRight) == 1  )
		||  ( (heightRight - heightLeft) == 1  ) ) 
		&&  balanceHelper(specificNode->getLeft() ) && balanceHelper(specificNode->getRight() )  ){
			return true;
	}

	return false;
}


template<typename Key, typename Value>
int BinarySearchTree<Key, Value>:: heightHelper(  Node<Key, Value>* specificNode) const{
	if(specificNode == NULL){
		return 0;
	}
	int heightLeft = heightHelper(specificNode->getLeft());
	int heightRight = heightHelper(specificNode->getRight());

	if(heightLeft > heightRight){
		return (1+heightLeft);
	}
	else{
		return (1+heightRight);
	}

}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::nodeSwap( Node<Key,Value>* n1, Node<Key,Value>* n2)
{
    if((n1 == n2) || (n1 == NULL) || (n2 == NULL) ) {
        return;
    }
    Node<Key, Value>* n1p = n1->getParent();
    Node<Key, Value>* n1r = n1->getRight();
    Node<Key, Value>* n1lt = n1->getLeft();
    bool n1isLeft = false;
    if(n1p != NULL && (n1 == n1p->getLeft())) n1isLeft = true;
    Node<Key, Value>* n2p = n2->getParent();
    Node<Key, Value>* n2r = n2->getRight();
    Node<Key, Value>* n2lt = n2->getLeft();
    bool n2isLeft = false;
    if(n2p != NULL && (n2 == n2p->getLeft())) n2isLeft = true;


    Node<Key, Value>* temp;
    temp = n1->getParent();
    n1->setParent(n2->getParent());
    n2->setParent(temp);

    temp = n1->getLeft();
    n1->setLeft(n2->getLeft());
    n2->setLeft(temp);

    temp = n1->getRight();
    n1->setRight(n2->getRight());
    n2->setRight(temp);

    if( (n1r != NULL && n1r == n2) ) {
        n2->setRight(n1);
        n1->setParent(n2);
    }
    else if( n2r != NULL && n2r == n1) {
        n1->setRight(n2);
        n2->setParent(n1);

    }
    else if( n1lt != NULL && n1lt == n2) {
        n2->setLeft(n1);
        n1->setParent(n2);

    }
    else if( n2lt != NULL && n2lt == n1) {
        n1->setLeft(n2);
        n2->setParent(n1);

    }


    if(n1p != NULL && n1p != n2) {
        if(n1isLeft) n1p->setLeft(n2);
        else n1p->setRight(n2);
    }
    if(n1r != NULL && n1r != n2) {
        n1r->setParent(n2);
    }
    if(n1lt != NULL && n1lt != n2) {
        n1lt->setParent(n2);
    }

    if(n2p != NULL && n2p != n1) {
        if(n2isLeft) n2p->setLeft(n1);
        else n2p->setRight(n1);
    }
    if(n2r != NULL && n2r != n1) {
        n2r->setParent(n1);
    }
    if(n2lt != NULL && n2lt != n1) {
        n2lt->setParent(n1);
    }


    if(this->root_ == n1) {
        this->root_ = n2;
    }
    else if(this->root_ == n2) {
        this->root_ = n1;
    }

}


#endif
