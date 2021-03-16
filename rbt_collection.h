//----------------------------------------------------------------------
// Name: Rie Durnil
// File: rbt_collection.h
// Date: Fall, 2020
// Desc: A Key-Value Collection implementation using a Red-Black tree.
//----------------------------------------------------------------------


#ifndef RBT_COLLECTION_H
#define RBT_COLLECTION_H


#include "string.h"
#include "collection.h"
#include "array_list.h"


template<typename K, typename V>
class RBTCollection : public Collection<K,V>
{
public:

  // create an empty collection
  RBTCollection();
  // copy constructor
  RBTCollection(const RBTCollection<K,V>& rhs);
  // assignment operator
  RBTCollection<K,V>& operator=(const RBTCollection<K,V>& rhs);
  // delete collection
  ~RBTCollection();
  // add a new key-value pair into the collection 
  void add(const K& a_key, const V& a_val);
  // remove a key-value pair from the collectiona
  void remove(const K& a_key);
  // find and return the value associated with the key
  bool find(const K& search_key, V& the_val) const;
  // find and return each key >= k1 and <= k2 
  void find(const K& k1, const K& k2, ArrayList<K>& keys) const;
  // return all of the keys in the collection 
  void keys(ArrayList<K>& all_keys) const;
  // return all of the keys in ascending (sorted) order
  void sort(ArrayList<K>& all_keys_sorted) const;
  // return the number of key-value pairs in the collection
  size_t size() const;
  // return the height of the tree
  size_t height() const;

  // for testing:
  // check if tree satisfies the red-black tree constraints
  bool valid_rbt() const; 
  // pretty-print the red-black tree (with heights)
  void print() const;
  
private:
  
  // RBT node structure
  enum color_t {RED, BLACK};
  struct Node {
    K key;
    V value;
    Node* left;
    Node* right;
    Node* parent;
    color_t color;
  };
  // root node
  Node* root;
  // number of k-v pairs stored in the collection
  size_t node_count;
  // helper to empty entire hash table
  void make_empty(Node* subtree_root);
  // copy helper
  void copy(Node* lhs_subtree_root, const Node* rhs_subtree_root); 
  // helper to recursively find range of keys
  void find(const Node* subtree_root, const K& k1, const K& k2, ArrayList<K>& keys) const;
  // helper to build sorted list of keys (used by keys and sort)
  void keys(const Node* subtree_root, ArrayList<K>& all_keys) const;
  // rotate right helper
  void rotate_right(Node* k2);
  // rotate left helper
  void rotate_left(Node* k2);
  // restore red-black constraints in add
  void add_rebalance(Node* x);
  // restore red-black constraints in remove
  void remove_rebalance(Node* x, bool going_right);
  // height helper
  size_t height(Node* subtree_root) const;
  
  // ------------
  // for testing:
  // ------------
  
  // validate helper
  bool valid_rbt(Node* subtree_root) const;

  // validate helper
  size_t black_node_height(Node* subtree_root) const;
  
  // recursive pretty-print helper
  void print_tree(std::string indent, Node* subtree_root) const;
};


// TODO: Finish the above functions below

template<typename K, typename V>
RBTCollection<K,V>::RBTCollection() {
  root = nullptr;
  node_count = 0;
}

template<typename K, typename V>
RBTCollection<K,V>::RBTCollection(const RBTCollection<K,V>& rhs){
  // defer to assignment operator
  root = nullptr;
  *this = rhs;
}

template<typename K, typename V>
RBTCollection<K,V>::~RBTCollection() {
  make_empty(root);
  root = nullptr;
}

template<typename K, typename V>
RBTCollection<K,V>& RBTCollection<K,V>::operator=(const RBTCollection<K,V>& rhs) {
  if (this != &rhs) {
    make_empty(root);
    root = nullptr;
    copy(root, rhs.root);
    node_count = rhs.node_count;
  }
  return *this;
}
 
template<typename K, typename V>
void RBTCollection<K,V>::add(const K& k, const V& val) {
  //create new node
  Node* n = new Node;
  n->key = k;
  n->value = val;
  n->left = nullptr;
  n->right = nullptr;
  n->color = RED;
  //create Node* for iteration
  Node* x = root;
  Node* p = nullptr;
  //iterate through tree
  while (x != nullptr) {
    add_rebalance(x);
    p = x;
    if (k < x->key) {
      x = x->left;
    } else {
      x = x->right;
    }
  }
  //adding cases
  if (p == nullptr) {
    root = n;
    n->parent = nullptr;
  } else if (k < p->key) {
    p->left = n;
    n->parent = p;
  } else {
    p->right = n;
    n->parent = p;
  }
  add_rebalance(n);
  root->color = BLACK;
  node_count++;
}

