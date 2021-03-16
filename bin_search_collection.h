//----------------------------------------------------------------------
// FILE: bin_search_collection.h
// NAME: Rie Durnil
// DATE: October, 2020
// DESC: Implements a version of the collection class, in which an ArrayList
//        is used toapplies the concept of key value pairs to common list 
//        operations, but additionally, binary search is used to make it
//        more efficient.
//----------------------------------------------------------------------

#ifndef BIN_SEARCH_COLLECTION_H
#define BIN_SEARCH_COLLECTION_H

#include "collection.h"
#include "array_list.h"

using namespace std;

template<typename K, typename V>
class BinSearchCollection : public Collection<K,V> 
{
  public:
    void add(const K& key, const V& val);
    void remove(const K& key);
    bool find(const K& key, V& val) const;
    void find(const K& k1, const K& k2, ArrayList<K>& keys) const;
    void keys(ArrayList<K>& all_keys) const;
    void sort(ArrayList<K>& all_keys_sorted) const;
    size_t size() const;
		
  private:
    ArrayList<std::pair<K,V>> kv_list;
    bool bin_search(const K& key, size_t& index) const;	
};

template<typename K, typename V>
void BinSearchCollection<K,V>::add(const K& key, const V& val) {
  //if the list is empty, add to front
  if (kv_list.size() == 0) {
    kv_list.add(std::pair<K,V>(key,val));
  return;
  }
  size_t pos = 0;
  std::pair<K,V> p;
  //won't add if key is already in list
  if (!bin_search(key, pos)) {
    kv_list.get(pos, p);
    //if index returned is the place before new key should be added
    if (p.first < key) {
      kv_list.add(pos + 1, std::pair<K,V>(key, val));
    //if index returned is the correct place for the new key
    } else {
      kv_list.add(pos, std::pair<K,V>(key, val));
    }
  }
}

template<typename K, typename V>
void BinSearchCollection<K,V>::remove(const K& key) {
  size_t pos;
  //only removes if found in bin search
  if (bin_search(key, pos)) {
    kv_list.remove(pos);
  }
}

template<typename K, typename V>
bool BinSearchCollection<K,V>::find(const K& key, V& val) const {
  size_t index;
  std::pair<K,V> p;
  //makes sure bin search returned true
  if (bin_search(key, index)) {
    kv_list.get(index, p);
    val = p.second;
    return true;
  }
  return false;
}

template<typename K, typename V>
void BinSearchCollection<K,V>::find(const K& k1, const K& k2, ArrayList<K>& keys) const {
  size_t pos;
  std::pair<K,V> p;
  //find lower constraint using bin search
  bin_search(k1, pos);
  kv_list.get(pos, p);
  // in case index returned was the one before k1
  if (p.first < k1) {
    pos = pos + 1;
    kv_list.get(pos, p);
  }
  //adding keys between k1 and k2
  while (p.first >= k1 && p.first <= k2) {
    keys.add(p.first);
    pos = pos + 1;
    //make sure pos is valid
    if (!kv_list.get(pos, p)) {
      return;
    }
  }
}

template<typename K, typename V>
void BinSearchCollection<K,V>::keys(ArrayList<K>& all_keys) const {
  std::pair<K,V> curr;
  //add every key in list to array
  for (size_t i = 0; i < kv_list.size(); ++i) {
    kv_list.get(i, curr);
    all_keys.add(curr.first);
  }
}

template<typename K, typename V>
void BinSearchCollection<K,V>::sort(ArrayList<K>& all_keys_sorted) const {
  //get all keys, list already sorted
  keys(all_keys_sorted);
}

template<typename K, typename V>
size_t BinSearchCollection<K,V>::size() const {
  return kv_list.size();
}

template<typename K, typename V>
bool BinSearchCollection<K,V>::bin_search(const K& key, size_t& index) const {
  bool found = false;
  //if list is empty, no need to search
  if (kv_list.size() == 0) {
    return found;
  }
  size_t low = 0;
  size_t high = kv_list.size() - 1;
  size_t mid_index;
  std::pair<K,V> mid;
  //while there are still indices to search in the right range and 
  //key hasn't been found.
  while (high >= low && !found) {
    //set mid index
    mid_index = (high + low) / 2;
    kv_list.get(mid_index, mid);
    //if the key should be searched for in the left half
    if (key < mid.first) {
      //ensuring high isn't going to be invalid
      if (high != 0) {
        high = mid_index - 1;
      } else {
        index = mid_index;
        return found;
      }
    //if the key should be searched for in the right half
    } else if (key > mid.first) {
      low = mid_index + 1;
    //if the key was found
    } else if (key == mid.first) {
      found = true;
    }
  }
  index = mid_index;
  return found;
}

#endif
