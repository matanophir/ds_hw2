#ifndef AvlTree_h
#define AvlTree_h

#include "utilesWet2.h"
#include <cstddef>
#include <memory>
#include <iostream>

using namespace std;

// should I save parent node or do recursion? chose parent node...🤦‍♂️🤦‍♂️🤦‍♂️

// reasonable that the data would be a shared ptr so that all the trees would have the same copy..

// can change add_node(shared_ptr<T> -> to just T if dont want to enforce data shared ptr..

template <typename T, typename C_e, typename C_lt>
class AvlTree
{
public:
    int size;
    shared_ptr<T> smallest_elem; // updates every add and remove

    StatusType add_node(shared_ptr<T> data_ptr);

    template <typename U>
    shared_ptr<T> remove_node(U&& dataT);  // can be a temporary object only for the sake of compare. (using universal referencing - acts both as rvalue and lvalue ref)
                                        
    void in_order_insertArr(int* ids);

    template <typename U>
    shared_ptr<T> get(U&& dataT);

    template<typename Func>
    void in_order_Wfunc(Func func);

    AvlTree() : size(0), cond_e(C_e()),cond_lt(C_lt()){};
    ~AvlTree() = default; // should be taken care of with smart ptrs..

private:
    class Node;
    shared_ptr<Node> root;
    void in_order(shared_ptr<Node> node); // for testing
    C_e cond_e;
    C_lt cond_lt;

    shared_ptr<Node> _create_node(shared_ptr<T> data_ptr);
    shared_ptr<Node> _delete_node(shared_ptr<Node> to_del);
    void _swap_nodes(shared_ptr<Node> to_del, shared_ptr<Node> to_swap);
    void _update_parent_child(shared_ptr<Node> parent,
                              shared_ptr<Node> new_child,
                              shared_ptr<Node> old_child);
    void _update_smallest_elem();

    void balance(shared_ptr<Node> curr);
    shared_ptr<Node>
    _choose_roll(shared_ptr<Node> curr);
    shared_ptr<Node> rr_roll(shared_ptr<Node> curr);
    shared_ptr<Node> ll_roll(shared_ptr<Node> curr);
    shared_ptr<Node> rl_roll(shared_ptr<Node> curr);
    shared_ptr<Node> lr_roll(shared_ptr<Node> curr);
};

template <typename T, typename C_e, typename C_lt>
class AvlTree<T,C_e,C_lt>::Node
{
public:
    Node(shared_ptr<T> data_ptr,AvlTree<T,C_e,C_lt>* tree ): data_ptr(data_ptr), tree(tree),height(0){};
    ~Node() = default;

    shared_ptr<T> data_ptr;
    int get_height(shared_ptr<Node> node);
    void update_height();
    int get_bs_factor();
    friend class AvlTree<T,C_e,C_lt>;

    bool operator==(T &dataT); 
    bool operator<(T &dataT);

private:
    shared_ptr<Node> left;
    shared_ptr<Node> right;
    weak_ptr<Node> parent; // avoid cyclic reference - dem trickss
    AvlTree<T,C_e,C_lt>* tree;

    int height;
};

// node implementation

template <typename T, typename C_e, typename C_lt>
int AvlTree<T,C_e,C_lt>::Node::get_height(shared_ptr<Node> node) // add const  & maybe TODO
{
    if (node == nullptr)
    {
        return -1;
    }
    return node->height;
}

template <typename T, typename C_e, typename C_lt>
void AvlTree<T,C_e,C_lt>::Node::update_height()
{
    height = 1 + std::max(get_height(left), get_height(right));
}

template <typename T, typename C_e, typename C_lt>
int AvlTree<T,C_e,C_lt>::Node::get_bs_factor()
{
    return get_height(left) - get_height(right);
}

template <typename T, typename C_e, typename C_lt>
bool AvlTree<T,C_e,C_lt>::Node::operator==(T &other_data)
{
    return tree -> cond_e(*data_ptr, other_data); 
}

template <typename T, typename C_e, typename C_lt>
bool AvlTree<T,C_e,C_lt>::Node::operator<(T &other_data) 
{
    return tree -> cond_lt(*data_ptr, other_data);
}

