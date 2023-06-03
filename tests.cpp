#include "Dictionary.h"
#include "Set.h"
#include "Customer.h"
#include "Record.h"
#include "utilesWet2.h"
#include <memory>
#include <stdio.h>
using namespace std;

int main (int argc, char *argv[]) {
    
    // dict tests
    StatusType status;
    auto cust1 = make_shared<Customer>(34667);
    auto cust2 = make_shared<Customer>(55523);
    auto cust3 = make_shared<Customer>(8574);
    auto cust4 = make_shared<Customer>(773);
    Dictionary<Customer> dict1;
    status = dict1.insert(cust1);
    status = dict1.insert(cust2);
    status = dict1.insert(cust3);
    status = dict1.insert(cust4);

    auto cust = dict1.get(773);
    cust = dict1.get(599);

    dict1.del(cust1->id);
    dict1.del(cust2->id);
    dict1.del(cust3->id);
    dict1.del(cust4->id);
    cout << status;
    return 0;
}
