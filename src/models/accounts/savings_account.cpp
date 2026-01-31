#include "models/accounts/savings_account.hpp"
#include <cmath>

namespace financesim {

SavingsAccount::SavingsAccount(std::string id, std::string name,
                               double apy,
                               double initial_balance,
                               std::string routing_tag,
                               Schedule schedule)
    : AccountBase(std::move(id), std::move(name),
                  std::move(routing_tag), initial_balance,
                  schedule)
    , apy_(apy)
    , last_interest_time_(0.0) {
}

void SavingsAccount::initialize(EventBus& bus) {
    AccountBase::initialize(bus);
    // Interest is calculated event-driven, no scheduled updates needed
}

void SavingsAccount::update(SimTime time) {
    // Savings account is event-driven, but update can be called
    // to accrue interest up to the current time
    accrue_interest_up_to(time);
}

void SavingsAccount::finalize() {
    // Note: We can't accrue final interest here since we don't know
    // the final simulation time. The simulation should call update()
    // with the final time before finalizing.
    AccountBase::finalize();
}

void SavingsAccount::reset() {
    AccountBase::reset();
    last_interest_time_ = 0.0;
}

void SavingsAccount::on_income(const IncomeEvent& event) {
    // Accrue interest up to this event's time before depositing
    accrue_interest_up_to(event.timestamp());
    AccountBase::on_income(event);
}

void SavingsAccount::on_expense(const ExpenseEvent& event) {
    // Accrue interest up to this event's time before withdrawing
    accrue_interest_up_to(event.timestamp());
    AccountBase::on_expense(event);
}

void SavingsAccount::accrue_interest_up_to(SimTime time) {
    if (balance() <= 0 || time <= last_interest_time_) return;

    // Monthly interest rate (APY compounded monthly)
    // Monthly rate = (1 + APY)^(1/12) - 1
    constexpr double days_per_month = 30.0;
    double monthly_rate = std::pow(1.0 + apy_, 1.0 / 12.0) - 1.0;

    // Accrue interest for each complete month
    while (time >= last_interest_time_ + days_per_month) {
        double interest = balance() * monthly_rate;
        last_interest_time_ += days_per_month;
        if (interest > 0) {
            deposit(last_interest_time_, interest, "interest");
        }
    }
}

} // namespace financesim
