//
//TODO: extend the skiplist to template class for further usage instead of map
#ifndef SKIP_LIST_HPP
#define SKIP_LIST_HPP

typedef int DataType;
typedef int KeyType;

template <typename KeyType, typename DataType>
struct SkipNode;

const int SKIPLIST_MAX_LEVEL = 32;
template <typename KeyType, typename DataType, class CompareFunc>
class SkipList {
public:
    SkipList();
    ~SkipList();
	int Level() const {return level;}
    bool insert(const std::pair<KeyType, DataType>& value);
	SkipNode<KeyType, DataType>* find(KeyType key);
    SkipNode<KeyType, DataType>* remove(KeyType key);

    template <class FuncType>
    void for_each(FuncType functor) {
        SkipNode<KeyType, DataType>* node = this->head->forward[0];
        while (node != NULL) {
            functor(node->data);
            node = node->forward[0];
        }
        return;
    }
private:
    SkipNode<KeyType, DataType>* head;
    int       level;
};

#include "SkipList.inl"

#endif/* SKIP_LIST_HPP */


