//----------------------------------------------------------------------
// FILE: linked_list.h
// NAME: Rie Durnil
// DATE: October, 2020
// DESC: Implements a linked list version of the list class. Elements
//       are added by default to the end of the list via a tail pointer.
//		 Implementations for selection, insertion, merge, and quick sort
//       are included.
//----------------------------------------------------------------------


#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "list.h"
#include <iostream>

using namespace std;

template<typename T>
class LinkedList : public List<T>
{
public:
  LinkedList();
  LinkedList(const LinkedList<T>& rhs);
  ~LinkedList();
  LinkedList& operator=(const LinkedList<T>& rhs);

  void add(const T& item);
  bool add(size_t index, const T& item);
  bool get(size_t index, T& return_item) const;
  bool set(size_t index, const T& new_item);
  bool remove(size_t index);
  size_t size() const;
  void selection_sort();
  void insertion_sort();
  void merge_sort();
  void quick_sort();
  void sort();

private:
  struct Node {
    T value;
    Node* next;
  };
  Node* head;
  Node* tail;
  size_t length;

  //helper functions
  void make_empty();
  Node* merge_sort(Node* left, int len);
  Node* quick_sort(Node* start, int len);
};


template<typename T>
LinkedList<T>::LinkedList()
  : head(nullptr), tail(nullptr), length(0)
{
}


template<typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& rhs)
  : head(nullptr), tail(nullptr), length(0)
{
  // defer to assignment operator
  *this = rhs;
}


// TODO: Finish the remaining functions below
template<typename T>
LinkedList<T>::~LinkedList() {
  make_empty();
}
  
template<typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& rhs){
  if (this != &rhs) {
  	make_empty();
  	//check to see if rhs is empty
  	if (rhs.head != nullptr){
  		length = rhs.length;
  		Node * curr_node = rhs.head;
  		head = new Node;
  		head->value = curr_node->value;
  		Node * copy_node = head;
  		//loop until at last element
  		while (curr_node->next != nullptr) {
  			copy_node->next = new Node;
  			copy_node->next->value = curr_node->next->value;
  			copy_node = copy_node->next;
  			curr_node = curr_node->next;
  		}
  		copy_node->next = nullptr;
  		this->tail = copy_node;
  	}
  }
  
  return *this;
}
  
template<typename T>
void LinkedList<T>::add(const T& item){
  Node * new_node = new Node;
  new_node->value = item;
  //if the list is empty
  if (length == 0) {
  	head = new_node;
  	tail = new_node;
  } else {
  	tail->next = new_node;
  	tail = new_node;
  }
  length++;
  new_node->next = nullptr;
}
  
template<typename T>
bool LinkedList<T>::add(size_t index, const T& item){
  //boolean condition
  if (index > length){
  	return false;
  }
  Node * new_node = new Node;
  Node * curr_node = head;
  Node * prev_node = head;
  new_node->value = item;
  new_node->next = nullptr;
  //see if add is at front of list
  if (index == 0) {
  	new_node->next = head;
  	head = new_node;
  	if (tail == nullptr) {
  		tail = head;
  	}
  } else {
  	//get to right element if not
  	for (int i = 1; i <= index; i++) {
  		prev_node = curr_node;
  		curr_node = curr_node->next;
  	}
  	//update pointers to add new node into list
  	prev_node->next = new_node;
  	new_node->next = curr_node;
  	if (prev_node == tail) {
  		tail = new_node;
  	}
  }
  length++;
  return true;
}
  
template<typename T>
bool LinkedList<T>::get(size_t index, T& return_item) const{
  //boolean condition
  if (index >= length) {
  	return false;
  }
  Node * curr_node = head;
  //get to right index in list
  for (int i = 1; i <= index; i++) {
  	curr_node = curr_node->next;
  }
  return_item = curr_node->value;
  return true;
}
  
