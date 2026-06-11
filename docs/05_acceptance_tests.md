# Acceptance Tests

## Overview

Acceptance tests verify that the system meets the functional requirements from the end user's perspective. These are manual tests performed by running the compiled REPL.

## Build Instructions

```bash
g++ -std=c++17 -o engine src/main.cpp
./engine
```

## Acceptance Test Cases

### AT-01: Launch and Table Load
**Steps:**
1. Compile and run the engine
2. Observe the startup message

**Expected Output:**
```
CSV Query Engine
Loaded tables: sales_data
Type a SELECT query or 'exit' to quit.
```
**Pass Criteria:** Engine starts without errors and lists `sales_data` as a loaded table.

---

### AT-02: SELECT All Columns
**Steps:**
1. At the `>>` prompt, type: `SELECT * FROM sales_data`

**Expected Output:** A bordered table with all 5 columns and 6 rows, followed by `6 row(s) returned.`

**Pass Criteria:** All rows and columns from `sales_data.csv` are displayed correctly.

---

### AT-03: Column Projection
**Steps:**
1. At the `>>` prompt, type: `SELECT customer_name, product FROM sales_data`

**Expected Output:** A bordered table with only `customer_name` and `product` columns, 6 rows.

**Pass Criteria:** Only the requested columns appear in the output.

---

### AT-04: Invalid Column Error
**Steps:**
1. At the `>>` prompt, type: `SELECT nonexistent FROM sales_data`

**Expected Output:**
```
Error: Column not found: nonexistent
```
**Pass Criteria:** Error is displayed and REPL continues without crashing.

---

### AT-05: Invalid Table Error
**Steps:**
1. At the `>>` prompt, type: `SELECT * FROM unknown`

**Expected Output:**
```
Error: Table not found: unknown
```
**Pass Criteria:** Error is displayed and REPL continues without crashing.

---

### AT-06: Exit Command
**Steps:**
1. At the `>>` prompt, type: `exit`

**Expected Output:** Program terminates cleanly.

**Pass Criteria:** No crash or hang on exit.