// avl tree implementation

// names like in tutorial 5
template <typename T, typename C_e, typename C_lt>
shared_ptr<typename AvlTree<T,C_e,C_lt>::Node> AvlTree<T,C_e,C_lt>::rr_roll(shared_ptr<Node> B)
{
    shared_ptr<Node> A = B->right;
    shared_ptr<Node> al = A->left;

    A->left = B;
    B->right = al;

    // updating parents
    if (al != nullptr)
    {
        al->parent = B;
    }
    A->parent = B->parent;
    B->parent = A;
    _update_parent_child(A->parent.lock(), A, B);

    B->update_height();
    A->update_height();

    return A;
}

template <typename T, typename C_e, typename C_lt>
shared_ptr<typename AvlTree<T,C_e,C_lt>::Node> AvlTree<T,C_e,C_lt>::ll_roll(shared_ptr<Node> B)
{
    shared_ptr<Node> A = B->left;
    shared_ptr<Node> ar = A->right;

    A->right = B;
    B->left = ar;

    // updating parents
    if (ar != nullptr)
    {
        ar->parent = B;
    }
    A->parent = B->parent;
    B->parent = A;
    _update_parent_child(A->parent.lock(), A, B);

    B->update_height();
    A->update_height();

    return A;
}

template <typename T, typename C_e, typename C_lt>
shared_ptr<typename AvlTree<T,C_e,C_lt>::Node> AvlTree<T,C_e,C_lt>::lr_roll(shared_ptr<Node> C)
{
    shared_ptr<Node> B = C->left;
    shared_ptr<Node> A = B->right;

    C->left = rr_roll(B);

    return ll_roll(C); // returns A
}

template <typename T, typename C_e, typename C_lt>
shared_ptr<typename AvlTree<T,C_e,C_lt>::Node> AvlTree<T,C_e,C_lt>::rl_roll(shared_ptr<Node> C)
{
    shared_ptr<Node> B = C->right;
    shared_ptr<Node> A = B->left;

    C->right = ll_roll(B);

    return rr_roll(C); // returns A
}

/**
 * @brief balances the tree with the algorithm shown at tutorial 5.
 * also update height as part of the process.
 *
 * @param curr_node
 */
template <typename T, typename C_e, typename C_lt>
void AvlTree<T,C_e,C_lt>::balance(shared_ptr<Node> curr_node)
{
    int old_height;
    int bs_factor;
    while (curr_node)
    {
        old_height = curr_node->height;
        curr_node->update_height();
        bs_factor = curr_node->get_bs_factor();
        if (bs_factor == 2 || bs_factor == -2)
        {
            curr_node = _choose_roll(curr_node);
            if (curr_node->parent.lock() == nullptr)
            {
                root = curr_node;
            }
        }
        if (curr_node->height == old_height)
        {
            return;
        }
        curr_node = curr_node->parent.lock();
    }
}

/**
 * @brief chooses the needed roll according to the table shown in the tutorial.
 *
 * @param curr
 * @return shared_ptr<AvlTree<T,C_e,C_lt>::Node>
 */
template <typename T, typename C_e, typename C_lt>
shared_ptr<typename AvlTree<T,C_e,C_lt>::Node> AvlTree<T,C_e,C_lt>::_choose_roll(shared_ptr<Node> curr)
{
    int bf_factor = curr->get_bs_factor();

    if (bf_factor == 2)
    {
        if (curr->left->get_bs_factor() >= 0)
        {
            return ll_roll(curr);
        }
        else
        { // curr -> left -> get_bs_factor == -1
            return lr_roll(curr);
        }
    }
    else
    { // bf_facror == -2
        if (curr->right->get_bs_factor() <= 0)
        {
            return rr_roll(curr);
        }
        else
        { // curr -> right -> get_bs_factor == 1
            return rl_roll(curr);
        }
    }
}

/**
 * @brief creates a node returns it. if baf_alloc then returns nullptr.
 *
 * @param data_ptr
 * @return shared_ptr<AvlTree<T,C_e,C_lt>::Node>
 */
