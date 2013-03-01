#include "SkipList.hpp"
#include <ctime>    //for rand
#include <cstdlib>  //for rand

#include <cstdio>
////////////////////////////////////////////////////////////////////////////////
static int randomLevel() {  //return 1~MAX_LEVEL
    int level = 1;
    while ((rand()&1) == 1)    // p = 0.25
        level += 1;
    if (level > SkipList::MAX_LEVEL)
        level = SkipList::MAX_LEVEL;
    return level;
}
////////////////////////////////////////////////////////////////////////////////
struct SkipNode {
    DataType data;
    KeyType  key;   
    SkipNode** forward;  //stores the pointers to next elements of different levels
};

//create skipnode whose forward field contains level'th element
SkipNode* createNode(DataType data, KeyType key, int level) {       
    SkipNode* node = new SkipNode;
    node->data = data;
    node->key = key;
    node->forward = new SkipNode*[level];       
    for (int i=0; i<level; ++i) {
        node->forward[i] = NULL;
    }
    return node;
}
void destoryNode(SkipNode* node) {
    if (node == NULL) return;
    delete[] node->forward;
    delete node;
}
////////////////////////////////////////////////////////////////////////////////
const int SkipList::MAX_LEVEL = 16;

SkipList::SkipList() {
    this->level = 0;        //denotes an empty list 
    this->head = createNode(DataType(0), KeyType(0), SkipList::MAX_LEVEL);  //do not contain any element
}

SkipList::~SkipList() {
    SkipNode *node = this->head, *next = node->forward[0];
    int i = 0;
    do {
        next = node->forward[0];
        destoryNode(node);
        node = next;
    } while (node != NULL);
    return;
}

bool SkipList::insert(DataType data, KeyType key) {
    SkipNode *update[SkipList::MAX_LEVEL], *node = this->head;
    
    //search for the insert position
    for (int i=this->level - 1; i >= 0; i--) {
        while (node->forward[i] != NULL && node->forward[i]->key < key) {
            node = node->forward[i];
        }
        update[i] = node; //point to the left most element whose key field is less than key or the head.
    }
    node = node->forward[0]; // if it points to NULL, denotes this is the maximum key to be inserted

    if (node != NULL && node->key == key) return false; //TODO: maybe we could replace the orginal node with new data

    int newLevel = randomLevel();
    if (this->level < newLevel) {
        for (int i=this->level; i < newLevel; ++i) update[i] = this->head;
        this->level = newLevel;
    }
    node = createNode(data, key, newLevel);
    for (int i=0; i<newLevel; ++i) {
        node->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = node;
    }

    return true;
}

SkipNode* SkipList::find(KeyType key) {
	SkipNode *node = this->head;
    //search for the insert position
    for (int i=this->level - 1; i >= 0; i--) {
        while (node->forward[i] != NULL && node->forward[i]->key < key) {
            node = node->forward[i];
        }
    }
    node = node->forward[0];
	if (node != NULL && node->key != key) return NULL;
	else return node;
}

// returns the node whose key field equal to key
// this function do not free the space the node occupied, so you should delete
// it outside by calling destoryNode(node)
SkipNode* SkipList::remove(KeyType key) {
    SkipNode *update[SkipList::MAX_LEVEL], *node = this->head;
    //search for the insert position
    for (int i=this->level - 1; i >= 0; i--) {
        while (node->forward[i] != NULL && node->forward[i]->key < key) {
            node = node->forward[i];
        }
        update[i] = node; //point to the left most element whose key field is less than key or the head.
    }
    node = node->forward[0];

    if (node == NULL || node->key != key) return NULL;
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

template <class FuncType>
void SkipList::for_each(FuncType functor) {
    SkipNode* node = this->head->forward[0];
    while (node != NULL) {
        functor(node->data);
        node = node->forward[0];
    }
    return;
}
//#define SKIP_LIST_TEST

#ifdef SKIP_LIST_TEST
#include <map>
#include <cassert>
using namespace std;
#include <Windows.h>
static inline long GetTime()
{
    SYSTEMTIME t;
    GetSystemTime(&t);
    return ((t.wHour*60 + t.wMinute)*60 +t.wSecond)*1000 + t.wMilliseconds;
}

void map_test(int elem_size) {
    long tic = GetTime();    
    map<int, int> m;
    for (int i=0; i<elem_size ; ++i) {
        int e = rand();
        m[e] = e;
    }
    long toc = GetTime();
    printf("map : after insert %d elements, time cost %ldms\n", elem_size, toc-tic);
    tic = toc;
	for (int i=0; i<elem_size; ++i) {
		int e = rand();
		auto it = m.find(e);
		assert (it == m.end() || it->second == e);
	}
	
    toc = GetTime();
    printf("map : after finding %d elements, time cost %ldms\n", elem_size, toc-tic);
}

void skip_list_test(int elem_size) {
    long tic = GetTime();    
    SkipList l;
    for (int i=0; i<elem_size ; ++i) {
        int e = rand() % 1000;
        l.insert(e, e);
    }
    long toc = GetTime();
    printf("skiplist : after insert %d elements, time cost %ldms\n", elem_size, toc-tic);
    tic = toc;
    for (int i=0; i<elem_size; ++i) {
		int e = rand();
		auto it = l.find(e);
		assert (it == NULL || it->key == e);
	}
	
    toc = GetTime();
    printf("skiplist : after finding %d elements, time cost %ldms\n", elem_size, toc-tic);
    //following code test the remove function
/*    printf("\n\n");
    for (int i=0; i<elem_size ; ++i) {
        int e = rand() % 1000;
        SkipNode* node = l.remove(e);
        assert (!node || e == node->key);
        destoryNode(node);
    }
    cnt = 0;
    l.for_each([&](DataType val) {printf("%d\t%d\n", cnt++, val);});
*/
}

int main() {
    int elem_size = 2000000;
    map_test(elem_size);
    skip_list_test(elem_size);
    return 0;
}
#endif
