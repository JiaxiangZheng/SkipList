#include <ctime>    //for rand
#include <cstdlib>  //for rand

////////////////////////////////////////////////////////////////////////////////
static int randomLevel() {  //return 1~MAX_LEVEL
    int level = 1;
    while ((rand()&1) == 1)    // p = 0.25
        level += 1;
    if (level > SKIPLIST_MAX_LEVEL)
        level = SKIPLIST_MAX_LEVEL;
    return level;
}
////////////////////////////////////////////////////////////////////////////////
template <typename KeyType, typename DataType>
struct SkipNode {
	std::pair<KeyType, DataType> value;
    SkipNode<KeyType, DataType>** forward;  //stores the pointers to next elements of different levels
};

//create skipnode whose forward field contains level'th element
template <typename KeyType, typename DataType>
SkipNode<KeyType, DataType>* createNode(const std::pair<KeyType, DataType>& value, int level) {       
    SkipNode<KeyType, DataType>* node = new SkipNode<KeyType, DataType>;
	node->value = value;
    node->forward = new SkipNode<KeyType, DataType>*[level];       
    for (int i=0; i<level; ++i) {
        node->forward[i] = NULL;
    }
    return node;
}
template <typename KeyType, typename DataType>
void destoryNode(SkipNode<KeyType, DataType>* node) {
    if (node == NULL) return;
    delete[] node->forward;
    delete node;
}
////////////////////////////////////////////////////////////////////////////////
template <typename KeyType, typename DataType, class CompareFunc>
SkipList<KeyType, DataType, CompareFunc>::SkipList() {
    this->level = 0;        //denotes an empty list 
    this->head = createNode(std::make_pair(KeyType(), DataType()), SKIPLIST_MAX_LEVEL);  //do not contain any element
}

template <typename KeyType, typename DataType, class CompareFunc>
SkipList<KeyType, DataType, CompareFunc>::~SkipList() {
    SkipNode<KeyType, DataType> *node = this->head, *next = node->forward[0];
    int i = 0;
    do {
        next = node->forward[0];
        destoryNode(node);
        node = next;
    } while (node != NULL);
    return;
}

template <typename KeyType, typename DataType, class CompareFunc>
bool SkipList<KeyType, DataType, CompareFunc>::insert(const std::pair<KeyType, DataType>& value) {
    SkipNode<KeyType, DataType> *update[SKIPLIST_MAX_LEVEL], *node = this->head;
	CompareFunc cmp;
    
    //search for the insert position
    for (int i=this->level - 1; i >= 0; i--) {
        while (node->forward[i] != NULL && cmp(node->forward[i]->value.first, value.first)) {
            node = node->forward[i];
        }
        update[i] = node; //point to the left most element whose key field is less than key or the head.
    }
    node = node->forward[0]; // if it points to NULL, denotes this is the maximum key to be inserted

    if (node != NULL && !cmp(node->value.first, value.first) && !cmp(value.first, node->value.first)) return false;

    int newLevel = randomLevel();
    if (this->level < newLevel) {
        for (int i=this->level; i < newLevel; ++i) update[i] = this->head;
        this->level = newLevel;
    }
    node = createNode(value, newLevel);
    for (int i=0; i<newLevel; ++i) {
        node->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = node;
    }

    return true;
}

template <typename KeyType, typename DataType, class CompareFunc>
SkipNode<KeyType, DataType>* SkipList<KeyType, DataType, CompareFunc>::find(KeyType key) {
	CompareFunc cmp;
	SkipNode<KeyType, DataType> *node = this->head;
    //search for the insert position
    for (int i=this->level - 1; i >= 0; i--) {
        while (node->forward[i] != NULL && cmp(node->forward[i]->value.first, key)) {
            node = node->forward[i];
        }
    }
    node = node->forward[0];
	if (node != NULL && 
            (cmp(node->value.first, key) 
                || cmp(key, node->value.first))
        ) return NULL;
	else return node;
}

// returns the node whose key field equal to key
// this function do not free the space the node occupied, so you should delete
// it outside by calling destoryNode(node)
template <typename KeyType, typename DataType, class CompareFunc>
SkipNode<KeyType, DataType>* SkipList<KeyType, DataType, CompareFunc>::remove(KeyType key) {
    SkipNode<KeyType, DataType> *update[SKIPLIST_MAX_LEVEL], *node = this->head;
	CompareFunc cmp;
    //search for the insert position
    for (int i=this->level - 1; i >= 0; i--) {
        while (node->forward[i] != NULL && cmp(node->forward[i]->value.first, key)) {
            node = node->forward[i];
        }
        update[i] = node; //point to the left most element whose key field is less than key or the head.
    }
    node = node->forward[0];

    if (node == NULL || 
            cmp(nodenode->value.first, key) || 
            cmp(key, node->value.first))
        return NULL;
    for (int i=0; i<this->level; ++i) {
        if (update[i]->forward[i] == node) {
            update[i]->forward[i] = node->forward[i];
        }
    }
    while (this->level > 0 && this->head->forward[this->level-1] == NULL) {
        this->level--;
    }

    return node;
}
