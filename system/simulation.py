"""
Simulation runner and experiment management.

Provides high-level API for configuring and running simulations.
Handles model lifecycle (initialize, run, finalize) and ensures
deterministic, replayable execution.
"""

import importlib
from typing import List, Optional
from .scheduler import Scheduler
from . import financesim_cpp as cpp


class Simulation:
    """
    High-level simulation runner.

    Manages model lifecycle and provides a simple API for running experiments.
    Ensures deterministic execution for replay capability.
    """

    def __init__(self):
        self._scheduler = Scheduler()
        self._event_bus = cpp.EventBus()
        self._models: List = []
        self._initialized: bool = False

    @property
    def scheduler(self) -> Scheduler:
        """Access the underlying scheduler."""
        return self._scheduler

    @property
    def current_time(self) -> float:
        """Current simulation time in days."""
        return self._scheduler.current_time

    @property
    def event_bus(self):
        """Access the event bus for subscribing to events."""
        return self._event_bus

    def add_model(self, model) -> None:
        """
        Add a model to the simulation.

        Args:
            model: A Model instance to add.

        Raises:
            RuntimeError: If simulation is already initialized.
        """
        if self._initialized:
            raise RuntimeError("Cannot add models after initialization")
        self._models.append(model)

    @property
    def scenario_name(self) -> str:
        """Name of the loaded scenario, if any."""
        return getattr(self, '_scenario_name', 'unnamed')

    @classmethod
    def from_scenario(cls, scenario_name: str) -> "Simulation":
        """
        Create a simulation from a scenario module.

        Args:
            scenario_name: Name of the scenario module (without .py)

        Returns:
            Configured Simulation instance with models loaded.
        """
        module = importlib.import_module(f"scenarios.{scenario_name}")
        sim = cls()
        sim._scenario_name = getattr(module, 'SCENARIO_NAME', scenario_name)
        for model in module.create_models():
            sim.add_model(model)
        return sim

    def initialize(self) -> None:
        """
        Initialize all models and prepare for execution.

        Must be called before run(). Calls initialize() on all models.
        """
        if self._initialized:
            raise RuntimeError("Simulation already initialized")

        for model in self._models:
            model.initialize(self._event_bus)
            self._scheduler.register_model(model)

        self._initialized = True

    def run(self, duration: float) -> None:
        """
        Run the simulation for the specified duration.

        Args:
            duration: Time to run in days.

        Raises:
            RuntimeError: If simulation not initialized.
        """
        if not self._initialized:
            raise RuntimeError("Simulation not initialized. Call initialize() first.")

        end_time = self._scheduler.current_time + duration
        self._scheduler.run_until(end_time)

    def run_until(self, end_time: float) -> None:
        """
        Run the simulation until the specified absolute time.

        Args:
            end_time: Absolute time to run until in days.

        Raises:
            RuntimeError: If simulation not initialized.
        """
        if not self._initialized:
            raise RuntimeError("Simulation not initialized. Call initialize() first.")

        self._scheduler.run_until(end_time)

    def finalize(self) -> None:
        """
        Finalize all models and clean up.

        Calls finalize() on all models. Should be called when simulation is complete.
        """
        if not self._initialized:
            return

        for model in self._models:
            model.finalize()

        self._initialized = False

    def reset(self) -> None:
        """
        Reset the simulation to initial state for replay.

        Resets all models and the scheduler.
        """
        self._scheduler.reset()

        for model in self._models:
            model.reset()

        if self._event_bus:
            self._event_bus.reset()

        self._initialized = False


def compare_scenarios(scenario_names: List[str], duration: float) -> dict:
    """
    Run multiple scenarios and collect results for comparison.

    Args:
        scenario_names: List of scenario module names to run.
        duration: How long to run each scenario (in days).

    Returns:
        Dict mapping scenario name to results dict.
    """
    results = {}
    for name in scenario_names:
        sim = Simulation.from_scenario(name)
        sim.initialize()
        sim.run(duration)
        results[sim.scenario_name] = {
            'final_time': sim.current_time,
            'model_count': len(sim._models),
        }
        sim.finalize()
    return results
