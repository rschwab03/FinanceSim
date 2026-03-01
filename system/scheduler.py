"""
Event-driven scheduler for financial simulation.

Time advances based on individual model update rates.
The scheduler maintains a priority queue of scheduled executions
and advances simulation time to the next scheduled event.
"""

import heapq
from dataclasses import dataclass, field
from typing import List, Optional

# Will be available after building the C++ module
# from . import financesim_cpp as cpp


@dataclass(order=True)
class ScheduledExecution:
    """Represents a scheduled model execution."""
    time: float
    model_id: str = field(compare=False)
    model: object = field(compare=False)


class Scheduler:
    """
    Event-driven scheduler that advances time based on model schedules.

    Models are executed at their scheduled times. Time jumps from one
    scheduled execution to the next - no fixed time stepping.
    """

    def __init__(self):
        self._models: dict = {}  # model_id -> model
        self._event_queue: List[ScheduledExecution] = []
        self._current_time: float = 0.0
        self._running: bool = False

    @property
    def current_time(self) -> float:
        """Current simulation time in days."""
        return self._current_time

    @property
    def models(self) -> dict:
        """Registered models by ID."""
        return self._models.copy()

    def register_model(self, model) -> None:
        """
        Register a model with the scheduler.

        Args:
            model: A Model instance with id(), schedule(), and update() methods.
        """
        model_id = model.id()
        if model_id in self._models:
            raise ValueError(f"Model with id '{model_id}' already registered")

        self._models[model_id] = model
        self._schedule_next_execution(model)

    def unregister_model(self, model_id: str) -> None:
        """
        Unregister a model from the scheduler.

        Args:
            model_id: The ID of the model to remove.
        """
        if model_id in self._models:
            del self._models[model_id]
            # Note: stale entries in event_queue are filtered during run

    def schedule_at(self, model, time: float) -> None:
        """
        Schedule a model to execute at a specific time.

        This is used for event-triggered scheduling, where a model needs
        to be called at a future time in response to an event.

        Args:
            model: The model to schedule.
            time: The absolute time to execute at.
        """
        if time <= self._current_time:
            return  # Can't schedule in the past
        if model.id() not in self._models:
            return  # Model not registered

        heapq.heappush(
            self._event_queue,
            ScheduledExecution(time, model.id(), model)
        )

    def _schedule_next_execution(self, model) -> None:
        """Calculate and schedule the next periodic execution for a model."""
        schedule = model.schedule()

        # Skip models with no periodic schedule (rate <= 0)
        if schedule.rate <= 0:
            return

        # Check if already past stop time
        if schedule.stop_time >= 0 and self._current_time >= schedule.stop_time:
            return  # Model has finished

        # Determine next execution time
        if self._current_time < schedule.start_time:
            next_time = schedule.start_time
        else:
            # Calculate next period
            elapsed = self._current_time - schedule.start_time
            periods = elapsed / schedule.rate
            next_time = schedule.start_time + (int(periods) + 1) * schedule.rate

        # Check if within bounds
        if schedule.stop_time >= 0 and next_time > schedule.stop_time:
            return  # Model has finished

        heapq.heappush(
            self._event_queue,
            ScheduledExecution(next_time, model.id(), model)
        )

    def run_until(self, end_time: float) -> None:
        """
        Run the simulation until the specified time.

        Args:
            end_time: Stop when simulation time reaches this value (days).
        """
        self._running = True

        while self._running and self._event_queue:
            # Peek at next execution
            next_exec = self._event_queue[0]

            # Stop if we've reached end time
            if next_exec.time > end_time:
                self._current_time = end_time
                break

            # Pop the execution
            heapq.heappop(self._event_queue)

            # Skip if model was unregistered
            if next_exec.model_id not in self._models:
                continue

            # Advance time and execute
            self._current_time = next_exec.time
            next_exec.model.update(self._current_time)

            # Schedule next execution for this model
            self._schedule_next_execution(next_exec.model)

        self._running = False

    def stop(self) -> None:
        """Stop the scheduler during a run."""
        self._running = False

    def reset(self) -> None:
        """Reset scheduler to initial state."""
        self._event_queue.clear()
        self._current_time = 0.0
        self._running = False

        # Re-schedule all models
        for model in self._models.values():
            self._schedule_next_execution(model)
