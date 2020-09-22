#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <algorithm>
#include "bst.h"

struct KeyError { };


template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    char getBalance () const;
    void setBalance (char balance);
    void updateBalance(char diff);

    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    char balance_;
};

template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}


template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}


template<class Key, class Value>
char AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}


template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(char balance)
{
    balance_ = balance;
}

template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(char diff)
{
    balance_ += diff;
}


template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}


template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}


template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}



template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

	void insertHelper(const std::pair<const Key, Value> &new_item, AVLNode<Key,Value>* curr);
	bool isLeftChild(AVLNode<Key,Value>* curr, AVLNode<Key,Value>* p );
	bool isRightChild(AVLNode<Key,Value>* curr, AVLNode<Key,Value>* p);
	void insertFix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* curr  );
	void removeFix(AVLNode<Key,Value>* p, char diff );
	void removeHelper(const Key& delKey, AVLNode<Key, Value>*curr);
	bool isZigZig(AVLNode<Key,Value>* p, AVLNode<Key,Value>* g  );
	bool isZigZag(AVLNode<Key,Value>* p, AVLNode<Key,Value>* g  );
	void rotateLeft( AVLNode<Key,Value>* p, AVLNode<Key,Value>* g );
	void rotateRight( AVLNode<Key,Value>* p, AVLNode<Key,Value>* g );


};

template<class Key, class Value>
bool AVLTree<Key, Value>::isLeftChild(AVLNode<Key,Value>* curr, AVLNode<Key,Value>* p){
	if(p->getLeft() == curr){
		return true;
	}
	else{
		return false;
	}
}
	
template<class Key, class Value>
bool AVLTree<Key, Value>::isRightChild(AVLNode<Key,Value>* curr, AVLNode<Key,Value>* p){
	if(p->getRight() == curr){
		return true;
	}
	else{
		return false;
	}
}


template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{

	if( BinarySearchTree<Key,Value>::root_ == NULL){
		AVLNode<Key,Value>* insertNode = new AVLNode<Key, Value>( new_item.first, new_item.second, NULL );
		 BinarySearchTree<Key,Value>::root_ = insertNode;

		return;
	}
	insertHelper(new_item, static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key,Value>::root_) );
}


template<class Key, class Value>
void AVLTree<Key, Value>::insertHelper(const std::pair<const Key, Value> &new_item, AVLNode<Key,Value>* curr){
	if(curr->getKey() == new_item.first){
		curr->setValue( new_item.second );
	}
	else if( curr->getKey() > new_item.first  ){
		if(curr->getLeft() == NULL){
			// inserting here (will be left child)
			AVLNode<Key,Value>* insertNode = new AVLNode<Key, Value>( new_item.first, new_item.second, curr );
			curr->setLeft(insertNode);

			if(curr->getBalance() == 1){
				curr->updateBalance(-1);
				

			}
			else if (curr->getBalance() == 0){
				curr->updateBalance(-1); //updating parent
				if(curr->getParent() != NULL){

					insertFix(curr, insertNode);				
				}
				
			}
		}
		else{
			insertHelper( new_item, curr->getLeft()  );
		}
	}
	else{
	
		if(curr->getRight() == NULL  ){
			AVLNode<Key,Value>* insertNode = new AVLNode<Key, Value>( new_item.first, new_item.second, curr );
			curr->setRight(insertNode);
			if(curr->getBalance() == -1){
				curr->updateBalance(1);
			}
		
			else if (curr->getBalance() == 0){
				curr->updateBalance(1);
				if(curr->getParent() != NULL){
				
					insertFix(curr, insertNode);
				}
				
				
			}
			
		}
		else{
			insertHelper( new_item, curr->getRight()  );

		}

	}
}

