#include <cstdio>
#include <map>
#include <cassert>
using namespace std;
#include <Windows.h>
#include "SkipList.hpp"

static inline long GetTime()
{
    SYSTEMTIME t;
    GetSystemTime(&t);
    return ((t.wHour*60 + t.wMinute)*60 +t.wSecond)*1000 + t.wMilliseconds;
}

void map_test(int elem_size) {
    long tic = GetTime();    
    map<int, int, std::less<int>> m;
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
    SkipList<int, int, std::less<int>> l;
    for (int i=0; i<elem_size ; ++i) {
        int e = rand() % 1000;
        l.insert(std::make_pair(e, e));
    }
    long toc = GetTime();
    printf("skiplist : after insert %d elements, time cost %ldms\n", elem_size, toc-tic);
    tic = toc;
    for (int i=0; i<elem_size; ++i) {
		int e = rand();
		auto it = l.find(e);
		assert (it == NULL || it->value.first == e);
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
//    map_test(elem_size);
	printf("start test\n");
    skip_list_test(elem_size);
    return 0;
}
