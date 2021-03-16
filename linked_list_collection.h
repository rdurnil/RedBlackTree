//----------------------------------------------------------------------
// FILE: linked_list_collection.h
// NAME: Rie Durnil
// DATE: October, 2020
// DESC: Implements a LinkedList version of the collection class, which 
//       applies the concept of key value pairs to common list operations. 
//----------------------------------------------------------------------

#ifndef LINKED_LIST_COLLECTION_H
#define LINKED_LIST_COLLECTION_H

#include "collection.h"
#include "linked_list.h"
#include "array_list.h"

template<typename K, typename V>
class LinkedListCollection : public Collection<K,V> 
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
		LinkedList<std::pair<K,V>> kv_list;
};

template<typename K, typename V>
void LinkedListCollection<K,V>::add(const K& key, const V& val) {
	kv_list.add(std::pair<K,V>(key, val));
}

template<typename K, typename V>
void LinkedListCollection<K,V>::remove(const K& key) {
	std::pair<K,V> curr;
	//search through list to find key to remove
	for (int i = 0; i < kv_list.size(); ++i) {
		kv_list.get(i, curr);
		if (curr.first == key) {
			kv_list.remove(i);
			//return early if found
			return;
		}
	}
}

template<typename K, typename V>
bool LinkedListCollection<K,V>::find(const K& key, V& val) const {
	bool found = false;
	std::pair<K,V> curr;
	//search through list for desired key
	for (int i = 0; i < kv_list.size(); ++i) {
		kv_list.get(i, curr);
		if (curr.first == key) {
			found = true;
			//get key's matching value
			val = curr.second;
			return found;
		}
	}
	return found;
}

template<typename K, typename V>
void LinkedListCollection<K,V>::find(const K& k1, const K& k2, ArrayList<K>& keys) const {
	std::pair<K,V> curr;
	//search through list for keys within range
	for (int i = 0; i < kv_list.size(); ++i) {
		kv_list.get(i, curr);
		if (curr.first >= k1 && curr.first <= k2) {
			//add keys within range to array
			keys.add(curr.first);
		}
	}
}

template<typename K, typename V>
void LinkedListCollection<K,V>::keys(ArrayList<K>& all_keys) const {
	std::pair<K,V> curr;
	//add every key in list to array
	for (size_t i = 0; i < kv_list.size(); ++i) {
		kv_list.get(i, curr);
		all_keys.add(curr.first);
	}
}

template<typename K, typename V>
void LinkedListCollection<K,V>::sort(ArrayList<K>& all_keys_sorted) const {
	//get all keys
	keys(all_keys_sorted);
	//sort resulting array
	all_keys_sorted.sort();
}

template<typename K, typename V>
size_t LinkedListCollection<K,V>::size() const {
	return kv_list.size();
}

#endif
