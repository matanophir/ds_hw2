#ifndef UF_h
#define UF_h

#include <memory>
using namespace std;

template <typename T>
class UF {
public:
    UF(shared_ptr<T> elems[],int num_of_elems);
    ~UF() = default;
    int find(int id, bool weighted = false);
    int unio(int p, int q);
    

private:
    
    struct Node;
    struct Group_Node;
    unique_ptr<Group_Node[]> groups;
    unique_ptr<Node[]> nodes;
    void _shrink_path(Node* first_node, Node* last_node,int w_sum);
    int num_of_elems;

    
};

template<typename T>
struct UF<T>::Node
{
    shared_ptr<T> data_ptr;
    Node* father;
    UF::Group_Node* group_ptr;
    int weight;
    int id;

    // Node();
    // ~Node();
};

template<typename T>
struct UF<T>::Group_Node
{
    UF::Node* head;
    int height;
    int id;
    int size;

    // Group_Node();
    // ~Group_Node();
    
};

template<typename T>
UF<T>::UF(shared_ptr<T> elems[],int num_of_elems) {
    this->num_of_elems = num_of_elems;
    groups = unique_ptr<Group_Node[]>(new Group_Node[num_of_elems]);
    nodes = unique_ptr<Node[]>(new Node[num_of_elems]);

    for (int i=0; i<num_of_elems; ++i) {
        Group_Node& group = groups[i];
        Node& node = nodes[i];

        node.data_ptr = elems[i];
        node.father= nullptr;
        node.group_ptr = &group;
        node.weight = 0;
        node.id = i;

        group.head = &node;
        group.height = node.data_ptr->copies; // beats the purpose of templated class..
        group.id = i;
        group.size = 1;

    }

}


template<typename T>
int UF<T>::find(int id, bool weighted)
{
    if (id < 0 || id >= num_of_elems )
        return -1;
    Node* first_node = &nodes[id];
    Node* node = &nodes[id];
    int w_sum = node->weight;
    while (node->group_ptr == nullptr)
    {
        node = node->father;
        w_sum+= node->weight;
    }

    if (first_node->id != node->id || (first_node->father != nullptr && first_node->father->id != node->id))
        _shrink_path(first_node, node, w_sum);
    
    if (weighted)
        return w_sum;
    return node->group_ptr->id;


}

template<typename T>
void UF<T>::_shrink_path(Node* first_node, Node* last_node,int w_sum)
{
    int sum = w_sum - first_node->weight - last_node->weight;
    Node* node = first_node;
    Node* next_node = first_node->father;
    while (next_node->group_ptr == nullptr)
    {
        node->weight+=sum;
        sum -= next_node->weight;
        node = next_node;
        next_node = next_node->father;
    }
}
    

/**
 * @brief unify the group that p in to group that q in.
 *
 * @tparam T the data ptr type
 * @param p id of the first elemnt
 * @param q id of the second element
 * @return the new group id (q's group)
 */
template<typename T>
int UF<T>::unio(int p, int q)
{
    if (p < 0 || p >= num_of_elems || q < 0 || q >=num_of_elems )
        return -1;
    Group_Node* p_group= &groups[find(p)];
    Group_Node* q_group= &groups[find(q)];
    Node* p_head = p_group->head;
    Node* q_head = q_group->head;

    if(p_group == q_group)
        return q_group->id;

    
    if (p_group->size <= q_group->size) // case pointing p to q;
    {
        p_head->father = q_head;
        p_head->group_ptr = nullptr;
        p_head->weight = p_head->weight + q_group->height - q_head->weight;

    }else { // (p_group->size > q_group->size)
        p_head->weight+= q_group->height;
        p_head->group_ptr = q_group;

        q_head->father = p_head;
        q_head->group_ptr = nullptr;
        q_head->weight -= p_head->weight;

        q_group->head= p_head;
    
    }
    q_group->height += p_group->height;
    p_group->head = nullptr;
    q_group->size += p_group->size; 

    return q_group->id;
}
#endif // !UF_h
