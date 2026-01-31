#pragma once

#include "core/model.hpp"
#include "core/event.hpp"

namespace financesim {

/// Helper to create default schedule for accounts (no periodic updates)
inline Schedule make_account_schedule() {
    Schedule s;
    s.rate = 0.0;  // Event-driven only, no periodic updates
    return s;
}

/// Base class for account models (checking, savings, etc.)
/// Accounts subscribe to IncomeEvent and ExpenseEvent and route based on target_account
class AccountBase : public Model {
public:
    AccountBase(std::string id, std::string name, std::string routing_tag,
                double initial_balance = 0.0,
                Schedule schedule = make_account_schedule());
    virtual ~AccountBase() = default;

    const std::string& id() const override { return id_; }
    const std::string& name() const override { return name_; }
    const Schedule& schedule() const override { return schedule_; }

    /// Get the routing tag used to match income/expense events
    const std::string& routing_tag() const { return routing_tag_; }

    /// Get current balance
    double balance() const { return balance_; }

    void initialize(EventBus& bus) override;
    void update(SimTime time) override;
    void finalize() override;
    void reset() override;

protected:
    EventBus& bus() { return *bus_; }

    /// Deposit funds into the account
    void deposit(SimTime time, double amount, const std::string& reason);

    /// Withdraw funds from the account
    void withdraw(SimTime time, double amount, const std::string& reason);

    /// Called when an income event is routed to this account
    virtual void on_income(const IncomeEvent& event);

    /// Called when an expense event is routed to this account
    virtual void on_expense(const ExpenseEvent& event);

    template<typename EventType, typename... Args>
    void emit(SimTime time, Args&&... args) {
        if (!bus_) return;
        std::shared_ptr<const EventType> event = std::make_shared<EventType>(
            time, id_, std::forward<Args>(args)...
        );
        bus_->publish(event);
    }

private:
    bool should_handle_event(const std::string& target_account) const;

    std::string id_;
    std::string name_;
    std::string routing_tag_;
    double balance_;
    double initial_balance_;
    Schedule schedule_;
    EventBus* bus_ = nullptr;
    SubscriptionId income_sub_ = 0;
    SubscriptionId expense_sub_ = 0;
};

} // namespace financesim
