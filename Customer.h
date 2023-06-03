
#ifndef Customer_h
#define Customer_h

class Customer {
public:
    Customer(int id, int phone): id(id), phone(phone), monthly_expenses(0), member(false){};
    ~Customer() = default;

    int id;
    int phone;
    int monthly_expenses;
    bool member;
private: // might as well just do struct
};

#endif // !Customer_h
