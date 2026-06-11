# Traceability

## Requirements to Test Cases

This matrix links each functional requirement from the SRS to the test cases that verify it.

| Requirement | Description | Unit Test | Acceptance Test |
|---|---|---|---|
| FR-01 | Load CSV file from disk | TC1 | AT-01 |
| FR-02 | First row is column header | TC1, TC2, TC3 | AT-02 |
| FR-03 | Remaining rows are data records | TC1 | AT-02 |
| FR-04 | `SELECT *` returns all columns and rows | TC1 | AT-02 |
| FR-05 | `SELECT col1, col2` returns only specified columns | TC2, TC3 | AT-03 |
| FR-06 | All rows returned unless filtered | TC1, TC2, TC3 | AT-02, AT-03 |
| FR-07 | Parser produces structured query object | TC1–TC7 | AT-02, AT-03 |
| FR-08 | SQL keywords are case-insensitive | — | — |
| FR-09 | Parser throws error for malformed queries | TC6, TC7 | AT-04, AT-05 |
| FR-10 | Interactive REPL for entering queries | — | AT-01 |
| FR-11 | Results printed in formatted bordered table | — | AT-02, AT-03 |
| FR-12 | REPL exits cleanly on `exit` | — | AT-06 |
| FR-13 | REPL shows error without crashing | TC4, TC5, TC6, TC7 | AT-04, AT-05 |

## Jira Tickets to Components

| Ticket | Branch | Component |
|---|---|---|
| FM11402-10 | feature/FM11402-10-Repo-preparation | README, repo setup |
| FM11402-13 | FM11402-13-Code-implementation-for-SELECT | `src/main.cpp` |
| FM11402-14 | FM11402-14-Create-test-cases-for-SELECT | `test/test_select.cpp`, `.github/workflows/ci.yml`, `docs/` |
