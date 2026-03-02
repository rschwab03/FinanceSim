#pragma once

#include "models/expenses/expenses_base.hpp"

namespace financesim{
    /// @brief rent expense model with monthly collections
    ///
    /// Represents renting a house/apartment/whatever 
    /// Cost of rent is expected to be prepresented by monthly cost
    class RentExpense : ExpensesBase {
        public:
        /// @brief Construct a RentExpense model
        /// @param id Unique identifier for this model instance
        /// @param name Human-readable name (e.g., "Beachfront Apartment")
        /// @param monthly_rent cost of rent per month
        /// @param start_day Day (in SimTime) when landlord starts collecting rent
        RentExpense(std::string id, std::string name, double monthly_rent, SimTime start_day = 0.0);

        /// @brief Get monthly rent expense
        double monthly_rent() const { return monthly_rent_; }

        void update(SimTime time) override;
        void reset() override;
        
        private:
        double monthly_rent_;

    };

} // namespace financesim
