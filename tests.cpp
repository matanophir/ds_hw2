#include "Dictionary.h"
#include "UF.h"
#include "Customer.h"
#include "Record.h"
#include "utilesWet2.h"
#include <memory>
#include <stdio.h>
using namespace std;
#define SIZE 5

int main (int argc, char *argv[]) {
    
    StatusType status(StatusType::SUCCESS);

    //UF tests
    bool result= false;
    int records[SIZE]={2,3,4};
    shared_ptr<Record>* arr = new shared_ptr<Record>[5];
    for (int i=0; i<SIZE; ++i) {
        arr[i] = make_shared<Record>(i,records[i]);
    }

    UF<Record> uf(arr,SIZE);
    result = uf.find(0) == 0;
    uf.unio(0, 1);
    result = uf.find(0) == 1;
    result = uf.find(0,true) == 3;
    uf.unio(0, 2);
    result = uf.find(0) == 2;
    result = uf.find(0,true) == 7;

    result = uf.find(1) == 2;
    result = uf.find(1,true) == 4;

    result = uf.find(2) == 2;
    result = uf.find(2,true) == 0;

    uf.unio(0, 2); // These are already unified
    result = uf.find(0) == 2;

    result = uf.find(3) == 3; // Assuming that SIZE > 3


    cout << result;
    
    delete [] arr;

    // Unifying a structure with more than two elements
    int records2[SIZE] = {5, 6, 7, 8, 9};
    shared_ptr<Record>* arr2 = new shared_ptr<Record>[SIZE];
    for (int i = 0; i < SIZE; ++i) {
        arr2[i] = make_shared<Record>(i, records2[i]);
    }
    UF<Record> uf2(arr2, SIZE);
    uf2.unio(0, 1);
    uf2.unio(1, 2);
    result = uf2.find(0) == 2;
    result = uf2.find(1) == 2;
    result = uf2.find(2) == 2;
    result = uf2.find(0,true) == 13;
    cout << result;

    // Check data after multiple unifications
    uf2.unio(3, 4);
    uf2.unio(0, 4);
    result = uf2.find(3,true) == 9;
    result = uf2.find(0,true) == 30;
    
    result = uf2.find(0) == 4;
    result = uf2.find(1) == 4;
    result = uf2.find(2) == 4;
    result = uf2.find(3) == 4;
    result = uf2.find(4) == 4;
    cout << result;

    delete [] arr2;


    // // dict tests
    // auto cust1 = make_shared<Customer>(34667);
    // auto cust2 = make_shared<Customer>(55523);
    // auto cust3 = make_shared<Customer>(8574);
    // auto cust4 = make_shared<Customer>(773);
    // Dictionary<Customer> dict1;
    // status = dict1.insert(cust1);
    // status = dict1.insert(cust2);
    // status = dict1.insert(cust3);
    // status = dict1.insert(cust4);
    //
    // auto cust = dict1.get(773);
    // cust = dict1.get(599);
    //
    // dict1.del(cust1->id);
    // dict1.del(cust2->id);
    // dict1.del(cust3->id);
    // dict1.del(cust4->id);
    cout << status;
    return 0;
}