template <typename T, typename C_e, typename C_lt>
shared_ptr<typename AvlTree<T,C_e,C_lt>::Node> AvlTree<T,C_e,C_lt>::_create_node(shared_ptr<T> data_ptr)
{
    shared_ptr<Node> new_node;
    try
    {
        new_node = make_shared<Node>(data_ptr, this);
    }
    catch (std::bad_alloc &exception)
    {
        return nullptr;
    }
    ++size;
    return new_node;
}

/**
 * @brief
 * returns parent of deleted node. if root was deleted returns nullptr
 *
 * @param to_del
 * @return shared_ptr<AvlTree<T,C_e,C_lt>::Node>
 */
template <typename T, typename C_e, typename C_lt>
shared_ptr<typename AvlTree<T,C_e,C_lt>::Node> AvlTree<T,C_e,C_lt>::_delete_node(shared_ptr<Node> to_del)
{
    shared_ptr<Node> parent = to_del->parent.lock();
    shared_ptr<Node> left = to_del->left;
    shared_ptr<Node> right = to_del->right;
    if (to_del == root)
    {
        if (left == nullptr && right == nullptr)
        {
            root = nullptr;
            --size;
        }
        else if (left == nullptr && right != nullptr)
        {
            root = right;
            right->parent.reset();
            --size;
        }
        else if (left != nullptr && right == nullptr)
        {
            root = left;
            left->parent.reset();
            --size;
        }
        else if (left != nullptr && right != nullptr)
        {
            shared_ptr<Node> to_swap = right;
            while (to_swap->left != nullptr)
            {
                to_swap = to_swap->left;
            }

            _swap_nodes(to_del, to_swap);
            parent = _delete_node(to_del);
        }
    }
    else
    {
        if (left == nullptr && right == nullptr)
        {
            _update_parent_child(parent, nullptr, to_del);
            --size;
        }
        else if (left == nullptr && right != nullptr)
        {
            right->parent = parent;
            _update_parent_child(parent, right, to_del);
            --size;
        }
        else if (left != nullptr && right == nullptr)
        {
            left->parent = parent;
            _update_parent_child(parent, left, to_del);
            --size;
        }
        else if (left != nullptr && right != nullptr)
        {
            shared_ptr<Node> to_swap = right;
            while (to_swap->left != nullptr)
            {
                to_swap = to_swap->left;
            }

            _swap_nodes(to_del, to_swap);
            parent = _delete_node(to_del);
        }
    }
    if (parent == nullptr)
    {
        parent = root;
    }
    return parent;
}

/**
 * @brief given a parent - updates its child in the correct place (left/right)
 * don't know why I avoided making this function.. its all I ever needed
 * @param parent
 * @param new_child
 * @param old_child
 */
template <typename T, typename C_e, typename C_lt>
void AvlTree<T,C_e,C_lt>::_update_parent_child(shared_ptr<Node> parent,
                                   shared_ptr<Node> new_child,
                                   shared_ptr<Node> old_child)
{
    if (parent == nullptr)
    {
        root = new_child;
        return;
    }

    if (parent->left == old_child)
    {
        parent->left = new_child;
    }
    else
    {
        parent->right = new_child;
    }
}

/**
 * @brief swaps nodes for the delete process.
 * takes care of all the ptr replacing in function.
 *
 * @param to_del
 * @param to_swap
 */
template <typename T, typename C_e, typename C_lt>
void AvlTree<T,C_e,C_lt>::_swap_nodes(shared_ptr<Node> to_del, shared_ptr<Node> to_swap)
{
    shared_ptr<Node> temp;
    shared_ptr<Node> to_del_parent = to_del->parent.lock();
    shared_ptr<Node> to_swap_parent = to_swap->parent.lock();
    // to_swap can't be root but to_del can
    temp = to_swap->right;
    if (to_del->right ==
        to_swap)
    { // case: nodes adjacent. to_swap have no left node.
        to_swap->right = to_del;
        to_del->parent = to_swap;
    }
    else
    {
        to_swap->right = to_del->right;
        to_swap->right->parent = to_swap;
        to_swap_parent->left = to_del; // it have to be the left child and
                                       // to_swap_parent cant be null
        to_del->parent = to_swap_parent;
    }
    to_swap->left = to_del->left;
    if (to_swap->left != nullptr)
    {
        to_swap->left->parent = to_swap;
    }
    to_del->right = temp;
    if (temp != nullptr)
    {
        temp->parent = to_del;
    }
    to_del->left = nullptr; // to_swap was the most left node.
    if (to_del_parent == nullptr)
    {
        to_swap->parent.reset();
        root = to_swap;
    }
    else
    {
        to_swap->parent = to_del_parent;
        if (to_del_parent->left == to_del)
        {
            to_del_parent->left = to_swap;
        }
        else
        {
            to_del_parent->right = to_swap;
        }
    }
}

