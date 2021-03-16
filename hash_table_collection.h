//----------------------------------------------------------------------
// FILE: hash_table_collection.h
// NAME: Rie Durnil
// DATE: November, 2020
// DESC: Implements a version of the collection class, in which a hash table
//        is implemented using an array of linked lists. Key-value pairs are
//        stored in nodes, and the key is used to hash it into the table.
//----------------------------------------------------------------------

#ifndef HASH_TABLE_COLLECTION_H
#define HASH_TABLE_COLLECTION_H

#include "collection.h"
#include "array_list.h"
#include <functional>
#include <iostream>

using namespace std;

template<typename K, typename V>
class HashTableCollection : public Collection<K,V> 
{
  public:
    HashTableCollection();
    HashTableCollection(const HashTableCollection<K,V>& rhs);
    ~HashTableCollection();
    HashTableCollection& operator=(const HashTableCollection<K,V>& rhs);
  
    void add(const K& key, const V& val);
    void remove(const K& key);
    bool find(const K& key, V& val) const;
    void find(const K& k1, const K& k2, ArrayList<K>& keys) const;
    void keys(ArrayList<K>& all_keys) const;
    void sort(ArrayList<K>& all_keys_sorted) const;
    size_t size() const;
    size_t min_chain_length();
    size_t max_chain_length();
    double avg_chain_length();
		
  private:
    struct Node {
      K key;
      V value;
      Node* next;
    };
    Node** hash_table;
    size_t length;
    size_t table_capacity;
    double load_factor_threshold = 0.75;
    void resize_and_rehash(); 
    void make_empty();
};

template<typename K, typename V>
HashTableCollection<K,V>::HashTableCollection() 
  : table_capacity(16), length(0)
{
  hash_table = new Node*[table_capacity];
  //make each bucket null
  for (size_t i = 0; i < table_capacity; ++i) {
    hash_table[i] = nullptr;
  }
}

template<typename K, typename V>
HashTableCollection<K,V>::HashTableCollection(const HashTableCollection<K,V>& rhs) {
  // defer to assignment operator
  hash_table = nullptr;
  *this = rhs;
}

template<typename K, typename V>
HashTableCollection<K,V>::~HashTableCollection() {
  make_empty();
}
    
template<typename K, typename V>
HashTableCollection<K,V>& HashTableCollection<K,V>::operator=
(const HashTableCollection<K,V>& rhs) {
  if (this != &rhs) {
  	make_empty();
    //check to see if rhs is empty
    if (rhs.length != 0) {
      table_capacity = rhs.table_capacity;
      length = 0;
      hash_table = new Node*[table_capacity];
      //set each bucket to null
      for (size_t i = 0; i < table_capacity; ++i) {
        hash_table[i] = nullptr;
      }
      Node * curr = nullptr;
      //add each Node
      for (size_t i = 0; i < table_capacity; ++i) {
        curr = rhs.hash_table[i];
        while (curr != nullptr) {
          add(curr->key, curr->value);
          curr = curr->next;
        }
      } 
    }
  }
  return *this;
}
  
template<typename K, typename V>
void HashTableCollection<K,V>::add(const K& k, const V& val) {
  // check to see if table needs to be resized
  if (length * 1.0 / table_capacity >= load_factor_threshold) {
  	resize_and_rehash();
  }
  // hash new key
  std::hash<K> hash_fun;
  size_t code = hash_fun(k);
  size_t index = code % table_capacity;
  // create new node and assign values
  Node* new_node = new Node;
  new_node->key = k;
  new_node->value = val;
  new_node->next = hash_table[index];
  hash_table[index] = new_node;
  length = length + 1;
}
    
template<typename K, typename V>
void HashTableCollection<K,V>::remove(const K& k) {
  // find which bucket key had been hashed to
  std::hash<K> hash_fun;
  size_t code = hash_fun(k);
  size_t index = code % table_capacity;
  // find key within linked list in hashed bucket
  Node* curr = hash_table[index];
  Node* prev = nullptr;
  while (curr != nullptr && curr->key != k) {
    prev = curr;
    curr = curr->next;
  }
  // make sure key was found
  if (curr != nullptr) {
    if (prev != nullptr) {
      prev->next = curr->next;
    } else {
      hash_table[index] = curr->next;
    }
    delete curr;
    length = length - 1;
  }
}

template<typename K, typename V>
bool HashTableCollection<K,V>::find(const K& k, V& v) const {
  // find correct bucket
  std::hash<K> hash_fun;
  size_t code = hash_fun(k);
  size_t index = code % table_capacity;
  // search through linked list at bucket
  Node* curr = hash_table[index];
  while (curr != nullptr && curr->key != k) {
    curr = curr->next;
  }
  // see if key was found
  if (curr != nullptr) {
    v = curr->value;
    return true;
  } else {
  	return false;
  }
}

