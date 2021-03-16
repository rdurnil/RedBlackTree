//----------------------------------------------------------------------
// FILE: bst_collection.h
// NAME: Rie Durnil
// DATE: November, 2020
// DESC: Implements a version of the collection class, in which a binary 
//        search tree is implemented. Key-value pairs are stored in nodes, 
//        and the keys remain sorted in the tree.
//----------------------------------------------------------------------

#ifndef BST_COLLECTION_H
#define BST_COLLECTION_H

#include "collection.h"
#include "array_list.h"

template<typename K, typename V>
class BSTCollection : public Collection<K,V> 
{
  public:
    BSTCollection();
    BSTCollection(const BSTCollection<K,V>& rhs);
    ~BSTCollection();
    BSTCollection& operator=(const BSTCollection<K,V>& rhs);
  
    void add(const K& key, const V& val);
    void remove(const K& key);
    bool find(const K& key, V& val) const;
    void find(const K& k1, const K& k2, ArrayList<K>& keys) const;
    void keys(ArrayList<K>& all_keys) const;
    void sort(ArrayList<K>& all_keys_sorted) const;
    size_t size() const;
    size_t height() const;
  
  private:
    struct Node {
      K key;
      V value;
      Node* left;
      Node* right;
    };
    // root node of the tree
    Node* root;
    // number of k-v pairs stored in the collection
    size_t node_count;
    // remove all elements in the bst
    void make_empty(Node* subtree_root);
    // copy helper
    void copy(Node* lhs_subtree_root, const Node* rhs_subtree_root);
    // remove helper
    Node* remove(Node* subtree_root, const K& a_key);
    // helper to recursively build up key list
    void find(const Node* subtree_root, const K& k1, const K& k2, ArrayList<K>& keys) const;
    // helper to recursively build sorted list of keys
    void keys(const Node* subtree_root, ArrayList<K>& all_keys) const;
    // helper to recursively find height of the tree
    size_t height(const Node* subtree_root) const;
};

template<typename K, typename V>
BSTCollection<K,V>::BSTCollection() {
  root = nullptr;
  node_count = 0;
}

template<typename K, typename V>
BSTCollection<K,V>::BSTCollection(const BSTCollection<K,V>& rhs){
  // defer to assignment operator
  root = nullptr;
  *this = rhs;
}

template<typename K, typename V>
BSTCollection<K,V>::~BSTCollection() {
  make_empty(root);
  root = nullptr;
}

template<typename K, typename V>
BSTCollection<K,V>& BSTCollection<K,V>::operator=(const BSTCollection<K,V>& rhs) {
  if (this != &rhs) {
    make_empty(root);
    root = nullptr;
    copy(root, rhs.root);
    node_count = rhs.node_count;
  }
  return *this;
}
 
template<typename K, typename V>
void BSTCollection<K,V>::add(const K& k, const V& val) {
  Node* curr = root;
  Node* prev = nullptr;
  //create new node
  Node* new_node = new Node;
  new_node->key = k;
  new_node->value = val;
  new_node->left = nullptr;
  new_node->right = nullptr;
  //if the new node is the first in the tree
  if (curr == nullptr) {
    root = new_node;	
  } else {
    //find correct place for new key
    while (curr != nullptr) {
      if (curr->key > k) {
        prev = curr;
        curr = curr->left;
      } else {
        prev = curr;
        curr = curr->right;
      } 
    }
    //insert it into place
    if (prev->key > k) {
  	  prev->left = new_node;
    } else {
  	  prev->right = new_node;
    }
  }
  //update node_count
  node_count++;
}

template<typename K, typename V>
void BSTCollection<K,V>::remove(const K& key) {
  //call helper function
  root = remove(root, key);
}

template<typename K, typename V>
bool BSTCollection<K,V>::find(const K& key, V& val) const {
  Node* curr = root;
  while (curr != nullptr) {
    if (curr->key == key) {
      val = curr->value;
      return true;
    } else if (curr->key > key) {
      curr = curr->left;
    } else if (curr->key < key) {
      curr = curr->right;
    }
  }
  //if curr became null
  return false;
}

