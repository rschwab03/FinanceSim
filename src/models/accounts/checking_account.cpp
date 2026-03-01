#include "models/accounts/checking_account.hpp"

namespace financesim {

CheckingAccount::CheckingAccount(std::string id, std::string name,
                                 double initial_balance,
                                 std::string routing_tag,
                                 Schedule schedule)
    : AccountBase(std::move(id), std::move(name),
                  std::move(routing_tag), initial_balance,
                  schedule) {
}

} // namespace financesim
