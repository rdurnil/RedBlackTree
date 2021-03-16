//----------------------------------------------------------------------
// FILE: avl_collection.h
// NAME: Rie Durnil
// DATE: December, 2020
// DESC: Implements a version of the collection class, in which an AVL 
//        tree is implemented. Key-value pairs are stored in nodes, 
//        and the keys remain sorted in the tree. Tree remains balanced.
//----------------------------------------------------------------------

#ifndef AVL_COLLECTION_H
#define AVL_COLLECTION_H

#include "collection.h"
#include "array_list.h"

template<typename K, typename V>
class AVLCollection : public Collection<K,V> 
{
  public:
    AVLCollection();
    AVLCollection(const AVLCollection<K,V>& rhs);
    ~AVLCollection();
    AVLCollection& operator=(const AVLCollection<K,V>& rhs);
  
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
      int height;
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
    // add helper
    Node* add(Node* subtree_root, const K& a_key, const V& a_val);
    // remove helper
    Node* remove(Node* subtree_root, const K& a_key);
    // helper to recursively build up key list
    void find(const Node* subtree_root, const K& k1, const K& k2, ArrayList<K>& keys) const;
    // helper to recursively build sorted list of keys
    void keys(const Node* subtree_root, ArrayList<K>& all_keys) const;
    // rotate right helper
    Node* rotate_right(Node* k2);
    // rotate left helper
    Node* rotate_left(Node* k2);
    // rebalance the subtree rooted at subtree_root
    Node* rebalance(Node* subtree_root);
    void print_tree(std::string indent, Node* subtree_root);
};

template<typename K, typename V>
AVLCollection<K,V>::AVLCollection() {
  root = nullptr;
  node_count = 0;
}

template<typename K, typename V>
AVLCollection<K,V>::AVLCollection(const AVLCollection<K,V>& rhs){
  // defer to assignment operator
  root = nullptr;
  *this = rhs;
}

template<typename K, typename V>
AVLCollection<K,V>::~AVLCollection() {
  make_empty(root);
  root = nullptr;
}

template<typename K, typename V>
AVLCollection<K,V>& AVLCollection<K,V>::operator=(const AVLCollection<K,V>& rhs) {
  if (this != &rhs) {
    make_empty(root);
    root = nullptr;
    copy(root, rhs.root);
    node_count = rhs.node_count;
  }
  return *this;
}
 
template<typename K, typename V>
void AVLCollection<K,V>::add(const K& k, const V& val) {
  //call helper function
  root = add(root, k, val);
  node_count++;
}

template<typename K, typename V>
void AVLCollection<K,V>::remove(const K& key) {
  //call helper function
  root = remove(root, key);
}

