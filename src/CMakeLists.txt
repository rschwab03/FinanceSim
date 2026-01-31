# Core library (headers only)
add_library(financesim_core INTERFACE)
target_include_directories(financesim_core INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Models library
add_library(financesim_models STATIC
    core/log_writer.cpp
    core/logger.cpp
    models/income/income_base.cpp
    models/income/career_job.cpp
    models/expenses/expenses_base.cpp
    models/assets/assets_base.cpp
    models/liabilities/liabilities_base.cpp
    models/accounts/account_base.cpp
    models/accounts/checking_account.cpp
    models/accounts/savings_account.cpp
)
target_link_libraries(financesim_models PUBLIC financesim_core)

# Python bindings
pybind11_add_module(financesim_cpp
    bindings/bindings.cpp
)
target_link_libraries(financesim_cpp PRIVATE financesim_models)

# Install the Python module to the system directory
set_target_properties(financesim_cpp PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/system
)
