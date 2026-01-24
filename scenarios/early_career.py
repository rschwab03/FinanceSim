"""Early career scenario - junior engineer, modest salary."""
from system.financesim_cpp import CareerJob

SCENARIO_NAME = "Early Career"


def create_models():
    """Return list of models for this scenario."""
    return [
        CareerJob("job1", "Junior Engineer", 75000),
    ]
