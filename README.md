# C++ Software Construction Projects
**Auburn University — COMP 2710 (Software Construction)**

A collection of five C++ projects completed during COMP 2710, covering core software engineering concepts including simulation, file I/O, data structures, multithreading, and version control with Git.

---

## Projects

| # | Project | Key Concepts |
|---|---------|-------------|
| 1 | [Credit Debt Calculator](#1-credit-debt-calculator) | Flow of control, amortization logic |
| 2 | [Truel of the Fates Simulator](#2-truel-of-the-fates-simulator) | Monte Carlo simulation, unit testing |
| 3 | [Data Analyzer](#3-data-analyzer) | File I/O, merge sort, CSV output, Git branching |
| 4 | [Quiz System](#4-quiz-system) | Linked lists, dynamic memory, debug/production modes |
| 5 | [Producer-Consumer Simulator](#5-producerconsumer-simulator) | POSIX threads, mutex, semaphores |

---

## 1. Credit Debt Calculator
**Language:** C++

Calculates how many months it takes to pay off a loan and the total interest paid. Takes loan amount, annual interest rate, and monthly payment as inputs and outputs a full amortization table.

**Features:**
- Input validation (payment must exceed monthly interest)
- Handles final partial payment correctly
- Formatted amortization table with month-by-month breakdown

**Compile & Run:**
```bash
g++ credit_debt_calculator.cpp -o calculator
./calculator
```

---

## 2. Truel of the Fates Simulator
**Language:** C++

Monte Carlo simulation of a three-way duel (truel) between marksmen of varying accuracy. Simulates 10,000 trials to determine win probabilities under two different strategies.

**Features:**
- Two shooting strategies compared across 10,000 simulations
- Demonstrates why the weakest shooter benefits from intentionally missing first
- Full unit test suite using `assert()` for all five core functions
- Two versions: without tests (`truel_simulator.cpp`) and with tests (`truel_simulator_with_tests.cpp`)

**Compile & Run:**
```bash
g++ truel_simulator_with_tests.cpp -o truel
./truel
```

---

## 3. Data Analyzer
**Language:** C++ (multi-file project)

Reads multiple data files, validates input, merges and sorts all values, then computes and outputs statistical summaries. Built with functional programming principles across multiple source files.

**Features:**
- Reads N input files from any valid path
- Detects and rejects malformed/corrupted input files
- Custom merge sort implementation — O(N log N), no STL sort used
- Computes mean, median, and mode (averaged if multiple modes)
- Exports sorted data and statistics to a CSV file
- Two development phases managed with Git branching and merging

**Compile & Run:**
```bash
make
./bin/main
```

**Files:**
- `DataAnalyzer.cpp` — main entry point
- `FileReader.cpp / .h` — file reading and validation
- `DataProcessor.cpp / .h` — sorting and statistics

---

## 4. Quiz System
**Language:** C++

A terminal-based quiz creation and assessment system built entirely on a singly linked list — no arrays or vectors used.

**Features:**
- Three question types: multiple choice, true/false, written response
- Full CRUD operations: create, edit, and delete questions dynamically
- Assessment phase: jump to any question, edit answers before submitting
- Conditional compilation (`#ifdef UNIT_TESTING`) to switch between debug and production modes
- Unit tests using `assert()` with at least 5 test cases

**Compile & Run (production):**
```bash
g++ phase2/project4_phase2.cpp -o quiz
./quiz
```

**Compile & Run (debug/test mode):**
```bash
g++ -DUNIT_TESTING phase2/project4_phase2.cpp -o quiz_debug
./quiz_debug
```

---

## 5. Producer-Consumer Simulator
**Language:** C++

Implements the classic producer-consumer concurrency problem using POSIX threads, mutex locks, and semaphores.

**Features:**
- Two threads: producer generates integers (7, 14, 21, ...), consumer reads and removes them
- Single-unit buffer with mutex-protected access
- Proper semaphore synchronization to prevent race conditions
- Compiled and tested on Linux with `-lpthread`

**Compile & Run:**
```bash
g++ project5_Cole_mzc0216.cpp -o producer_consumer -lpthread
./producer_consumer
```

---

## Author
**Michael Cole** — Computer Engineering, Auburn University  
[LinkedIn](https://www.linkedin.com/in/michael-cole-b948b0280/) | [GitHub](https://github.com/lapizliluzi)