template<typename K, typename V>
void RBTCollection<K,V>::remove(const K& k) {
  //node to remove doesn't exist
  if (node_count == 0) {
    return;
  }
  //create sentinel and initialize interative nodes
  Node* sentinel = new Node;
  sentinel->right = root;
  root->parent = sentinel;
  sentinel->color = RED;
  Node* x = root;
  Node* p = x->parent;
  bool found = false;
  //iteratively find node to delete
  while (x != nullptr && !found) {
    if (k < x->key) {
      remove_rebalance(x, false);
      p = x;
      x = x->left;
    } else if (x->key < k) {
      remove_rebalance(x, true);
      p = x;
      x = x->right;
    } else {
      remove_rebalance(x, x->left);
      found = true;
      p = x->parent;
    }
  }
  //node to delete not found, exit
  if (!found) {
    return;
  }
  //remove cases
  if (!x->left || !x->right) {
    //0 or 1 children
    if (x->right) {
      x->right->parent = x->parent;
      if (x->parent->left == x) {
        x->parent->left = x->right;
      } else {
        x->parent->right = x->right;
      }
      delete x;
    } else if (x->left) {
      x->left->parent = x->parent;
      if (x->parent->left == x) {
        x->parent->left = x->right;
      } else {
        x->parent->right = x->right;
      }
      delete x;
    } else {
      if (x->parent->left == x) {
        x->parent->left = x->right;
      } else {
        x->parent->right = x->right;
      }
      delete x;
    }
  } else {
    //2 children
    Node* s = x->right;
    remove_rebalance(s, false);
    while (s->left) {
      s = s->left;
      remove_rebalance(s, false);
    }
    //copy s key-value into x
    x->key = s->key;
    x->value = s->value;
    //remove s cases
    if (s == s->parent->left) {
      s->parent->left = s->right;
    } else {
      s->parent->right = s->right;
    }
    if (s->right) {
      s->right->parent = s->parent;
    }
    delete s;
  }
  //clean up
  root = sentinel->right;
  if (root) {
    root->color = BLACK;
    root->parent = nullptr;
  }
  delete sentinel;
  node_count--;
}

