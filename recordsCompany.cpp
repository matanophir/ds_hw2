#include "recordsCompany.h"
#include "Customer.h"
#include "Record.h"
#include "UF.h"
#include "utilesWet2.h"
#include <cstddef>
#include <memory>
#include <new>



StatusType RecordsCompany::newMonth(int *records_stocks, int number_of_records)
{
    if (number_of_records < 0)
        return StatusType::INVALID_INPUT;

    // makes new records UF
    try {
        shared_ptr<Record>* new_records = new shared_ptr<Record>[number_of_records];
        for (int i=0; i<number_of_records; ++i) {
            new_records[i] = make_shared<Record>(i,records_stocks[i]);
        }
        records_uf= unique_ptr<UF<Record>>(new UF<Record>(new_records,number_of_records));
        delete [] new_records;
    }
    catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }

    //resets all the expenses
    auto customer_reset_func = [] (shared_ptr<Customer> c_ptr) {c_ptr->monthly_expenses=0;};
    customers_dict.func_on_all_elems(customer_reset_func);


    //resets all the awards
    auto node_extra_reset_func = [] (shared_ptr<Node> node_ptr) {node_ptr->extra = 0;};
    members_rtree.in_order_WNodefunc(node_extra_reset_func);
    

    return StatusType::SUCCESS;
}


StatusType RecordsCompany::addCostumer(int c_id, int phone)
{
    if (c_id <0 || phone < 0)
        return INVALID_INPUT;
    StatusType status;
    try {
        auto customer = make_shared<Customer>(c_id,phone);
        status = customers_dict.insert(customer);
    } catch (const std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }
    return status;

}

Output_t<int> RecordsCompany::getPhone(int c_id)
{
    Output_t<int> output;
    if(c_id<0)
        return Output_t<int>(StatusType::INVALID_INPUT);

    auto customer_ptr = customers_dict.get(c_id);
    if (customer_ptr == nullptr)
        return Output_t<int>(StatusType::DOESNT_EXISTS);

    return Output_t<int>(customer_ptr->phone);
}

StatusType RecordsCompany::makeMember(int c_id)
{
    if (c_id < 0)
        return StatusType::INVALID_INPUT;

    auto customer_ptr = customers_dict.get(c_id);
    if (customer_ptr == nullptr)
        return StatusType::DOESNT_EXISTS;
    if(customer_ptr->member)
        return StatusType::ALREADY_EXISTS;

    try {
        members_rtree.add_node(customer_ptr);
        customer_ptr->member = true;
    } catch (const std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

Output_t<bool> RecordsCompany::isMember(int c_id)
{
    if (c_id < 0)
        return Output_t<bool>(StatusType::INVALID_INPUT);

    auto customer_ptr = customers_dict.get(c_id);
    if (customer_ptr == nullptr)
        return Output_t<bool>(StatusType::DOESNT_EXISTS);

    return Output_t<bool>(customer_ptr->member);

}

StatusType RecordsCompany::buyRecord(int c_id, int r_id)
{
    if (c_id < 0 || r_id < 0)
        return StatusType::INVALID_INPUT;

    auto customer_ptr = customers_dict.get(c_id);
    if (customer_ptr == nullptr || records_uf->num_of_elems <= r_id)
        return StatusType::DOESNT_EXISTS;

    auto& record = *records_uf->get_elem(r_id);
    int price = 100 + record.month_purchases;
    ++record.month_purchases;
    if (customer_ptr->member)
        customer_ptr->monthly_expenses += price;

    return StatusType::SUCCESS;

}

StatusType RecordsCompany::addPrize(int c_id1, int c_id2, double  amount)
{
    if (c_id1 < 0 || c_id2 < c_id1 || amount <=0)
        return StatusType::INVALID_INPUT;
    if (c_id1 == c_id2)
        return StatusType::SUCCESS;

    members_rtree.add_extra(Customer(c_id2), amount);
    members_rtree.add_extra(Customer(c_id1), -amount);

    return StatusType::SUCCESS;
        
}

Output_t<double> RecordsCompany::getExpenses(int c_id)
{
    if (c_id < 0)
        return Output_t<double>(StatusType::INVALID_INPUT);
    
    int prize;
    auto member_ptr = members_rtree.get(Customer(c_id));
    if (member_ptr == nullptr)
        return Output_t<double>(StatusType::DOESNT_EXISTS);
    prize = members_rtree.get_extra(*member_ptr);
    return Output_t<double>(double(member_ptr->monthly_expenses) - prize);

}

StatusType RecordsCompany::putOnTop(int r_id1, int r_id2)
{
    if (r_id1 < 0 || r_id2 < 0)
        return StatusType::INVALID_INPUT;
    if ((r_id1 < 0 || r_id1 >= records_uf->num_of_elems ) || ((r_id2 < 0 || r_id2 >= records_uf->num_of_elems )))
        return StatusType::DOESNT_EXISTS;

    int id1_group = records_uf->find(r_id1);
    int id2_group = records_uf->find(r_id2);
    if (id1_group == id2_group)
        return StatusType::FAILURE;

    records_uf->unio(r_id1, r_id2);
    return StatusType::SUCCESS;
}

StatusType RecordsCompany::getPlace(int r_id, int *column, int *hight)
{
    if (r_id < 0 || column ==NULL || hight == NULL)
        return  StatusType::INVALID_INPUT;
    if (r_id < 0 || r_id >= records_uf->num_of_elems )
        return StatusType::DOESNT_EXISTS; // needed?
    
    *column = records_uf->find(r_id);
    *hight = records_uf->find(r_id,true);

    return StatusType::SUCCESS;
}