template<typename K, typename V>
void HashTableCollection<K,V>::find(const K& k1, const K& k2, ArrayList<K>& keys) const {
  // search through every single node in list
  Node* curr = nullptr;
  for (size_t i = 0; i < table_capacity; ++i) {
    curr = hash_table[i];
    // if key is between k1 and k2, add to keys
    while (curr != nullptr) {
      if (curr->key >= k1 && curr->key <= k2) {
        keys.add(curr->key);
      }
      curr = curr->next;
    }
  }
}

template<typename K, typename V>
void HashTableCollection<K,V>::keys(ArrayList<K>& all_keys) const {
  // add every single node to all_keys
  Node * curr = nullptr;
  for (int i = 0; i < table_capacity; ++i) {
    curr = hash_table[i];
    while (curr != nullptr) {
      all_keys.add(curr->key);
      curr = curr->next;
    }
  }
}

template<typename K, typename V>
void HashTableCollection<K,V>::sort(ArrayList<K>& all_keys_sorted) const {
  //get all keys
  keys(all_keys_sorted);
  //sort resulting array
  all_keys_sorted.sort();
}

template<typename K, typename V>
size_t HashTableCollection<K,V>::size() const {
  return length;
}

template<typename K, typename V>
size_t HashTableCollection<K,V>::min_chain_length() {
  // if list is empty, longest chain is zero
  if (length == 0) {
  	return 0;
  }
  size_t count = 0;
  size_t min = 0;
  Node* curr = nullptr;
  // count nodes at each bucket
  for (size_t i = 0; i < table_capacity; ++i) {
    curr = hash_table[i];
    while (curr != nullptr) {
      count++;
      curr = curr->next;
    }
    // if new minimum is found, update min
    if (min == 0 || min > count) {
      min = count;
    }
    //reset count for next bucket
    count = 0;
  }
  return min;
}

template<typename K, typename V>
size_t HashTableCollection<K,V>::max_chain_length() {
  // if list is empty, longest chain is zero
  if (length == 0) {
  	return 0;
  }
  size_t count = 0;
  size_t max = 0;
  Node* curr = nullptr;
  // count nodes at each bucket
  for (size_t i = 0; i < table_capacity; ++i) {
    curr = hash_table[i];
    while (curr != nullptr) {
      count++;
      curr = curr->next;
    }
    // if a new max is found, update max
    if (max < count) {
      max = count;
    }
    // reset count
    count = 0;
  }
  return max;
}

template<typename K, typename V>
double HashTableCollection<K,V>::avg_chain_length() {
  // if list is empty, no chains to count
  if (length == 0) {
  	return 0;
  }
  size_t count = 0;
  size_t sum;
  Node* curr = nullptr;
  //keep a continual count through every bucket
  for (size_t i = 0; i < table_capacity; ++i) {
    curr = hash_table[i];
    while (curr != nullptr) {
      count++;
      curr = curr->next;
    }
  }
  // compute and return the average
  return count * 1.0 / table_capacity;
}

template<typename K, typename V>
void HashTableCollection<K,V>::resize_and_rehash() {
  //create new array and set each bucket to null
  Node** copy = new Node*[2 * table_capacity];
  for (size_t i = 0; i < 2*table_capacity; ++i) {
    copy[i] = nullptr;
  }
  Node* curr;
  std::hash<K> hash_fun;
  size_t code;
  size_t index;
  //go through each node and rehash it into new array
  for (size_t i = 0; i < table_capacity; ++i) {
    curr = hash_table[i];
    while (curr != nullptr) {
      code = hash_fun(curr->key);
      index = code % (2 * table_capacity);
      Node* temp = curr->next;
      curr->next = copy[index];
      copy[index] = curr;
      curr = temp;
    }
  }
  //hold the length
  size_t l = length;
  //set hash table to copy array
  hash_table = copy;
  length = l;
  copy = nullptr;
  table_capacity = 2 * table_capacity;
}

template<typename K, typename V>
void HashTableCollection<K,V>::make_empty() {
  if (hash_table != nullptr) {
    Node * curr = nullptr;
    Node * next_node = nullptr;
    // delete linked list at each bucket
    for (size_t i = 0; i < table_capacity; ++i) {
  	  curr = hash_table[i];
      while (curr != nullptr) {
        next_node = curr->next;
        delete curr;
        curr = next_node;
      }
    }
    // delete array
    delete [] hash_table;
    hash_table = nullptr;
    length = 0;
    table_capacity = 0;
  }
}

#endif