template<typename K, typename V>
bool RBTCollection<K,V>::find(const K& key, V& val) const {
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
void RBTCollection<K,V>::find(const K& k1, const K& k2, ArrayList<K>& keys) const {
  //call helper function
  find(root, k1, k2, keys);
}

template<typename K, typename V>
void RBTCollection<K,V>::keys(ArrayList<K>& all_keys) const {
  //call helper function
  keys(root, all_keys);
}

template<typename K, typename V>
void RBTCollection<K,V>::sort(ArrayList<K>& all_keys_sorted) const {
  //call helper function
  keys(root, all_keys_sorted);
}

template<typename K, typename V>
size_t RBTCollection<K,V>::size() const {
  return node_count;
}

template<typename K, typename V>
size_t RBTCollection<K,V>::height() const {
  //print();
  return height(root);
}

template<typename K, typename V>
void RBTCollection<K,V>::make_empty(Node* subtree_root) {
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
void RBTCollection<K,V>::copy(Node* lhs_subtree_root, const Node* rhs_subtree_root) {
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
void RBTCollection<K,V>::find(const Node* subtree_root, const K& k1, const K& k2,
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
void RBTCollection<K,V>::keys(const Node* subtree_root, ArrayList<K>& all_keys) const {
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
void RBTCollection<K,V>::rotate_right(Node* k2) {
  Node* k1 = k2->left;
  k2->left = k1->right;
  if (k2->left) {
    k2->left->parent = k2;
  }
  k1->parent = k2->parent;
  if (k1->parent && k2 == k1->parent->right) {
    k1->parent->right = k1;
  } else if (k1->parent && k2 == k1->parent->left) {
    k1->parent->left = k1;
  } else {
    root = k1;
    k1->parent = nullptr;
  }
  k1->right = k2;
  k2->parent = k1;
}

template<typename K, typename V>
void RBTCollection<K,V>::rotate_left(Node* k2) {
  Node* k1 = k2->right;
  k2->right = k1->left;
  if (k2->right) {
    k2->right->parent = k2;
  }
  k1->parent = k2->parent;
  if (k1->parent && k2 == k1->parent->right) {
    k1->parent->right = k1;
  } else if (k1->parent && k2 == k1->parent->left) {
    k1->parent->left = k1;
  } else {
    root = k1;
    k1->parent = nullptr;
  }
  k1->left = k2;
  k2->parent = k1;
}

template<typename K, typename V>
void RBTCollection<K,V>::add_rebalance(Node* x) {
  Node* p = x->parent;
  //case 1: color flip
  if (x->color == BLACK && x->right && x->left && x->right->color == RED && x->left->color == RED) {
    x->color = RED;
    x->left->color = BLACK;
    x->right->color = BLACK;
  }
  //case 2 & 3: rotations
  if (x->color == RED && p && p->color == RED) {
    if (p->parent->right == p) {
      //handle inside (right-left) case
      if (p->left == x) {
        rotate_right(p);
        x = p;
        p = p->parent;
      }
      //handle outside right-right case
      rotate_left(p->parent);
      p->color = BLACK;
      p->left->color = RED;
    } else {
      //handle inside (left-right) case
      if (p->parent->left == p) {
        if (p->right == x) {
          rotate_left(p);
          x = p;
          p = p->parent;
        }
        //handle outside (left-left) case
        rotate_right(p->parent);
        p->color = BLACK;
        p->right->color = RED;
      }
    }
  }
}

template<typename K, typename V>
void RBTCollection<K,V>::remove_rebalance(Node* x, bool going_right) {
  //x being red is the goal
  if (x->color == RED) {
    return;
  }
  //initialize to iterate
  Node* p = x->parent;
  Node* t = nullptr;
  if (p->right && p->left) {
    if (p->right == x) {
      t = p->left;
    } else {
      t = p->right;
    }
  }
  //case 1: x has non-navigation red child
  if (x->right && x->left && (x->right->color == RED || x->left->color == RED)) {
    if (going_right && x->left->color == RED) {
      rotate_right(x);
      x->parent->color = BLACK;
      x->color = RED;
    } else if (!going_right && x->right->color == RED) {
      rotate_left(x);
      x->parent->color = BLACK;
      x->color = RED;
    }
  //case 2: color flip
  } else if (t && (!t->left || t->left->color == BLACK) && (!t->right || t->right->color ==BLACK)) {
    p->color = BLACK;
    x->color = RED;
    t->color = RED;  
  //cases 3 & 4: outside and inside red sibling children
  } else if (t && ((t->right && t->right->color==RED) || (t->left && t->left->color==RED))) {
    //right-left case
    if (t == p->right && t->left && t->left->color == RED) {
      rotate_right(t);
      rotate_left(p);
    //left-right case
    } else if (t == p->left && t->right && t->right->color == RED) {
      rotate_left(t);
      rotate_right(p);
    //right-right case
    } else if (t == p->right) {
      t->color = RED;
      if (t->right) {
        t->right->color = BLACK;
      }
      rotate_left(p);
    //left-left case
    } else {
      t->color = RED;
      if (t->left) {
        t->left->color = BLACK;
      }
      rotate_right(p);
    }
    p->color = BLACK;
    x->color = RED;
  }
}
  
template<typename K, typename V>
size_t RBTCollection<K,V>::height(Node* subtree_root) const {
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


//----------------------------------------------------------------------
// Provided Helper Functions:
//----------------------------------------------------------------------

template<typename K, typename V>
bool RBTCollection<K,V>::valid_rbt() const
{
  return !root or (root->color == BLACK and valid_rbt(root));
}


template<typename K, typename V>
bool RBTCollection<K,V>::valid_rbt(Node* subtree_root) const
{
  if (!subtree_root)
    return true;
  color_t rc = subtree_root->color;
  color_t lcc = subtree_root->left ? subtree_root->left->color : BLACK;
  color_t rcc = subtree_root->right ? subtree_root->right->color : BLACK;  
  size_t lbh = black_node_height(subtree_root->left);
  size_t rbh = black_node_height(subtree_root->right);
  bool lv = valid_rbt(subtree_root->left);
  bool rv = valid_rbt(subtree_root->right);
  // check equal black node heights, no two consecutive red nodes, and
  // left and right are valid RBTs
  return (lbh == rbh) and (rc != RED or (lcc != RED and rcc != RED)) and lv and rv;
}


template<typename K, typename V>
size_t RBTCollection<K,V>::black_node_height(Node* subtree_root) const
{
  if (!subtree_root)
    return 1;
  size_t hl = black_node_height(subtree_root->left);
  size_t hr = black_node_height(subtree_root->right);
  size_t h = hl > hr ? hl : hr;
  if (subtree_root->color == BLACK)
    return 1 + h;
  else
    return h;
}


template<typename K, typename V>
void RBTCollection<K,V>::print() const
{
  print_tree("", root);
}


template<typename K, typename V>
void RBTCollection<K,V>::print_tree(std::string indent, Node* subtree_root) const
{
  if (!subtree_root)
    return;
  std::string color = "[BLACK]";
  if (subtree_root->color == RED)
    color = "[RED]";
  std::cout << indent << subtree_root->key << " "
	    << color << " (h="
	    << height(subtree_root) << ")" << std::endl;
  print_tree(indent + "  ", subtree_root->left);
  print_tree(indent + "  ", subtree_root->right);
}


#endif
