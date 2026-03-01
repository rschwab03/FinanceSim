"""Scenario with accounts and logging - tests income routing and interest accrual."""
from system.financesim_cpp import (
    CareerJob,
    CheckingAccount,
    SavingsAccount,
    Schedule,
    Logger,
    ConsoleWriter,
    JsonWriter,
    LogLevel,
)

SCENARIO_NAME = "With Accounts"


def create_models():
    """Return list of models for this scenario."""
    return [
        # Income source - salary goes to default account (checking)
        CareerJob("job1", "Software Engineer", 120000),
        # Checking account - default recipient for income (no scheduled updates)
        CheckingAccount("checking1", "Primary Checking", initial_balance=5000.0),
        # Savings account - earns 4.5% APY, interest calculated monthly
        SavingsAccount("savings1", "High-Yield Savings", apy=0.045, initial_balance=10000.0),
    ]


def create_logger(output_file="simulation.jsonl"):
    """Create and configure a logger for the simulation."""
    logger = Logger()
    logger.set_level(LogLevel.INFO)
    logger.add_writer(ConsoleWriter())
    logger.add_writer(JsonWriter(output_file))
    return logger


# Example: Customizing schedule for daily interest compounding
def create_daily_savings():
    """Create a savings account with daily interest compounding."""
    daily_schedule = Schedule()
    daily_schedule.rate = 1.0  # Update daily instead of monthly
    return SavingsAccount(
        "savings_daily", "Daily Compound Savings",
        apy=0.045,
        initial_balance=10000.0,
        schedule=daily_schedule
    )