template<typename T>
bool LinkedList<T>::set(size_t index, const T& new_item){
  //boolean condition
  if (index >= length) {
  	return false;
  }
  Node * curr_node = head;
  //get to right index in list
  for (int i = 1; i <= index; i++) {
  	curr_node = curr_node->next;
  } 
  //update node to new value
  curr_node->value = new_item; 
  return true;
}
  
template<typename T>
bool LinkedList<T>::remove(size_t index){
  //boolean condition
  if (index >= length) {
  	return false;
  }  
  Node * curr_node = head;
  Node * prev_node = head;
  //check to see if node to remove is at front
  if (index == 0) {
  	head = head->next;
  	if (head == nullptr) {
  		tail = nullptr;
  	}
  	delete curr_node;
  } else {
  	//find element to delete
  	for (int i = 1; i <= index; i++) {
  		prev_node = curr_node;
		curr_node = curr_node->next;
  	}
  	prev_node->next = curr_node->next;
  	//check to see if node being deleted is tail
  	if (curr_node == tail) {
  		tail = prev_node;
  	}
  	delete curr_node;
  }
  //update length
  length--;
  	
  return true;
}

template<typename T>  
size_t LinkedList<T>::size() const {
  return this->length;
}

template<typename T>
void LinkedList<T>::make_empty(){
	Node * curr_node = head;
	Node * next_node = nullptr;

	while (curr_node != nullptr) {
		next_node = curr_node->next;
		delete curr_node;
		curr_node = next_node;
	}
	length = 0;
	head = nullptr;
	tail = nullptr;
}

template<typename T>
void LinkedList<T>::selection_sort(){
	Node * smallest = head;
	Node * curr_node = head;
	Node * prev_node = nullptr;
	Node * sorted = nullptr;
	//if list is empty
	if (length <= 1) {
		return;
	}
	for (int i = 0; i < length - 1; i++) {
		while (curr_node->next != nullptr) {
			if (smallest->value > curr_node->next->value) {
				prev_node = curr_node;
				smallest = curr_node->next;
			}
			curr_node = curr_node->next;
		}
		//if tail node will be moved
		if (smallest == tail) {
			tail = prev_node;
		}
		//for first time to set head
		if (sorted == nullptr && smallest != head) {
			prev_node->next = smallest->next;
			smallest->next = head;
			head = smallest;
		//every other time
		} else if (smallest != head) {
			prev_node->next = smallest->next;
			smallest->next = sorted->next;
			sorted->next = smallest;
		} 
		//reset pointers for next iteration
		sorted = smallest;
		smallest = smallest->next;
		curr_node = smallest;
		prev_node = sorted;
	}
}

template<typename T>
void LinkedList<T>::insertion_sort(){
	//returns if 0 or 1 items are in the list
	if (length <= 1) {
		return;
	}
	Node * sorted = head->next;
	Node * prev_node = head;
	Node * curr_node = head;
	Node * sorted_prev = head;
	Node* to_move = nullptr;
	while (sorted != nullptr) {
		while (curr_node != sorted) {
			if (curr_node->value >= sorted->value) {
				sorted_prev->next = sorted->next;
				to_move = sorted;
				sorted = sorted_prev;
				//if head needs to be updated
				if (curr_node == head) {
					head = to_move;
				}
				//if it's not the first element
				if (prev_node != curr_node) {
					prev_node->next = to_move;
				}
				to_move->next = curr_node;
				//check to see if tail needs to be updated
				if (sorted_prev->next == nullptr) {
					tail = sorted_prev;
				}
			} else {
				//if statement doesn't run the first time
				if (curr_node != prev_node) {
					prev_node = prev_node->next;
				}
				curr_node = curr_node->next;
			}
		}
		//reset pointers for next iteration
		sorted_prev = sorted;
		sorted = sorted->next;
		curr_node = head;
		prev_node = head;
	}
}

template<typename T>
void LinkedList<T>::merge_sort() {
	//check to see if list is already sorted (1 elem)
	if (length > 1) {
		head = merge_sort(head, length);
		Node* curr_node = head;
		while (curr_node->next != nullptr) {
			curr_node = curr_node->next;
		}
		tail = curr_node;
	}
}

