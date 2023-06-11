#include "Dictionary.h"
#include "UF.h"
#include "Customer.h"
#include "Record.h"
#include "RankTree.h"
#include "utilesWet2.h"
#include <memory>
#include <stdio.h>
#include "recordsCompany.h"

using namespace std;
#define SIZE 5

int main (int argc, char *argv[]) {
    
    StatusType status(StatusType::SUCCESS);
    bool result= false;

    // //recordsCompany tests
    // RecordsCompany* rc = new RecordsCompany();
    // int rec[5] = {1,2,3,4,5};
    //
    // status = rc->newMonth(rec, 5);
    // cout << result;
    //
    // delete rc;



    // //RankTree test
    // shared_ptr<Customer> arr[10];
    // RankTree<Customer, Def_e<Customer>, Def_lt<Customer>> tree1;
    // for (int i=0; i<SIZE; ++i) {
    //     arr[i] = make_shared<Customer>(i);
    //     tree1.add_node(arr[i]);
    // }
    // tree1.add_extra(*arr[2], 5);
    // tree1.add_extra(*arr[3], 5);
    //
    // result = tree1.get_extra(*arr[0]) == 10;
    // result = tree1.get_extra(*arr[1]) == 10;
    // result = tree1.get_extra(*arr[2]) == 5;
    // result = tree1.get_extra(*arr[3]) == 0;
    // result = tree1.get_extra(*arr[4]) == 0;
    //
    // tree1.remove_node(*arr[0]); // checking after deletion roll
    // result = tree1.get_extra(*arr[1]) == 10;
    // result = tree1.get_extra(*arr[2]) == 5;
    // result = tree1.get_extra(*arr[3]) == 0;
    // result = tree1.get_extra(*arr[4]) == 0;
    //
    // tree1.add_node(arr[0]); // check the cancelling of new node
    // result = tree1.get_extra(*arr[0]) == 0;
    // result = tree1.get_extra(*arr[1]) == 10;
    // result = tree1.get_extra(*arr[2]) == 5;
    // result = tree1.get_extra(*arr[3]) == 0;
    // result = tree1.get_extra(*arr[4]) == 0;
    //
    //
    // for (int i=5; i<10; ++i) { // check after alot of rolls
    //     arr[i] = make_shared<Customer>(i);
    //     tree1.add_node(arr[i]);
    // }
    // result = tree1.get_extra(*arr[0]) == 0;
    // result = tree1.get_extra(*arr[1]) == 10;
    // result = tree1.get_extra(*arr[2]) == 5;
    // result = tree1.get_extra(*arr[3]) == 0;
    // result = tree1.get_extra(*arr[4]) == 0;
    //
    //
    // // check ranges
    // result = tree1.get_extra(*arr[0]) == 0;
    // result = tree1.get_extra(*arr[1]) == 10;
    // result = tree1.get_extra(*arr[2]) == 5;
    // result = tree1.get_extra(*arr[3]) == 0;
    // result = tree1.get_extra(*arr[4]) == 0;
    // result = tree1.get_extra(*arr[5]) == 0;
    // result = tree1.get_extra(*arr[6]) == 0;
    // result = tree1.get_extra(*arr[7]) == 0;
    // result = tree1.get_extra(*arr[8]) == 0;
    // result = tree1.get_extra(*arr[9]) == 0;
    //
    //
    // tree1.add_extra(*arr[8], 7);
    // tree1.add_extra(*arr[5], -7);
    //
    // tree1.add_extra(Customer(14), 8);
    // tree1.add_extra(*arr[9], -8);
    // result = tree1.get_extra(*arr[0]) == 0;
    // result = tree1.get_extra(*arr[1]) == 10;
    // result = tree1.get_extra(*arr[2]) == 5;
    // result = tree1.get_extra(*arr[3]) == 0;
    // result = tree1.get_extra(*arr[4]) == 0;
    // result = tree1.get_extra(*arr[5]) == 7;
    // result = tree1.get_extra(*arr[6]) == 7;
    // result = tree1.get_extra(*arr[7]) == 7;
    // result = tree1.get_extra(*arr[8]) == 0;
    // result = tree1.get_extra(*arr[9]) == 8;
    //
    // cout << result;


    //UF tests
    int records[SIZE]={2,3,4};
    shared_ptr<Record>* arr = new shared_ptr<Record>[5];
    for (int i=0; i<SIZE; ++i) {
        arr[i] = make_shared<Record>(i,records[i]);
    }

    UF<Record> uf(arr,SIZE);
    auto record = uf.get_elem(0);
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
