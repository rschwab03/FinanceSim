"""
Simulation runner and experiment management.

Provides high-level API for configuring and running simulations.
Handles model lifecycle (initialize, run, finalize) and ensures
deterministic, replayable execution.
"""

from typing import List, Optional
from .scheduler import Scheduler

# Will be available after building the C++ module
# from . import financesim_cpp as cpp


class Simulation:
    """
    High-level simulation runner.

    Manages model lifecycle and provides a simple API for running experiments.
    Ensures deterministic execution for replay capability.
    """

    def __init__(self):
        self._scheduler = Scheduler()
        self._event_bus = None  # cpp.EventBus() after build
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

    def initialize(self) -> None:
        """
        Initialize all models and prepare for execution.

        Must be called before run(). Calls initialize() on all models.
        """
        if self._initialized:
            raise RuntimeError("Simulation already initialized")

        # TODO: Create event bus from C++ module
        # self._event_bus = cpp.EventBus()

        for model in self._models:
            if self._event_bus:
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