template<typename T>
typename LinkedList<T>::Node*
LinkedList<T>::merge_sort(Node* left, int len) {
	//stops when list is already sorted (1 elem)
	if (len <= 1) {
		return left;
	}
	//compute mid length and set up right
	int mid_len = len / 2;
	Node* right = left;
	Node* prev = nullptr;
	for (int i = 0; i < mid_len; i++) {
		prev = right;
		right = right->next;
	}
	//separate left from right
	prev->next = nullptr;
	//recursive calls to keep splitting
	left = merge_sort(left, mid_len);
	right = merge_sort(right, (len - mid_len));
	//merge the two lists left and right
	Node* curr_node = nullptr;
	Node* new_head = nullptr;
	while (left != nullptr && right != nullptr) {
		if (left->value < right->value) {
			//if this is the first element of list
			if (new_head == nullptr) {
				new_head = left;
				left = left->next;
				curr_node = new_head;
			} else {
				curr_node->next = left;
				curr_node = curr_node->next;
				left = left->next;
			}
		} else {
			//if this is the first element of list
			if (new_head == nullptr) {
				new_head = right;
				right = right->next;
				curr_node = new_head;
			} else {
				curr_node->next = right;
				curr_node = curr_node->next;
				right = right->next;
			}
		}
	}
	//lists left & right are already sorted
	if (left != nullptr) {
		curr_node->next = left;
	}
	if (right != nullptr) {
		curr_node->next = right;
	}	
	//return pointer to first node of merged list
	return new_head;
}

template<typename T>
void LinkedList<T>::quick_sort() {
	//check to see if list is already sorted (1 elem)
	if (length > 1) {
		head = quick_sort(head, length);
		Node* curr_node = head;
		while (curr_node->next != nullptr) {
			curr_node = curr_node->next;
		}
		tail = curr_node;
	}	
}

template<typename T>
typename LinkedList<T>::Node*
LinkedList<T>::quick_sort(Node* start, int len) {
	Node * temp1 = nullptr;
	//if list is already sorted
	if (len <= 1) {
		return start;
	//if only 2 nodes, may need to be switched
	} else if (len == 2) {
		if (start->value > start->next->value) {
			temp1 = start->next;
			temp1->next = start;
			start->next = nullptr;
			start = temp1;
		}
		return start;
	} else {
		Node * pivot_node = start;
		Node * curr_node = start->next;
		pivot_node->next = nullptr;
		Node * smaller = nullptr;
		Node * larger = nullptr;
		Node * temp2 = nullptr;
		int smaller_len = 0;
		int larger_len = 0;
		//splitting into smaller and larger lists than pivot
		while (curr_node != nullptr) {
			if (curr_node->value < pivot_node->value) {
				//if smaller is empty
				if (smaller == nullptr) {
					smaller = curr_node;
					curr_node = curr_node->next;
					smaller->next = nullptr;
					temp1 = smaller;
				} else {
					temp1->next = curr_node;
					curr_node = curr_node->next;
					temp1 = temp1->next;
					temp1->next = nullptr;
				}
				smaller_len += 1;
			} else {
				//if larger is empty
				if (larger == nullptr) {
					larger = curr_node;
					curr_node = curr_node->next;
					larger->next = nullptr;
					temp2 = larger;
				} else {
					temp2->next = curr_node;
					curr_node = curr_node->next;
					temp2 = temp2->next;
					temp2->next = nullptr;
				}
				larger_len += 1;
			}
		}
		//recursive calls
		smaller = quick_sort(smaller, smaller_len);
		larger = quick_sort(larger, larger_len);
		//reconnect nodes
		curr_node = smaller;
		if (smaller != nullptr) {
			while (curr_node->next != nullptr) {
				curr_node = curr_node->next;
			}
			curr_node->next = pivot_node;
		} else {
			smaller = pivot_node;
		}
		pivot_node->next = larger;
		return smaller;
	}
}

template<typename T>
void LinkedList<T>::sort() {
	merge_sort();
}

#endif
