#include "models/expenses/rent_expense.hpp"

namespace financesim {

RentExpense::RentExpense(str::string id, std::string name, double monthly_rent, SimTime start_day) 
    : ExpensesBase(std::move(id), std::move(name),
                    schedule{start_day, -1, 30, ExecutionTiming::StartOfPeriod})
                    , monthly_rent_(monthly_rent)
{
}

void RentExpense::update(SimTime time){
    emit<ExpenseEvent>(time, monthly_rent_);
}

void RentExpense::reset(){
    ExpenseBase::reset();
}


}