/**
 * @brief adds a node the tree
 *
 * @param data_ptr
 * @return StatusType
 */
template <typename T, typename C_e, typename C_lt>
StatusType AvlTree<T,C_e,C_lt>::add_node(shared_ptr<T> data_ptr)
{
    if (root == nullptr)
    {
        root = _create_node(data_ptr);
        if (root != nullptr)
        {
            _update_smallest_elem();
            return StatusType::SUCCESS;
        }
        return StatusType::ALLOCATION_ERROR;
    }

    shared_ptr<Node> new_node;
    shared_ptr<Node> curr_node = root;

    while (curr_node != nullptr)
    {
        if (*curr_node == *data_ptr)
        {
            return StatusType::FAILURE;
        }
        else if (*curr_node < *data_ptr)
        {
            if (curr_node->right == nullptr)
            {
                new_node = _create_node(data_ptr);
                curr_node->right = new_node;
                break;
            }
            curr_node = curr_node->right;
        }
        else
        {
            if (curr_node->left == nullptr)
            {
                new_node = _create_node(data_ptr);
                curr_node->left = new_node;
                break;
            }
            curr_node = curr_node->left;
        }
    }
    if (new_node == nullptr)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    new_node->parent = curr_node;

    // balancing the tree
    balance(curr_node);

    _update_smallest_elem();
    return StatusType::SUCCESS;
}

/**
 * @brief removes a node from the tree
 *
 * @param data_ptr
 * @return the deleted node data
 */
template <typename T, typename C_e, typename C_lt>
template<typename U>
shared_ptr<T> AvlTree<T,C_e,C_lt>::remove_node(U&& dataT) // can refactor with get
{

    shared_ptr<Node> curr_node = root;

    while (curr_node != nullptr)
    {
        if (*curr_node == dataT)
        {
            auto data_to_del = curr_node -> data_ptr;
            curr_node = _delete_node(curr_node);
            if (curr_node != nullptr)
            {
                balance(curr_node);
            }
            _update_smallest_elem();
            return data_to_del;
        }
        if (*curr_node < dataT)
        {
            curr_node = curr_node->right;
        }
        else
        {
            curr_node = curr_node->left;
        }
    }
    return nullptr;
}


template <typename T, typename C_e, typename C_lt>
template <typename U>
shared_ptr<T> AvlTree<T,C_e,C_lt>::get(U&& dataT)
{
    shared_ptr<Node> curr_node = root;

    while (curr_node != nullptr)
    {
        if (*curr_node == dataT)
        {
            return curr_node -> data_ptr;
        }
        if (*curr_node < dataT)
        {
            curr_node = curr_node->right;
        }
        else
        {
            curr_node = curr_node->left;
        }
    }
    return nullptr;
}

