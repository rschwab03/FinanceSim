"""Tests for the scheduler."""

import pytest
import sys
import os

# Add system directory to path for imports
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..', 'system'))

from scheduler import Scheduler


class MockSchedule:
    """Mock schedule for testing."""
    def __init__(self, start_time=0.0, rate=1.0, stop_time=-1.0):
        self.start_time = start_time
        self.rate = rate
        self.stop_time = stop_time


class MockModel:
    """Mock model for testing the scheduler."""
    def __init__(self, model_id: str, schedule: MockSchedule):
        self._id = model_id
        self._schedule = schedule
        self.update_times: list = []

    def id(self) -> str:
        return self._id

    def schedule(self) -> MockSchedule:
        return self._schedule

    def update(self, time: float) -> None:
        self.update_times.append(time)


class TestScheduler:
    """Test cases for the Scheduler class."""

    def test_register_model(self):
        """Test registering a model with the scheduler."""
        scheduler = Scheduler()
        model = MockModel("test", MockSchedule())

        scheduler.register_model(model)

        assert "test" in scheduler.models

    def test_duplicate_model_id_raises(self):
        """Test that registering duplicate model ID raises error."""
        scheduler = Scheduler()
        model1 = MockModel("test", MockSchedule())
        model2 = MockModel("test", MockSchedule())

        scheduler.register_model(model1)

        with pytest.raises(ValueError):
            scheduler.register_model(model2)

    def test_run_until_executes_model(self):
        """Test that run_until executes models at scheduled times."""
        scheduler = Scheduler()
        model = MockModel("test", MockSchedule(start_time=0.0, rate=10.0))

        scheduler.register_model(model)
        scheduler.run_until(25.0)

        # Should execute at 0, 10, 20
        assert model.update_times == [0.0, 10.0, 20.0]

    def test_multiple_models_interleaved(self):
        """Test that multiple models execute in correct order."""
        scheduler = Scheduler()
        model_a = MockModel("a", MockSchedule(start_time=0.0, rate=10.0))
        model_b = MockModel("b", MockSchedule(start_time=5.0, rate=10.0))

        scheduler.register_model(model_a)
        scheduler.register_model(model_b)
        scheduler.run_until(20.0)

        # A: 0, 10, 20  B: 5, 15
        assert model_a.update_times == [0.0, 10.0, 20.0]
        assert model_b.update_times == [5.0, 15.0]

    def test_model_stop_time(self):
        """Test that models stop at their stop_time."""
        scheduler = Scheduler()
        model = MockModel("test", MockSchedule(start_time=0.0, rate=10.0, stop_time=15.0))

        scheduler.register_model(model)
        scheduler.run_until(50.0)

        # Should only execute at 0, 10 (not 20)
        assert model.update_times == [0.0, 10.0]

    def test_reset(self):
        """Test that reset clears state and allows re-run."""
        scheduler = Scheduler()
        model = MockModel("test", MockSchedule(start_time=0.0, rate=10.0))

        scheduler.register_model(model)
        scheduler.run_until(15.0)

        assert model.update_times == [0.0, 10.0]

        model.update_times.clear()
        scheduler.reset()
        scheduler.run_until(15.0)

        assert model.update_times == [0.0, 10.0]


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
