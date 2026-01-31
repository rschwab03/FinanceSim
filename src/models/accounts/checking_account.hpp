#pragma once

#include "models/accounts/account_base.hpp"

namespace financesim {

/// Checking account - the default income recipient, no interest earned
class CheckingAccount : public AccountBase {
public:
    /// Create a checking account
    /// @param id Unique identifier
    /// @param name Human-readable name
    /// @param initial_balance Starting balance
    /// @param routing_tag Tag for routing events (defaults to "default")
    /// @param schedule Execution schedule (default: no periodic updates)
    CheckingAccount(std::string id, std::string name,
                    double initial_balance = 0.0,
                    std::string routing_tag = "default",
                    Schedule schedule = make_account_schedule());
    ~CheckingAccount() override = default;
};

} // namespace financesim