//helper function to balance grandparents once something has been inserted
template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* curr  ){
	if(p == NULL || p->getParent() == NULL){
		return;
	}
	
	if( isLeftChild(p, p->getParent())  ){
		p->getParent()->updateBalance(-1); 
	}
	else{
		//new node's parent is right child
		p->getParent()->updateBalance(1); 
	}	

	if( p->getParent()->getBalance() == 0 ){
		return;
	}
	else if( isLeftChild(p, p->getParent() ) == true ){
		//p is the left child of g (curr's parent is left child of curr's grandfather node)
		if(p->getParent()->getBalance() == -1){
			insertFix(p->getParent() , p);
		}
		else{
			//balance of grandparent is -2

			//zig zig
			if(isZigZig( p, p->getParent() ) ){
				AVLNode<Key,Value>* g = p->getParent();
				rotateRight( p, p->getParent() );
				p->setBalance(0);
				g->setBalance(0);
			}

			//zig zag, should be functioning like ELSE statement
			else if( isZigZag( p, p->getParent() )){
				AVLNode<Key,Value>* g = p->getParent(); 
				rotateLeft(curr, p);
				rotateRight( p->getParent(), p->getParent()->getParent() ); 

				//setting balances
				if( curr->getBalance() == -1 ){
					p->setBalance(0);
					curr->setBalance(0);
					g->setBalance(1);
				}
				else if( curr->getBalance() == 0 ){
					p->setBalance(0);
					curr->setBalance(0);
					g->setBalance(0);
				}
				else if( curr->getBalance() == 1 ){
					p->setBalance(-1);
					curr->setBalance(0);
					g->setBalance(0);
				}
			}

		}
	}
	else if( isRightChild(p, p->getParent() ) == true ){
		//p is the right child of g (curr's parent is right child of curr's grandfather node)
		if(p->getParent()->getBalance() == 1){
			insertFix(p->getParent() , p);
		}
		else{
			//balance of grandparent is -2

			//zig zig
			if(isZigZig( p, p->getParent() ) ){
				AVLNode<Key,Value>* g = p->getParent(); 
				rotateLeft( p , p->getParent()  );
				//setting balances
				g->setBalance(0);
				p->setBalance(0);
			}

			else if( isZigZag( p, p->getParent() )){

				AVLNode<Key,Value>* g = p->getParent(); 
				rotateRight(curr, p);

				rotateLeft( p->getParent(), p->getParent()->getParent() ); 

				//setting balances
				if( curr->getBalance() == 1 ){
					p->setBalance(0);
					curr->setBalance(0);
					g->setBalance(-1);
				}
				else if( curr->getBalance() == 0 ){
					p->setBalance(0);
					curr->setBalance(0);
					g->setBalance(0);
				}
				else if( curr->getBalance() == -1 ){
					//like else
					p->setBalance(1);
					curr->setBalance(0);
					g->setBalance(0);
				}

			}

		}
	}
	
}

template<class Key, class Value>
bool  AVLTree<Key, Value>:: isZigZig(AVLNode<Key,Value>* p, AVLNode<Key,Value>* g  ){

	if( g->getBalance() == -2  ){
		if( p->getBalance() == 1 ){
			return false; // is zig zag
		}
		else if( p->getBalance() == -1){
			return true; // is zig zig
		}
	}

	else if( g->getBalance() == 2  ){
		if( p->getBalance() == -1 ){
			return false; // is zig zag
		}
		else if( p->getBalance() == 1){
			return true; // is zig zig
		}
	}
	//wont get here
	return false;


}

template<class Key, class Value>
bool  AVLTree<Key, Value>:: isZigZag(AVLNode<Key,Value>* p, AVLNode<Key,Value>* g  ){
	if( g->getBalance() == -2  ){
		if( p->getBalance() == 1 ){
			return true; // is zig zag
		}
		else if( p->getBalance() == -1){
			return false; // is zig zig
		}
	}

	else if( g->getBalance() == 2  ){
		//like else statemetn
		if( p->getBalance() == -1 ){
			return true; // is zig zag
		}
		else if( p->getBalance() == 1){
			return false; // is zig zig
		}
	}	
	//wont get here
	return false;


}

//performing rotation
template<class Key, class Value>
void AVLTree<Key, Value>:: rotateLeft( AVLNode<Key,Value>* p, AVLNode<Key,Value>* g ){
	if( g->getParent() != NULL  ){
		//setting great grandparent's child 
		if( isLeftChild(g, g->getParent())){
			g->getParent()->setLeft( p );
		}
		if(  isRightChild(g, g->getParent()) ){
			g->getParent()->setRight( p );
		}
	}
	else if(g->getParent() == NULL){
		//g is root
		 BinarySearchTree<Key, Value>::root_ = p;
	}

	if(p->getLeft() != NULL){ 
		p->getLeft()->setParent(g); //changing "b"'s parent on the slide
	}

		g->setRight(p->getLeft()); //changing g to have 'b' as right child


	p->setParent(g->getParent() );
	p->setLeft( g );
	g->setParent(p);

	
}


//performing rotation
template<class Key, class Value>
void AVLTree<Key, Value>:: rotateRight( AVLNode<Key,Value>* p, AVLNode<Key,Value>* g ){
	if(g->getParent() != NULL){
		//setting great grandparent's child 
		if( isLeftChild(g, g->getParent())){
			g->getParent()->setLeft( p );
		}
		else if(  isRightChild(g, g->getParent()) ){
			g->getParent()->setRight( p );
		}
	}
	else if(g->getParent() == NULL){
		//g is root
		 BinarySearchTree<Key, Value>::root_ = p;
	}
	if(p->getRight() != NULL){ 
		p->getRight()->setParent(g); 
	}
	
	g->setLeft(p->getRight()); // setting g's left to be c
	p->setParent(g->getParent()); 
	p->setRight(g);
	g->setParent(p);
	
}