template <typename T, typename C_e, typename C_lt>
void AvlTree<T,C_e,C_lt>::in_order(shared_ptr<Node> node)
{
    if (node == nullptr)
    {
        return;
    }

    in_order(node->left);
    // int bs_factor = node -> get_bs_factor();
    // if (bs_factor > 1 || bs_factor < -1)
    // {
    //     cout << "uneven!" << endl;
    // }
    cout << *(node->data_ptr) << endl;
    in_order(node->right);
}
template <typename T, typename C_e, typename C_lt>
void AvlTree<T,C_e,C_lt>::in_order_insertArr(int* ids)
{
    shared_ptr<Node> curr = root;
    shared_ptr<Node> prev = nullptr;
    int index = 0;

    if (curr == nullptr)
    {
        return;
    }

    while (curr != nullptr)
    {
        if (prev == nullptr || prev->right == curr) // if now at the start or got down to the right branch
        {
            while (curr->left != nullptr)
            {
                prev = curr;
                curr = curr->left;
            }
            ids[index++] = curr->data_ptr -> id;
            if (curr->right != nullptr) // if can go right
            {

                prev = curr;
                curr = curr->right;
                continue;
            }
            else
            {
                prev = curr;
                curr = curr->parent.lock();
                continue;
            }
        }

        if (curr->left == prev) // if rose from left
        {
            ids[index++] = curr->data_ptr -> id;
            if (curr->right != nullptr)
            {
                prev = curr;
                curr = curr->right;
            }
            else
            {
                prev = curr;
                curr = curr->parent.lock();
            }
        }
        else if (curr->right == prev) // rose from right
        {
            prev = curr;
            curr = curr->parent.lock();
        }
    }
}

/**
 * @brief does inorder and activate Func on each element (shard_ptr)
 *
 * @tparam Func the func to activate
 */
template <typename T, typename C_e, typename C_lt>
template<typename Func>
void AvlTree<T,C_e,C_lt>::in_order_Wfunc(Func func)
{
    shared_ptr<Node> curr = root;
    shared_ptr<Node> prev = nullptr;

    if (curr == nullptr)
    {
        return;
    }

    while (curr != nullptr)
    {
        if (prev == nullptr || prev->right == curr) // if now at the start or got down to the right branch
        {
            while (curr->left != nullptr)
            {
                prev = curr;
                curr = curr->left;
            }
            func(curr -> data_ptr);
            // cout << curr->data_ptr << endl;
            if (curr->right != nullptr) // if can go right
            {

                prev = curr;
                curr = curr->right;
                continue;
            }
            else
            {
                prev = curr;
                curr = curr->parent.lock();
                continue;
            }
        }

        if (curr->left == prev) // if rose from left
        {
            func(curr -> data_ptr);
            // cout << curr->data_ptr << endl;
            if (curr->right != nullptr)
            {
                prev = curr;
                curr = curr->right;
            }
            else
            {
                prev = curr;
                curr = curr->parent.lock();
            }
        }
        else if (curr->right == prev) // rose from right
        {
            prev = curr;
            curr = curr->parent.lock();
        }
    }
}


template <typename T, typename C_e, typename C_lt>
void AvlTree<T,C_e,C_lt>::_update_smallest_elem()
{
    shared_ptr<Node> curr = root;
    if (curr == nullptr) {
        smallest_elem = nullptr;
        return;
    }
    while (curr->left !=nullptr) {
        curr = curr->left;
    }
    smallest_elem = curr->data_ptr;
    
}
//generic declarations of compare funcs

template <typename T>
struct Id_e // struct cause default is public..
{
    bool operator()(T& a,T& b)
    {
        return a.id == b.id;
    }
};

template <typename T>
struct Id_lt
{
    bool operator()(T& a,T& b)
    {
        return a.id < b.id;
    }
};


template <typename T>
struct Complex_e // struct cause default is public..
{
    bool operator()(T& a,T& b)
    {
        return a.id == b.id;
    }
};

template <typename T>
struct Complex_lt
{
    bool operator()(T& a,T& b)
    {
        double a_rating = a.get_avg_rating();
        double b_rating = b.get_avg_rating();
        if (a_rating > b_rating) {
            return true; 
        }else if (a_rating < b_rating) {
            return false;
        }else if (a_rating == b_rating) {
            if (a.views > b.views) {
                return true; 
            }else if (a.views < b.views) {
                return false;
            }else if (a.views == b.views) {
                return a.id < b.id;                
            }
 
        }
        return false;

    }
};

template <typename T>
struct Def_e
{
    bool operator()(T& a,T& b)
    {
        return a== b;
    }
};

template <typename T>
struct Def_lt
{
    bool operator()(T& a,T& b)
    {
        return a< b;
    }
};
#endif // !AvlTree_h
