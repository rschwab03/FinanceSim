#!/usr/bin/env python3
"""Run a financial simulation scenario from the command line."""

import argparse
import sys
from system.simulation import Simulation, compare_scenarios


def list_scenarios():
    """List available scenarios."""
    import os
    scenarios_dir = os.path.join(os.path.dirname(__file__), "scenarios")
    scenarios = []
    for f in os.listdir(scenarios_dir):
        if f.endswith(".py") and not f.startswith("_"):
            scenarios.append(f[:-3])
    return sorted(scenarios)


def run_single(scenario_name: str, duration: float, verbose: bool):
    """Run a single scenario."""
    sim = Simulation.from_scenario(scenario_name)
    print(f"Loaded scenario: {sim.scenario_name}")
    print(f"Models: {len(sim._models)}")

    sim.initialize()
    sim.run(duration)

    print(f"\nSimulation complete: {sim.current_time} days")

    if verbose:
        events = sim.event_bus.event_log()
        print(f"\nEvents ({len(events)} total):")
        for event in events[:20]:  # Show first 20
            print(f"  Day {event.timestamp():7.2f}: {event.type_name()}")
        if len(events) > 20:
            print(f"  ... and {len(events) - 20} more events")

    sim.finalize()


def run_comparison(scenario_names: list, duration: float):
    """Compare multiple scenarios."""
    print(f"Comparing scenarios: {', '.join(scenario_names)}")
    print(f"Duration: {duration} days\n")

    results = compare_scenarios(scenario_names, duration)

    for name, data in results.items():
        print(f"{name}:")
        for key, value in data.items():
            print(f"  {key}: {value}")
        print()


def main():
    parser = argparse.ArgumentParser(
        description="Run FinanceSim scenarios",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  %(prog)s early_career                    # Run for default 365 days
  %(prog)s early_career -d 1825            # Run for 5 years
  %(prog)s early_career senior_engineer    # Compare two scenarios
  %(prog)s --list                          # Show available scenarios
        """
    )
    parser.add_argument(
        "scenarios",
        nargs="*",
        help="Scenario name(s) to run"
    )
    parser.add_argument(
        "-d", "--duration",
        type=float,
        default=365,
        help="Duration in days (default: 365)"
    )
    parser.add_argument(
        "-v", "--verbose",
        action="store_true",
        help="Show event log"
    )
    parser.add_argument(
        "--list",
        action="store_true",
        help="List available scenarios"
    )

    args = parser.parse_args()

    if args.list:
        print("Available scenarios:")
        for s in list_scenarios():
            print(f"  {s}")
        return 0

    if not args.scenarios:
        parser.print_help()
        return 1

    try:
        if len(args.scenarios) == 1:
            run_single(args.scenarios[0], args.duration, args.verbose)
        else:
            run_comparison(args.scenarios, args.duration)
        return 0
    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    sys.exit(main())
