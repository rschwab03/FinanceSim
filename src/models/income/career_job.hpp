#pragma once

#include "models/income/income_base.hpp"

namespace financesim {

/// @brief Career job income model with semi-monthly salary payments
///
/// Represents a salaried career position that pays twice per month.
/// The payment amount is calculated as annual_salary / 24.
class CareerJob : public IncomeBase {
public:
    /// @brief Construct a CareerJob model
    /// @param id Unique identifier for this model instance
    /// @param name Human-readable name (e.g., "Software Engineer at Acme Corp")
    /// @param annual_salary Gross annual salary in dollars
    /// @param start_day Day (in SimTime) when employment begins
    CareerJob(std::string id, std::string name, double annual_salary, SimTime start_day = 0.0);

    /// @brief Get the annual salary
    double annual_salary() const { return annual_salary_; }

    /// @brief Get the per-paycheck amount (annual / 24)
    double payment_amount() const { return payment_amount_; }

    void update(SimTime time) override;
    void reset() override;

private:
    double annual_salary_;
    double payment_amount_;
};

} // namespace financesim
