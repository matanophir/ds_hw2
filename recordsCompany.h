#ifndef RECORDS_COMPANY_H
#define RECORDS_COMPANY_H

#include "utilesWet2.h"
#include "AvlTree.h"
#include "RankTree.h"
#include "Dictionary.h"
#include "UF.h"
#include "Customer.h"
#include "Record.h"
#include <memory>


class RecordsCompany {
  private:
    // todo

      Dictionary<Customer> customers_dict;
      RankTree<Customer, Def_e<Customer>, Def_lt<Customer>> members_rtree;
      unique_ptr<UF<Record>> records_uf;

      typedef RankTree<Customer, Def_e<Customer>, Def_lt<Customer>>::Node Node;

  public:
    RecordsCompany():customers_dict(Dictionary<Customer>()), 
                    members_rtree(RankTree<Customer, Def_e<Customer>, Def_lt<Customer>>()), 
                    records_uf(nullptr){};
    ~RecordsCompany() = default;
    StatusType newMonth(int *records_stocks, int number_of_records);
    StatusType addCostumer(int c_id, int phone);
    Output_t<int> getPhone(int c_id);
    StatusType makeMember(int c_id);
    Output_t<bool> isMember(int c_id);
    StatusType buyRecord(int c_id, int r_id);
    StatusType addPrize(int c_id1, int c_id2, double  amount);
    Output_t<double> getExpenses(int c_id);
    StatusType putOnTop(int r_id1, int r_id2);
    StatusType getPlace(int r_id, int *column, int *hight);
};

#endif
