//----------------------------------------------------------------------
// FILE: array_list.h
// NAME: Rie Durnil
// DATE: October, 2020
// DESC: Implements a resizable array version of the list
//       class. Elements are added by default to the last available
//       index in the array. Implementations for selection, insertion,
//		 merge, and quick sort are included.
//----------------------------------------------------------------------

#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include "list.h"
#include <iostream>

using namespace std;

template<typename T>
class ArrayList : public List<T>
{
public:
  ArrayList();
  ArrayList(const ArrayList<T>& rhs);
  ~ArrayList();
  ArrayList& operator=(const ArrayList<T>& rhs);

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
  T* items;
  size_t capacity;
  size_t length;

  // helper to resize items array
  void resize();
  void merge_sort(int start, int end);
  void quick_sort(int start, int end);
};


template<typename T>
ArrayList<T>::ArrayList()
  : capacity(10), length(0)
{
  items = new T[capacity];
}


template<typename T>
ArrayList<T>::ArrayList(const ArrayList<T>& rhs)
  : capacity(10), length(0), items(nullptr)
{
  // defer to assignment operator
  *this = rhs;
}


// TODO: Finish the remaining functions below
template<typename T>
ArrayList<T>::~ArrayList(){
  delete [] items;
  length = 0;
}

template<typename T>
ArrayList<T>& ArrayList<T>::operator=(const ArrayList<T>& rhs){
  size_t i;
  if (this != &rhs) {
    items = new T[rhs.capacity];
    length = rhs.length;
    for (i = 0; i < rhs.length; i++) {
      items[i] = rhs.items[i];
    }
  }
  return *this;
}

template<typename T>  
void ArrayList<T>::add(const T& item){
  //check to see if array needs to be resized
  if (length < capacity) {
    items[length] = item;
  } else {
    this->resize();
    items[length] = item;
  }
  length++;
}

template<typename T>
bool ArrayList<T>::add(size_t index, const T& item){
  //boolean condition
  if (index > length) {
    return false;
  }
  size_t i;
  //check to see if array needs to be resized
  if (length == capacity - 1) {
    this->resize();
  }
  //shift elements over by one
  for (i = length; i > index; --i) {
    items[i] = items[i-1];
  }
  items[index] = item;
  length++;
	
  return true;
}

template<typename T>
bool ArrayList<T>::get(size_t index, T& return_item) const{
  //boolean condition
  if (index >= length) {
    return false;
  }
  return_item = this->items[index];
	
  return true;
}

template<typename T>
bool ArrayList<T>::set(size_t index, const T& new_item){
  //boolean condition
  if (index >= length) {
    return false;
  }
  //update element
  this->items[index] = new_item;
	
  return true;
}

template<typename T>
bool ArrayList<T>::remove(size_t index){
  //boolean condition
  if (index >= length || length == 0) {
    return false;
  }
  int i;
  //get rid of element at index and move all other elements over
  for (i = index; i < length - 1; i++) {
    items[i] = items[i + 1];
  }
  //update length
  length--;
	
  return true;
}

template<typename T>
size_t ArrayList<T>::size() const{
  return length;
}

template<typename T>
void ArrayList<T>::resize(){
  T * copy_arr = new T[2 * capacity];
  size_t i;
  for (i = 0; i < length; i++) {
    copy_arr[i] = items[i];
  }
  delete [] items;
  items = copy_arr;
  capacity *= 2;
}

template<typename T>
void ArrayList<T>::selection_sort(){
  for (int i = length - 1; i > 0; i--) {
    size_t index = 0;
    for (int j = 1; j <= i; j++) {
      if (items[j] > items[index]) {
        index = j;
      }
    }
    T largest = items[index];
    items[index] = items[i];
    items[i] = largest;
  }
}

template<typename T>
void ArrayList<T>::insertion_sort(){
  T curr_val;
  int j;
  for (int i = 1; i < length; i++) {
    curr_val = items[i];
    j = i - 1;
    while (j >= 0 && curr_val < items[j]) {
      items[j + 1] = items[j];
      j--;
    }
    items[j + 1] = curr_val;
  }
}

template<typename T>
void ArrayList<T>::merge_sort() {
  merge_sort(0, length - 1);
}

template<typename T>
void ArrayList<T>::merge_sort(int start, int end) {
  int mid;
  //if list has more than 1 element 
  if (start < end) {
    //split in half for recursive calls
    mid = (start + end) / 2;
    merge_sort(start, mid);
    merge_sort(mid + 1, end);
    //temporary array
    T temp[(end - start) + 1];
    int first1 = start;
    int first2 = mid + 1;
    int i = 0;
    //checking element by element for smallest
    while (first1 <= mid && first2 <= end) {
      if (items[first1] < items[first2]) {
        temp[i++] = items[first1++];
      } else {
        temp[i++] = items[first2++];
      }
    }
    //once one half is empty
    while (first1 <= mid) {
      temp[i++] = items[first1++];
    }
    while (first2 <= end) {
       temp[i++] = items[first2++];
    }
    //copying temp back into items
    for (i = 0; i <= (end - start); i++) {
      items[start + i] = temp[i];
    }
  }
}

template<typename T>
void ArrayList<T>::quick_sort() {
  quick_sort(0, length - 1);
}

template<typename T>
void ArrayList<T>::quick_sort(int start, int end) {
  T temp, pivot_val;
  int last_p1, i;
  //if list has more than 1 element 
  if (start < end) {
    pivot_val = items[start];
    last_p1 = start;
    for (i = (start + 1); i <= end; ++i) {
      if (items[i] < pivot_val) {
        last_p1 = last_p1 + 1;
        //swap items[i] and items[last_p1]
        temp = items[i];
        items[i] = items[last_p1];
        items[last_p1] = temp;
      }
    }
    //swap items[start] and items[last_p1]
    temp = items[start];
    items[start] = items[last_p1];
    items[last_p1] = temp;
    //recursive calls
    quick_sort(start, last_p1);
    quick_sort(last_p1 + 1, end);
  }
}

template<typename T>
void ArrayList<T>::sort() {
  quick_sort();
}

#endif