template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{

	Node<Key, Value>*  temp = BinarySearchTree<Key, Value>::root_;
	AVLNode<Key,Value>* curr = (AVLNode<Key,Value>*) temp ;
	
	if(BinarySearchTree<Key, Value>::root_ == NULL){
		return;
	}
	if(BinarySearchTree<Key, Value>::root_->getKey() == key){
		if( BinarySearchTree<Key, Value>::root_->getLeft() == NULL && BinarySearchTree<Key, Value>::root_->getRight() == NULL  ){
			BinarySearchTree<Key, Value>::root_ = NULL;
			delete curr;
			
		}
		else if( curr->getLeft() == NULL && curr->getRight() != NULL  ){
			//right node present
			BinarySearchTree<Key, Value>::root_ = curr->getRight();
			BinarySearchTree<Key, Value>::root_->setParent(NULL);
			delete curr;

		
		}
		else if( curr->getLeft() != NULL && curr->getRight() == NULL  ){
			//left node present
			BinarySearchTree<Key, Value>::root_ = curr->getLeft();
			BinarySearchTree<Key, Value>::root_->setParent(NULL);
			delete curr;
			
		}
		else{
			//has 2 nodes
			
			//curr has 2 child nodes
			//swap with predecessor, then delete node
			Node<Key, Value>*  pred = BinarySearchTree<Key,Value>::predecessor(curr);
			
			char diff;
			nodeSwap(static_cast<AVLNode<Key, Value>*>(pred), curr); //swapping pred node and curr node

			//curr is now a leaf node
			if(curr->getParent()->getRight() == curr){ //curr is right node
				diff = -1;

				if(curr->getLeft() != NULL){
					//the original pred had a left child
					curr->getParent()->setRight(curr->getLeft());
					curr->getLeft()->setParent( curr->getParent() );
				}
				else{
					//originally had right

					curr->getParent()->setRight(NULL);
				}
			}
			else{ //curr is left node
				diff = 1;
				if(curr->getLeft() != NULL){
					curr->getParent()->setLeft(curr->getLeft()); 
					curr->getLeft()->setParent( curr->getParent() );
				}
				else{
					curr->getParent()->setLeft(NULL) ;
				}
			}
			BinarySearchTree<Key, Value>::root_ = pred;
			BinarySearchTree<Key, Value>::root_->setParent(NULL);
			AVLNode<Key, Value>* parent = curr->getParent();
			delete curr;
			removeFix(parent, diff );
		}

		return;
	}
	removeHelper( key, static_cast<AVLNode<Key, Value>*>(curr ) );

	
}

