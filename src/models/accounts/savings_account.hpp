#pragma once

#include "models/accounts/account_base.hpp"

namespace financesim {

/// Helper to create default schedule for savings accounts (monthly interest)
inline Schedule make_savings_schedule() {
    Schedule s;
    s.rate = 30.0;  // Monthly interest calculation
    return s;
}

/// Savings account - earns APY interest, compounded monthly
class SavingsAccount : public AccountBase {
public:
    /// Create a savings account
    /// @param id Unique identifier
    /// @param name Human-readable name
    /// @param apy Annual Percentage Yield (e.g., 0.05 for 5%)
    /// @param initial_balance Starting balance
    /// @param routing_tag Tag for routing events (defaults to "savings")
    /// @param schedule Execution schedule (default: monthly updates for interest)
    SavingsAccount(std::string id, std::string name,
                   double apy,
                   double initial_balance = 0.0,
                   std::string routing_tag = "savings",
                   Schedule schedule = make_savings_schedule());
    ~SavingsAccount() override = default;

    /// Get the APY (annual percentage yield)
    double apy() const { return apy_; }

    void initialize(EventBus& bus) override;
    void update(SimTime time) override;
    void finalize() override;
    void reset() override;

protected:
    void on_income(const IncomeEvent& event) override;
    void on_expense(const ExpenseEvent& event) override;

private:
    void accrue_interest_up_to(SimTime time);

    double apy_;
    SimTime last_interest_time_ = 0.0;
};

} // namespace financesim
