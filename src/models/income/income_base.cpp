#include "models/income/income_base.hpp"

namespace financesim {

IncomeBase::IncomeBase(std::string id, std::string name, Schedule schedule)
    : id_(std::move(id))
    , name_(std::move(name))
    , schedule_(schedule) {}

void IncomeBase::initialize(EventBus& bus) {
    bus_ = &bus;
    // TODO: Subscribe to relevant events
}

void IncomeBase::update(SimTime time) {
    // TODO: Implement in derived classes
}

void IncomeBase::finalize() {
    // TODO: Cleanup
}

void IncomeBase::reset() {
    bus_ = nullptr;
    // TODO: Reset state
}

} // namespace financesim
