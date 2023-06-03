#ifndef Dictionary_h
#define Dictionary_h

#include "utilesWet2.h"
#include <memory>
#include <iostream>

using namespace std;
#define BASE_M 47

template<typename T>
class Dictionary {
public:
    StatusType insert(shared_ptr<T> data_ptr);
    shared_ptr<T> del(int id);
    shared_ptr<T> get(int id);    
    
    Dictionary();
    ~Dictionary();

private:
    
};


template<typename T>
Dictionary<T>::Dictionary() {
}


template<typename T>
Dictionary<T>::~Dictionary() {
}       

#endif // !Dictionary_h
