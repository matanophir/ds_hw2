
#ifndef Record_h
#define Record_h

class Record {
public:
    Record(int id, int copies):id(id), copies(copies), month_purchases(0){};
    ~Record() = default;

    int id;
    int copies;
    int month_purchases;
private:
    
};      

#endif // !Record_h