template<class Key, class Value>
void AVLTree<Key, Value>:: removeHelper(const Key& delKey, AVLNode<Key, Value>* curr){
	if(curr->getKey() == delKey ){

		if( curr->getLeft() == NULL && curr->getRight() != NULL  ){
			//curr only has right child
			if( (curr->getParent() )->getLeft() == curr){
				//curr is parent's left
				AVLNode<Key,Value>* p = curr->getParent();
				(curr->getParent() )->setLeft( curr->getRight()  );	
				curr->getRight()->setParent(  curr->getParent()  );	
				removeFix(p, 1);
			
			}
			else{
				//curr is parent's right
				AVLNode<Key,Value>* p = curr->getParent();
				curr->getParent()->setRight( curr->getRight() ) ;	
				curr->getRight()->setParent(  curr->getParent()  );			
				removeFix(p, -1);
			}
			
			delete curr;
		}
		else if( curr->getRight() == NULL && curr->getLeft() != NULL  ){
			//curr only has left child
			if( (curr->getParent() )->getLeft() == curr){
				//curr is parents left child
				AVLNode<Key,Value>* p = curr->getParent();
				(curr->getParent() )->setLeft(curr->getLeft());	
				removeFix(p, 1);	
			}
			else{
				//curr is parent's right
				AVLNode<Key,Value>* p = curr->getParent();
				curr->getParent()->setRight( curr->getLeft()  );	
				removeFix(p, -1);			
			}
			curr->getLeft()->setParent( curr->getParent() );
			delete curr;
		}
		else if( curr->getRight() == NULL && curr->getLeft() == NULL  ){
			//curr has no children / it is a leaf node
			
			if(curr->getParent()->getRight() == curr){
				//curr is right child of parent
				AVLNode<Key,Value>* p = curr->getParent();
				curr->getParent()->setRight( NULL );
				removeFix(p, -1);
			}
			else{
				//curr is its parents left child
				AVLNode<Key,Value>* p = curr->getParent();
				curr->getParent()->setLeft(NULL);
				removeFix(p, 1);
			}
			delete curr;
		}
		else{
			//curr has 2 child nodes
			//swap with predecessor, then delete node
			Node<Key, Value>* pred = BinarySearchTree<Key,Value>::predecessor(curr);
			nodeSwap(static_cast<AVLNode<Key, Value>*>(pred), curr); //swapping pred node and curr node
			if(curr->getParent()->getRight() == curr){
				AVLNode<Key,Value>* p = curr->getParent();
				if(curr->getLeft() != NULL){
					curr->getParent()->setRight(curr->getLeft()); 
					curr->getLeft()->setParent( curr->getParent() );
				}
				else{
					curr->getParent()->setRight(NULL);
				}
				removeFix(p, -1);
			}
			else{
				AVLNode<Key,Value>* p = curr->getParent();
				if(curr->getLeft() != NULL){
					curr->getParent()->setLeft(curr->getLeft()); 
					curr->getLeft()->setParent( curr->getParent() );
				}
				else {
					curr->getParent()->setLeft(NULL) ;
				}
				removeFix(p, 1);	
			}

			delete curr;

		}
		return;
	}


	if(curr->getLeft() == NULL && curr->getRight() == NULL){
		//the key is not in the tree, so do nothing
		return;
	}

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

//Does rotations and sets balances so the AVL tree is still balanced after a removal
template<class Key, class Value>
void AVLTree<Key, Value>:: removeFix(AVLNode<Key,Value>* node, char diff ){
	if(node == NULL ){
		return;
	}
	char ndiff;
	AVLNode<Key,Value>* p = NULL;
	if(node->getParent() != NULL){
		p = node->getParent();
	}
	if( p!= NULL &&  isRightChild(node,p)){ 
		ndiff = -1;
	}
	else if(p!= NULL && isLeftChild(node, p)){
		ndiff = 1;
	}
	
	
	if( diff == 1 ){

		
		if( (node->getBalance() + diff )   == 2){
			//c is the taller of the children
			AVLNode<Key,Value>* c = node->getRight(); 

			if(c->getBalance() == 1){
				rotateLeft(c, node);

				//setting balances
				node->setBalance(0);
				c->setBalance(0);
				removeFix(p, ndiff);
			}
			else if(c->getBalance() == 0){
				
				rotateLeft(c, node);
				//setting balances
				node->setBalance(1);
				c->setBalance(-1);
			}

			else if(c->getBalance() == -1  ){
				AVLNode<Key,Value>* g = c->getLeft();
				rotateRight( g, c ); 
				rotateLeft( g, node  );

				//setting balances
				if(g->getBalance() == -1){
					node->setBalance(0);
					c->setBalance(1);
					g->setBalance(0);
				}		

				else if(g->getBalance() == 0){
					node->setBalance(0);
					c->setBalance(0);
					g->setBalance(0);
				}			
				else if(g->getBalance() == 1){
					node->setBalance(-1);
					c->setBalance(0);
					g->setBalance(0);
				}
				removeFix(p, ndiff);

			}


					
		}

		else if( (node->getBalance() + diff) == 1){
			node->setBalance(1);
		}
		else if( (node->getBalance() + diff) == 0  ){
			node->setBalance( 0 ) ;
			removeFix(p, ndiff);

		}
	}



	else if( diff == -1 ){
		
		if( (node->getBalance() + diff )   == -2){
			//c is the taller of the children
			AVLNode<Key,Value>* c = node->getLeft(); 

			if(c->getBalance() == -1){
				rotateRight(c, node);

				node->setBalance(0);
				c->setBalance(0);
				removeFix(p, ndiff);
			}
			else if(c->getBalance() == 0){
				
				rotateRight(c, node);
				node->setBalance(-1);
				c->setBalance(1);
			}

			else if(c->getBalance() == 1  ){
			
				AVLNode<Key,Value>* g = c->getRight();
				rotateLeft( g, c );
				rotateRight( g, node  );

				if(g->getBalance() == 1){
					node->setBalance(0);
					c->setBalance(-1);
					g->setBalance(0);
				}		

				else if(g->getBalance() == 0){
					node->setBalance(0);
					c->setBalance(0);
					g->setBalance(0);
				}			
				else if(g->getBalance() == -1){
					node->setBalance(1);
					c->setBalance(0);
					g->setBalance(0);
				}	
				removeFix(p, ndiff);
			}
			


		}

		else if( (node->getBalance() + diff) == -1){
			node->setBalance(-1);

		}
		
		else if( (node->getBalance() + diff) == 0  ){
			node->setBalance( 0 ) ;
			removeFix(p, ndiff);

		}
	}

}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    char tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
