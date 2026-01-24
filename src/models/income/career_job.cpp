#include "models/income/career_job.hpp"
#include "core/event.hpp"

namespace financesim {

namespace {
    // Semi-monthly payment interval: ~15.22 days (365 / 24 payments)
    constexpr double SEMI_MONTHLY_DAYS = 365.0 / 24.0;
}

CareerJob::CareerJob(std::string id, std::string name, double annual_salary, SimTime start_day)
    : IncomeBase(std::move(id), std::move(name),
                 Schedule{start_day, -1.0, SEMI_MONTHLY_DAYS, ExecutionTiming::EndOfPeriod})
    , annual_salary_(annual_salary)
    , payment_amount_(annual_salary / 24.0)
{
}

void CareerJob::update(SimTime time) {
    emit<IncomeEvent>(time, payment_amount_, "salary");
}

void CareerJob::reset() {
    IncomeBase::reset();
}

} // namespace financesim
