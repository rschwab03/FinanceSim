/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "FinanceSim", "index.html", [
    [ "Overview", "index.html#autotoc_md69", null ],
    [ "Architecture", "index.html#autotoc_md70", [
      [ "EventBus", "index.html#autotoc_md71", null ],
      [ "Models", "index.html#autotoc_md72", null ],
      [ "Events", "index.html#autotoc_md73", null ]
    ] ],
    [ "Table of Contents", "index.html#autotoc_md74", [
      [ "Getting Started", "index.html#autotoc_md75", null ],
      [ "Core Framework", "index.html#autotoc_md76", null ],
      [ "Financial Models", "index.html#autotoc_md77", [
        [ "Income Models", "index.html#autotoc_md78", null ],
        [ "Expense Models", "index.html#autotoc_md79", null ],
        [ "Asset Models", "index.html#autotoc_md80", null ],
        [ "Liability Models", "index.html#autotoc_md81", null ]
      ] ],
      [ "Events Reference", "index.html#autotoc_md82", null ]
    ] ],
    [ "Quick Start", "index.html#autotoc_md83", null ],
    [ "Python Bindings", "index.html#autotoc_md84", null ],
    [ "Building", "index.html#autotoc_md85", null ],
    [ "Building and Running", "building_and_running.html", [
      [ "Building and Running FinanceSim", "building_and_running.html#autotoc_md41", [
        [ "System Requirements", "building_and_running.html#autotoc_md42", [
          [ "Required", "building_and_running.html#autotoc_md43", null ],
          [ "Optional", "building_and_running.html#autotoc_md44", null ],
          [ "Installing Dependencies", "building_and_running.html#autotoc_md45", null ]
        ] ],
        [ "Building the Simulation", "building_and_running.html#autotoc_md46", [
          [ "Quick Build", "building_and_running.html#autotoc_md47", null ],
          [ "Manual Build", "building_and_running.html#autotoc_md48", null ],
          [ "Build Options", "building_and_running.html#autotoc_md49", null ],
          [ "Build Output", "building_and_running.html#autotoc_md50", null ]
        ] ],
        [ "Building Documentation", "building_and_running.html#autotoc_md51", null ],
        [ "Running Simulations", "building_and_running.html#autotoc_md52", [
          [ "Python API", "building_and_running.html#autotoc_md53", null ],
          [ "Using Scenarios", "building_and_running.html#autotoc_md54", null ],
          [ "Creating Custom Scenarios", "building_and_running.html#autotoc_md55", null ]
        ] ],
        [ "Troubleshooting", "building_and_running.html#autotoc_md56", [
          [ "Import Errors", "building_and_running.html#autotoc_md57", null ],
          [ "CMake Can't Find pybind11", "building_and_running.html#autotoc_md58", null ],
          [ "Segmentation Faults", "building_and_running.html#autotoc_md59", null ]
        ] ]
      ] ]
    ] ],
    [ "Adding a New Model", "adding_a_model.html", [
      [ "Adding a New Model to FinanceSim", "adding_a_model.html#autotoc_md0", [
        [ "Prerequisites", "adding_a_model.html#autotoc_md1", null ],
        [ "Understanding the Architecture", "adding_a_model.html#autotoc_md2", [
          [ "The Event-Driven Design", "adding_a_model.html#autotoc_md3", null ],
          [ "The Model Lifecycle", "adding_a_model.html#autotoc_md4", null ]
        ] ],
        [ "Deciding What Kind of Model to Create", "adding_a_model.html#autotoc_md5", null ],
        [ "Step-by-Step: Creating an Income Model", "adding_a_model.html#autotoc_md6", [
          [ "Step 1: Create the Header File", "adding_a_model.html#autotoc_md7", [
            [ "A Note on Getters and Naming Conventions", "adding_a_model.html#autotoc_md8", null ]
          ] ],
          [ "Step 2: Create the Implementation File", "adding_a_model.html#autotoc_md9", null ],
          [ "Step 3: Add to CMakeLists.txt", "adding_a_model.html#autotoc_md10", null ],
          [ "Step 4: Add Python Bindings", "adding_a_model.html#autotoc_md11", [
            [ "Step 4a: Add the Include", "adding_a_model.html#autotoc_md12", null ],
            [ "Step 4b: Add the Class Binding", "adding_a_model.html#autotoc_md13", null ],
            [ "Understanding the Binding Code", "adding_a_model.html#autotoc_md14", null ],
            [ "The Complete Binding Explained Visually", "adding_a_model.html#autotoc_md15", null ],
            [ "Why Do We Need Bindings?", "adding_a_model.html#autotoc_md16", null ]
          ] ],
          [ "Step 5: Build and Test", "adding_a_model.html#autotoc_md17", null ]
        ] ],
        [ "Understanding the Schedule", "adding_a_model.html#autotoc_md18", null ],
        [ "Creating an Event-Driven Model", "adding_a_model.html#autotoc_md19", [
          [ "Subscribing to Events", "adding_a_model.html#autotoc_md20", null ],
          [ "Unsubscribing in reset()", "adding_a_model.html#autotoc_md21", null ],
          [ "Full Example: Event-Driven Model", "adding_a_model.html#autotoc_md22", null ]
        ] ],
        [ "Publishing Events", "adding_a_model.html#autotoc_md23", [
          [ "Using the emit() Helper", "adding_a_model.html#autotoc_md24", null ],
          [ "Publishing Directly", "adding_a_model.html#autotoc_md25", null ]
        ] ],
        [ "Creating a New Event Type", "adding_a_model.html#autotoc_md26", [
          [ "Define the Event Class", "adding_a_model.html#autotoc_md27", null ],
          [ "Add Python Bindings for the Event", "adding_a_model.html#autotoc_md28", null ]
        ] ],
        [ "Creating a Scenario", "adding_a_model.html#autotoc_md29", null ],
        [ "Checklist for Adding a Model", "adding_a_model.html#autotoc_md30", null ],
        [ "Common Mistakes", "adding_a_model.html#autotoc_md31", [
          [ "Forgetting to call base class methods", "adding_a_model.html#autotoc_md32", null ],
          [ "Not saving the EventBus pointer", "adding_a_model.html#autotoc_md33", null ],
          [ "Forgetting to unsubscribe", "adding_a_model.html#autotoc_md34", null ],
          [ "Using the wrong schedule rate", "adding_a_model.html#autotoc_md35", null ]
        ] ],
        [ "Next Steps", "adding_a_model.html#autotoc_md36", null ],
        [ "Summary", "adding_a_model.html#autotoc_md37", null ]
      ] ],
      [ "Building and Running", "building_and_running.html", [
        [ "Building and Running FinanceSim", "building_and_running.html#autotoc_md41", [
          [ "System Requirements", "building_and_running.html#autotoc_md42", [
            [ "Required", "building_and_running.html#autotoc_md43", null ],
            [ "Optional", "building_and_running.html#autotoc_md44", null ],
            [ "Installing Dependencies", "building_and_running.html#autotoc_md45", null ]
          ] ],
          [ "Building the Simulation", "building_and_running.html#autotoc_md46", [
            [ "Quick Build", "building_and_running.html#autotoc_md47", null ],
            [ "Manual Build", "building_and_running.html#autotoc_md48", null ],
            [ "Build Options", "building_and_running.html#autotoc_md49", null ],
            [ "Build Output", "building_and_running.html#autotoc_md50", null ]
          ] ],
          [ "Building Documentation", "building_and_running.html#autotoc_md51", null ],
          [ "Running Simulations", "building_and_running.html#autotoc_md52", [
            [ "Python API", "building_and_running.html#autotoc_md53", null ],
            [ "Using Scenarios", "building_and_running.html#autotoc_md54", null ],
            [ "Creating Custom Scenarios", "building_and_running.html#autotoc_md55", null ]
          ] ],
          [ "Troubleshooting", "building_and_running.html#autotoc_md56", [
            [ "Import Errors", "building_and_running.html#autotoc_md57", null ],
            [ "CMake Can't Find pybind11", "building_and_running.html#autotoc_md58", null ],
            [ "Segmentation Faults", "building_and_running.html#autotoc_md59", null ]
          ] ]
        ] ]
      ] ]
    ] ],
    [ "How Scheduling Works", "scheduling.html", [
      [ "How Scheduling Works", "scheduling.html#autotoc_md86", [
        [ "The Big Picture", "scheduling.html#autotoc_md87", null ],
        [ "Key Concepts", "scheduling.html#autotoc_md88", [
          [ "Simulation Time", "scheduling.html#autotoc_md89", null ],
          [ "The Schedule Struct", "scheduling.html#autotoc_md90", null ],
          [ "Example Schedules", "scheduling.html#autotoc_md91", null ]
        ] ],
        [ "How the Scheduler Works", "scheduling.html#autotoc_md92", [
          [ "Visual Example", "scheduling.html#autotoc_md93", null ]
        ] ],
        [ "Event-Driven vs Scheduled Models", "scheduling.html#autotoc_md94", [
          [ "Scheduled Models (rate > 0)", "scheduling.html#autotoc_md95", null ],
          [ "Event-Driven Models (rate = 0 or rate <= 0)", "scheduling.html#autotoc_md96", [
            [ "How Event-Driven Updates Work", "scheduling.html#autotoc_md97", null ],
            [ "Setting Up Event Subscriptions", "scheduling.html#autotoc_md98", null ],
            [ "Handling Events", "scheduling.html#autotoc_md99", null ],
            [ "Cleaning Up Subscriptions", "scheduling.html#autotoc_md100", null ]
          ] ],
          [ "Hybrid Models: Both Scheduled AND Event-Driven", "scheduling.html#autotoc_md101", null ],
          [ "When to Use Each Approach", "scheduling.html#autotoc_md102", null ]
        ] ],
        [ "Configuring Schedules in Your Models", "scheduling.html#autotoc_md103", [
          [ "In the Constructor", "scheduling.html#autotoc_md104", null ],
          [ "Accepting a Custom Schedule", "scheduling.html#autotoc_md105", null ]
        ] ],
        [ "ExecutionTiming", "scheduling.html#autotoc_md106", null ],
        [ "Common Patterns", "scheduling.html#autotoc_md107", [
          [ "\"Run Once at Startup\"", "scheduling.html#autotoc_md108", null ],
          [ "\"Wait for Event, Then Start\"", "scheduling.html#autotoc_md109", [
            [ "Approach 1: Use <tt>schedule_at()</tt> for One-Time Triggers", "scheduling.html#autotoc_md110", null ],
            [ "Approach 2: Start with Event-Driven, Then Switch to Scheduled", "scheduling.html#autotoc_md111", null ],
            [ "Approach 3: Purely Event-Driven with Internal Scheduling", "scheduling.html#autotoc_md112", null ],
            [ "Approach 4: Use <tt>start_time</tt> for Known Activation Times", "scheduling.html#autotoc_md113", null ],
            [ "Choosing the Right Approach", "scheduling.html#autotoc_md114", null ],
            [ "Example: Loan That Starts After Funding", "scheduling.html#autotoc_md115", null ]
          ] ],
          [ "\"Run During Specific Period\"", "scheduling.html#autotoc_md116", null ]
        ] ],
        [ "Summary", "scheduling.html#autotoc_md117", null ]
      ] ]
    ] ],
    [ "Income Model Descriptions", "income_models.html", [
      [ "Income Models", "income_models.html#autotoc_md63", [
        [ "Base Class", "income_models.html#autotoc_md64", null ],
        [ "Available Models", "income_models.html#autotoc_md65", null ]
      ] ]
    ] ],
    [ "Expense Model Descriptions", "expense_models.html", [
      [ "Expense Models", "expense_models.html#autotoc_md60", [
        [ "Base Class", "expense_models.html#autotoc_md61", null ],
        [ "Available Models", "expense_models.html#autotoc_md62", null ]
      ] ]
    ] ],
    [ "Asset Model Descriptions", "asset_models.html", [
      [ "Asset Models", "asset_models.html#autotoc_md38", [
        [ "Base Class", "asset_models.html#autotoc_md39", null ],
        [ "Available Models", "asset_models.html#autotoc_md40", null ]
      ] ]
    ] ],
    [ "Liability Model Descriptions", "liability_models.html", [
      [ "Liability Models", "liability_models.html#autotoc_md66", [
        [ "Base Class", "liability_models.html#autotoc_md67", null ],
        [ "Available Models", "liability_models.html#autotoc_md68", null ]
      ] ]
    ] ],
    [ "Namespaces", "namespaces.html", [
      [ "Namespace List", "namespaces.html", "namespaces_dup" ],
      [ "Namespace Members", "namespacemembers.html", [
        [ "All", "namespacemembers.html", null ],
        [ "Functions", "namespacemembers_func.html", null ],
        [ "Typedefs", "namespacemembers_type.html", null ],
        [ "Enumerations", "namespacemembers_enum.html", null ]
      ] ]
    ] ],
    [ "Classes", "annotated.html", [
      [ "Class List", "annotated.html", "annotated_dup" ],
      [ "Class Index", "classes.html", null ],
      [ "Class Hierarchy", "hierarchy.html", "hierarchy" ],
      [ "Class Members", "functions.html", [
        [ "All", "functions.html", "functions_dup" ],
        [ "Functions", "functions_func.html", null ],
        [ "Variables", "functions_vars.html", null ]
      ] ]
    ] ],
    [ "Files", "files.html", [
      [ "File List", "files.html", "files_dup" ],
      [ "File Members", "globals.html", [
        [ "All", "globals.html", null ],
        [ "Functions", "globals_func.html", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"account__base_8cpp.html",
"classfinancesim_1_1CheckingAccount.html",
"classfinancesim_1_1Logger.html#a26c283aae3622301a804886212034eda",
"namespaces.html"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';