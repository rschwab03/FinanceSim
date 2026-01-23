#include "models/expenses_base.hpp"

namespace financesim {

ExpensesBase::ExpensesBase(std::string id, std::string name, Schedule schedule)
    : id_(std::move(id))
    , name_(std::move(name))
    , schedule_(schedule) {}

void ExpensesBase::initialize(EventBus& bus) {
    bus_ = &bus;
    // TODO: Subscribe to relevant events
}

void ExpensesBase::update(SimTime time) {
    // TODO: Implement in derived classes
}

void ExpensesBase::finalize() {
    // TODO: Cleanup
}

void ExpensesBase::reset() {
    bus_ = nullptr;
    // TODO: Reset state
}

} // namespace financesim
