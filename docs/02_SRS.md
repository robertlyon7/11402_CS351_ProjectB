# Software Requirements Specification (SRS)

## 1. Introduction

This document defines the functional and non-functional requirements for the CSV Mini Database and Query Engine (Project B).

## 2. Functional Requirements

### 2.1 CSV Storage
- **FR-01:** The system shall load a `.csv` file from disk into an in-memory table structure.
- **FR-02:** The first row of the CSV file shall be treated as the column header.
- **FR-03:** Each subsequent row shall be treated as a data record.

### 2.2 SELECT Query
- **FR-04:** The system shall support `SELECT * FROM <table>` to return all columns and all rows.
- **FR-05:** The system shall support `SELECT <col1>, <col2> FROM <table>` to return only specified columns.
- **FR-06:** The system shall return all rows in the table unless otherwise filtered.

### 2.3 Query Parser
- **FR-07:** The parser shall accept a SQL-like string as input and produce a structured query object.
- **FR-08:** The parser shall be case-insensitive for SQL keywords (`SELECT`, `FROM`).
- **FR-09:** The parser shall throw a descriptive error for malformed queries.

### 2.4 REPL
- **FR-10:** The system shall provide an interactive command-line loop for entering queries.
- **FR-11:** The REPL shall print results in a formatted bordered table.
- **FR-12:** The REPL shall exit cleanly when the user types `exit`.
- **FR-13:** The REPL shall display a meaningful error message for invalid queries without crashing.

## 3. Non-Functional Requirements

- **NFR-01:** The system shall be implemented in C++ (C++17 standard).
- **NFR-02:** The system shall compile and run on Linux (Ubuntu) and Windows (MinGW).
- **NFR-03:** Query results shall be returned within 1 second for CSV files up to 10,000 rows.
- **NFR-04:** All test cases shall pass automatically via GitHub Actions on every push.

## 4. Constraints

- No external database libraries (e.g., SQLite, MySQL) may be used.
- The CSV parser assumes comma-delimited files with no quoted fields containing commas.
