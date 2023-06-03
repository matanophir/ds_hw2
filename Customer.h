
#ifndef Customer_h
#define Customer_h

class Customer {
public:
    explicit Customer(int id, int phone = 0): id(id), phone(phone), monthly_expenses(0), member(false){};
    bool operator==(const Customer& other){return id == other.id;};
    bool operator<(const Customer& other){return id < other.id;};
    ~Customer() = default;

    // int get_hash_id(){return id;}; // if want to hash with different params

    int id;
    int phone;
    int monthly_expenses;
    bool member;
private: // might as well just do struct
};

#endif // !Customer_h