template<typename K, typename V>
bool AVLCollection<K,V>::find(const K& key, V& val) const {
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
void AVLCollection<K,V>::find(const K& k1, const K& k2, ArrayList<K>& keys) const {
  //call helper function
  find(root, k1, k2, keys);
}

template<typename K, typename V>
void AVLCollection<K,V>::keys(ArrayList<K>& all_keys) const {
  //call helper function
  keys(root, all_keys);
}

template<typename K, typename V>
void AVLCollection<K,V>::sort(ArrayList<K>& all_keys_sorted) const {
  //call helper function
  keys(root, all_keys_sorted);
}

template<typename K, typename V>
size_t AVLCollection<K,V>::size() const {
  return node_count;
}

template<typename K, typename V>
size_t AVLCollection<K,V>::height() const {
  if (root == nullptr) {
    return 0;
  }
  return root->height;
}

template<typename K, typename V>
void AVLCollection<K,V>::make_empty(Node* subtree_root) {
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
void AVLCollection<K,V>::copy(Node* lhs_subtree_root, const Node* rhs_subtree_root) {
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
typename AVLCollection<K,V>::Node*
AVLCollection<K,V>::add(Node* subtree_root, const K& a_key, const V& a_val) {
  //add node as leaf
  if (subtree_root == nullptr) {
    Node* new_node = new Node;
    new_node->key = a_key;
    new_node->value = a_val;
    new_node->height = 1;
    new_node->left = nullptr;
    new_node->right = nullptr;
    return new_node;
  //find right place to add
  } else {
    if (a_key < subtree_root->key) {
      subtree_root->left = add(subtree_root->left, a_key, a_val);
    } else {
      subtree_root->right = add(subtree_root->right, a_key, a_val);
    }
  }
  //backtracking: adjust heights
  if (!subtree_root->right && !subtree_root->left) {
    subtree_root->height = 1;
  } else if (subtree_root->right && !subtree_root->left) {
    subtree_root->height = 1 + subtree_root->right->height;
  } else if (subtree_root->left && !subtree_root->right) {
    subtree_root->height = 1 + subtree_root->left->height;
  } else if (subtree_root->right->height > subtree_root->left->height) {
    subtree_root->height = 1 + subtree_root->right->height;
  } else {
    subtree_root->height = 1 + subtree_root->left->height;
  }
  //calls to rebalance
  return rebalance(subtree_root);
}

template<typename K, typename V>
typename AVLCollection<K,V>::Node*
AVLCollection<K,V>::remove(Node* subtree_root, const K& a_key) {
  if (!subtree_root) {
    return subtree_root;
  }
  //find right node to remove
  if (a_key < subtree_root->key) {
    subtree_root->left = remove(subtree_root->left, a_key);
  } else if (a_key > subtree_root->key) {
    subtree_root->right = remove(subtree_root->right, a_key);
  //once found  
  }  else {
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
  //backtracking: adjust heights
  if (!subtree_root->right && !subtree_root->left) {
    subtree_root->height = 1;
  } else if (subtree_root->right && !subtree_root->left) {
    subtree_root->height = 1 + subtree_root->right->height;
  } else if (subtree_root->left && !subtree_root->right) {
    subtree_root->height = 1 + subtree_root->left->height;
  } else if (subtree_root->right->height > subtree_root->left->height) {
    subtree_root->height = 1 + subtree_root->right->height;
  } else {
    subtree_root->height = 1 + subtree_root->left->height;
  }
  //calls to rebalance
  return rebalance(subtree_root);
}

template<typename K, typename V>
void AVLCollection<K,V>::find(const Node* subtree_root, const K& k1, const K& k2,
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
void AVLCollection<K,V>::keys(const Node* subtree_root, ArrayList<K>& all_keys) const {
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
typename AVLCollection<K,V>::Node*
AVLCollection<K,V>::rotate_right(Node* k2) {
  Node* k1 = k2->left;
  k2->left = k1->right;
  k1->right = k2;
  return k1;
}

template<typename K, typename V>
typename AVLCollection<K,V>::Node*
AVLCollection<K,V>::rotate_left(Node* k2) {
  Node* k1 = k2->right;
  k2->right = k1->left;
  k1->left = k2;
  return k1;
}

template<typename K, typename V>
typename AVLCollection<K,V>::Node*
AVLCollection<K,V>::rebalance(Node* subtree_root) {
  //no need to rebalance
  if (subtree_root == nullptr) {
    return subtree_root;
  }
  Node* lptr = subtree_root->left;
  Node* rptr = subtree_root->right;
  if (lptr && !rptr && lptr->height > 1) {
    //check if double rotation is necessary
    if (lptr->right) {
      //rotate left first
      subtree_root->left = rotate_left(lptr);
      //adjust heights
      subtree_root->left->left->height = 1;
      subtree_root->left->height = 2;
    }
    //single right rotation
    subtree_root = rotate_right(subtree_root);
    //adjust heights
    subtree_root->right->height -= 2;
  } else if (rptr && !lptr && rptr->height > 1) {
    //check if double rotation is necessary
    if (rptr->left) {
      //rotate right first
      subtree_root->right = rotate_right(rptr);
      //adjust heights
      subtree_root->right->right->height = 1;
      subtree_root->right->height = 2;
    }
    //single left rotation
    subtree_root = rotate_left(subtree_root);
    //adjust heights
    subtree_root->left->height -= 2;
  } else if (rptr && lptr && lptr->height - rptr->height > 1) {
    //check if double rotation is necessary
    //rotate left first and update heights
    if (lptr->right && lptr->left && lptr->right->height > lptr->left->height) {
      subtree_root->left = rotate_left(lptr);
      subtree_root->left->height++;
    } else if (lptr->right && !lptr->left && lptr->right->height > 1) {
      subtree_root->left = rotate_left(lptr);
      subtree_root->left->height++;
    }
    //single right rotation
    subtree_root = rotate_right(subtree_root);
    //adjust heights
    subtree_root->right->height -= 2;
  } else if (rptr && lptr && rptr->height - lptr->height > 1) {
    //check if double rotation is necessary
    //rotate right first and update heights
    if (rptr->right && rptr->left && rptr->left->height > rptr->right->height) {
      subtree_root->right = rotate_right(rptr);
      subtree_root->right->height++;
    } else if (rptr->left && !rptr->right && rptr->left->height > 1) {
      subtree_root->right = rotate_right(rptr);
      subtree_root->right->height++;
    }
    //singl left rotation
    subtree_root = rotate_left(subtree_root);
    //adjust heights
    subtree_root->left->height -= 2;
  }
  return subtree_root;
}

template<typename K, typename V>
void AVLCollection<K,V>::print_tree(std::string indent, Node* subtree_root)
{
  if (!subtree_root)
    return;
  std::cout << indent << subtree_root->key << " (h=" << subtree_root->height << ")" << std::endl;
  print_tree(indent + " ", subtree_root->left);
  print_tree(indent + " ", subtree_root->right);
}

#endif    
