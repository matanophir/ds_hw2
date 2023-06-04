#ifndef Dictionary_h
#define Dictionary_h

#include <memory>
#include <iostream>
#include "utilesWet2.h"
#include "AvlTree.h"

using namespace std;
#define BASE_M 47

template <typename T>
class Insert_elem_func;

template<typename T>
class Dictionary {
public:
    StatusType insert(shared_ptr<T> data_ptr);
    shared_ptr<T> del(int id);
    shared_ptr<T> get(int id);    
    
    Dictionary(): table(new AvlTree<T, Def_e<T>, Def_lt<T>>[BASE_M]), table_size(BASE_M), num_of_elems(0){};
    ~Dictionary() {delete[] table;};

private:
    AvlTree<T, Def_e<T>, Def_lt<T>>* table;  // with default func
    int table_size;
    int num_of_elems;
    int hash(int id);
    StatusType _add(shared_ptr<T> data_ptr);
    void _check_table_size();
    void _copy_to_new_table(int new_size); //TODO: need to change inOrderWfunc to catch alloctaion error?

    template<typename U>
    friend class Insert_elem_func;

};


template<typename T>
StatusType Dictionary<T>::insert(shared_ptr<T> data_ptr)
{
    StatusType status = _add(data_ptr);
    if (status!=StatusType::SUCCESS)
        return status;

    _check_table_size();    
    return status;
}



template<typename T>
shared_ptr<T> Dictionary<T>::del(int id)
{
    int index = hash(id);
    auto data_ptr = table[index].remove_node(T(id));
    if (data_ptr == nullptr)
        return nullptr;

    --num_of_elems;
    _check_table_size();
    return data_ptr;

}


template<typename T>
shared_ptr<T> Dictionary<T>::get(int id)
{
    int index = hash(id);
    auto data_ptr = table[index].get(T(id));
    return data_ptr;
}

template<typename T>
int Dictionary<T>::hash(int id)
{
    return id % table_size;
}


template<typename T>
void Dictionary<T>::_check_table_size()
{
    if (num_of_elems < BASE_M && table_size == BASE_M)
        return;

    int new_size = table_size;
    if(num_of_elems == table_size)
    {
        new_size = table_size*2;
        _copy_to_new_table(new_size);
    }else if (num_of_elems == table_size/4) {
        new_size = table_size/2;
        if (new_size < BASE_M)
            new_size = BASE_M;
        _copy_to_new_table(new_size);
    }
    table_size = new_size;
}

template<typename T>
void Dictionary<T>::_copy_to_new_table(int new_size)
{
    auto new_table = new AvlTree<T, Def_e<T>, Def_lt<T>>[new_size];
    auto old_table = table;
    int old_size = table_size;

    table = new_table;
    table_size = new_size;
    num_of_elems = 0;

    Insert_elem_func<T> func(this);
    for (int i = 0; i<old_size; ++i) {
        old_table[i].in_order_Wfunc(func);
    }
    delete[] old_table; 
}

template <typename T>
class Insert_elem_func
{
    public:
    Insert_elem_func(Dictionary<T>* dict_ptr): dict_ptr(dict_ptr){};
    ~Insert_elem_func() = default;
    StatusType operator()(shared_ptr<T> data_ptr)
    {
        return dict_ptr->_add(data_ptr);
    }

    private:
    Dictionary<T>* dict_ptr;
};

template<typename T>
StatusType Dictionary<T>::_add(shared_ptr<T> data_ptr)
{
    int index = hash(data_ptr->id);
    StatusType status = table[index].add_node(data_ptr);
    if (status!=StatusType::SUCCESS)
        return status;

    ++num_of_elems;
    return status;
}
#endif // !Dictionary_h
