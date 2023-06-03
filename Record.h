
#ifndef Record_h
#define Record_h

class Record {
public:
    explicit Record(int id, int copies = 1):id(id), copies(copies), month_purchases(0){};
    bool operator==(const Record& other){return id == other.id;};
    bool operator<(const Record& other){return id < other.id;};
    ~Record() = default;

    // int get_hash_id(){return id;}; 

    int id;
    int copies;
    int month_purchases;
private:
    
};      

#endif // !Record_h
