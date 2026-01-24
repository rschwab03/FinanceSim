"""Senior engineer scenario - experienced engineer, higher salary."""
from system.financesim_cpp import CareerJob

SCENARIO_NAME = "Senior Engineer"


def create_models():
    """Return list of models for this scenario."""
    return [
        CareerJob("job1", "Senior Engineer", 150000),
    ]