template<typename K, typename V>
void BSTCollection<K,V>::find(const K& k1, const K& k2, ArrayList<K>& keys) const {
  //call helper function
  find(root, k1, k2, keys);
}

template<typename K, typename V>
void BSTCollection<K,V>::keys(ArrayList<K>& all_keys) const {
  //call helper function
  keys(root, all_keys);
}

template<typename K, typename V>
void BSTCollection<K,V>::sort(ArrayList<K>& all_keys_sorted) const {
  //call helper function
  keys(root, all_keys_sorted);
}

template<typename K, typename V>
size_t BSTCollection<K,V>::size() const {
  return node_count;
}

template<typename K, typename V>
size_t BSTCollection<K,V>::height() const {
  return height(root);
}

template<typename K, typename V>
void BSTCollection<K,V>::make_empty(Node* subtree_root) {
  //base case
  if (subtree_root == nullptr) {
    return;
  }
  //call on left and right subtrees
  make_empty(subtree_root->left);
  make_empty(subtree_root->right);
  delete subtree_root;
}

template<typename K, typename V>
void BSTCollection<K,V>::copy(Node* lhs_subtree_root, const Node* rhs_subtree_root) {
  //base case
  if (rhs_subtree_root == nullptr) {
    return;
  }
  //add each node
  add(rhs_subtree_root->key, rhs_subtree_root->value);
  copy(lhs_subtree_root, rhs_subtree_root->left);
  copy(lhs_subtree_root, rhs_subtree_root->right);
}

template<typename K, typename V>
typename BSTCollection<K,V>::Node*
BSTCollection<K,V>::remove(Node* subtree_root, const K& a_key) {
  //find right node to remove
  if (subtree_root && a_key < subtree_root->key) {
    subtree_root->left = remove(subtree_root->left, a_key);
  } else if (subtree_root && a_key > subtree_root->key) {
    subtree_root->right = remove(subtree_root->right, a_key);
  //once found  
  } else if (subtree_root) {
    Node* curr = subtree_root;
    Node* prev = nullptr;
    //if node has two children
    if (subtree_root->left && subtree_root->right) {
      //finding in order successor
      prev = curr;
      curr = subtree_root->right;
      while (curr != nullptr) {
        prev = curr;
        curr = curr->left;
      }
      //copy over data and remove in order successor
      subtree_root->key = prev->key;
      subtree_root->value = prev->value;
      subtree_root->right = remove(subtree_root->right, prev->key);
      return subtree_root;
    } else {
      //one or zero children
      if (subtree_root->right) {
        curr = curr->right;
        delete subtree_root;
        node_count--;
        return curr;        
      } else {
        curr = curr->left;
        delete subtree_root;
        node_count--;
        return curr;
      }
    }
  }
  return subtree_root;
}

template<typename K, typename V>
void BSTCollection<K,V>::find(const Node* subtree_root, const K& k1, const K& k2,
ArrayList<K>& keys) const {
  //base case
  if (subtree_root == nullptr) {
    return;
  } else if (subtree_root->key < k1) {
  //search right subtree
    find(subtree_root->right, k1, k2, keys);
  } else if (subtree_root->key > k2) {
  //search left subtree
    find(subtree_root->left, k1, k2, keys);
  } else {
  //add keys, search right and left subtrees
    keys.add(subtree_root->key);
    find(subtree_root->left, k1, k2, keys);
    find(subtree_root->right, k1, k2, keys);
  }
}

template<typename K, typename V>
void BSTCollection<K,V>::keys(const Node* subtree_root, ArrayList<K>& all_keys) const {
  //check if done
  if (subtree_root == nullptr) {
    return;
  }
  //visit left subtree
  keys(subtree_root->left, all_keys);
  //add subtree_root to array
  all_keys.add(subtree_root->key);
  //visit right subtree
  keys(subtree_root->right, all_keys);
}

template<typename K, typename V>
size_t BSTCollection<K,V>::height(const Node* subtree_root) const {
  //base case
  if (subtree_root == nullptr) {
    return 0;
  }
  size_t l_height = height(subtree_root->left);
  size_t r_height = height(subtree_root->right);
  //using the max height
  if (l_height > r_height) {
    return 1 + l_height;
  } else {
    return 1 + r_height;
  }
}

#endif
    
