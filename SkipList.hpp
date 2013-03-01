//
//TODO: extend the skiplist to template class for further usage instead of map
#ifndef SKIP_LIST_HPP
#define SKIP_LIST_HPP

typedef int DataType;
typedef int KeyType;
struct SkipNode;

class SkipList {
public:
    static const int MAX_LEVEL;
    SkipList();
    ~SkipList();
	int Level() const {return level;}
    bool insert(DataType data, KeyType key);
	SkipNode* find(KeyType key);
    SkipNode* remove(KeyType key);

    template <class FuncType>
    void for_each(FuncType functor);
private:
    SkipNode* head;
    int       level;
};

#endif/* SKIP_LIST_HPP */
