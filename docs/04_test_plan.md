# Test Plan

## 1. Objectives

Verify that the SELECT query engine correctly handles valid queries, column projection, and error conditions as defined in the SRS.

## 2. Test Scope

| In Scope | Out of Scope |
|---|---|
| SELECT * and column projection | WHERE filtering |
| Parser error handling | ORDER BY, LIMIT |
| Invalid column / table handling | INSERT, UPDATE, DELETE |
| Empty input handling | Multi-table joins |

## 3. Test Approach

- **Framework:** Google Test (gtest)
- **Test file:** `test/test_select.cpp`
- **Execution:** Automated via GitHub Actions on every push and pull request to `main`
- **Environment:** Ubuntu (GitHub Actions runner), g++ with C++17

## 4. Test Cases

| ID | Test Name | Input | Expected Result |
|---|---|---|---|
| TC1 | SelectAllReturnsAllColumnsAndRows | `SELECT * FROM sales_data` | 6 rows, 5 columns returned |
| TC2 | ColumnProjectionReturnsOnlySpecifiedColumns | `SELECT customer_name, total FROM sales_data` | 6 rows, 2 columns returned |
| TC3 | SingleColumnSelection | `SELECT product FROM sales_data` | 6 rows, 1 column returned |
| TC4 | InvalidColumnThrowsError | `SELECT nonexistent FROM sales_data` | throws `"Column not found: nonexistent"` |
| TC5 | InvalidTableThrowsError | `SELECT * FROM unknown_table` | throws `"Table not found: unknown_table"` |
| TC6 | MissingFromThrowsParseError | `SELECT customer_name sales_data` | throws `"Expected FROM"` |
| TC7 | EmptyInputIsHandledGracefully | *(empty string)* | throws, no crash |

## 5. Pass/Fail Criteria

- All 7 test cases must pass for the build to be considered successful
- Any test failure will cause the GitHub Actions pipeline to fail and block the pull request
