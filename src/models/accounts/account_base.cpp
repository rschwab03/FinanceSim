#include "models/accounts/account_base.hpp"

namespace financesim {

AccountBase::AccountBase(std::string id, std::string name, std::string routing_tag,
                         double initial_balance,
                         Schedule schedule)
    : id_(std::move(id))
    , name_(std::move(name))
    , routing_tag_(std::move(routing_tag))
    , balance_(initial_balance)
    , initial_balance_(initial_balance)
    , schedule_(schedule) {
}

void AccountBase::initialize(EventBus& bus) {
    bus_ = &bus;

    // Subscribe to income events
    income_sub_ = bus.subscribe<IncomeEvent>(
        [this](const IncomeEvent& event) {
            if (should_handle_event(event.target_account())) {
                on_income(event);
            }
        }
    );

    // Subscribe to expense events
    expense_sub_ = bus.subscribe<ExpenseEvent>(
        [this](const ExpenseEvent& event) {
            if (should_handle_event(event.target_account())) {
                on_expense(event);
            }
        }
    );
}

void AccountBase::update(SimTime /*time*/) {
    // Base account has no scheduled updates
    // Derived classes (like SavingsAccount) may override for interest calculations
}

void AccountBase::finalize() {
    // Nothing to finalize
}

void AccountBase::reset() {
    if (bus_) {
        bus_->unsubscribe(income_sub_);
        bus_->unsubscribe(expense_sub_);
    }
    bus_ = nullptr;
    income_sub_ = 0;
    expense_sub_ = 0;
    balance_ = initial_balance_;
}

bool AccountBase::should_handle_event(const std::string& target_account) const {
    if (target_account.empty()) {
        // No explicit target - accept only if we're the default
        return routing_tag_ == "default";
    }
    // Explicit target - accept only if we match
    return target_account == routing_tag_;
}

void AccountBase::deposit(SimTime time, double amount, const std::string& reason) {
    if (amount <= 0) return;
    balance_ += amount;
    emit<AccountEvent>(time, id_, balance_, amount, reason);
}

void AccountBase::withdraw(SimTime time, double amount, const std::string& reason) {
    if (amount <= 0) return;
    balance_ -= amount;
    emit<AccountEvent>(time, id_, balance_, -amount, reason);
}

void AccountBase::on_income(const IncomeEvent& event) {
    deposit(event.timestamp(), event.amount(), event.category());
}

void AccountBase::on_expense(const ExpenseEvent& event) {
    withdraw(event.timestamp(), event.amount(), event.category());
}

} // namespace financesim
