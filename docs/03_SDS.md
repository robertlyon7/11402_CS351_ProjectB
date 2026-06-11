# Software Design Specification (SDS)

## 1. Architecture Overview

```
┌─────────────┐     SQL string      ┌─────────────┐     AST      ┌─────────────┐
│    REPL     │ ─────────────────►  │    Query    │ ──────────►  │   Query     │
│             │                     │    Parser   │              │   Engine    │
└─────────────┘                     └─────────────┘              └──────┬──────┘
                                                                         │ read
                                                                  ┌──────▼──────┐
                                                                  │  CSV Store  │
                                                                  │ (*.csv files)│
                                                                  └─────────────┘
```

## 2. Components

### 2.1 CSV Store (`loadCSV`)

**Responsibility:** Read a `.csv` file from disk and return a `Table` struct.

**Data Structures:**
```cpp
struct Table {
    std::string name;
    std::vector<std::string> columns;
    std::vector<std::vector<std::string>> rows;
};
```

**Behavior:**
- Opens the file and reads line by line
- First line is parsed as column headers
- Remaining lines are parsed as rows

### 2.2 Query Parser (`parseSelect`)

**Responsibility:** Tokenize and parse a SQL-like string into a `SelectQuery` struct.

**Data Structures:**
```cpp
struct SelectQuery {
    bool selectAll;
    std::vector<std::string> columns;
    std::string tableName;
};
```

**Behavior:**
- Expects tokens in order: `SELECT`, column list, `FROM`, table name
- Sets `selectAll = true` if column list is `*`
- Throws `std::runtime_error` on malformed input

### 2.3 Query Executor (`executeSelect`)

**Responsibility:** Apply the parsed query against the in-memory table and return projected rows.

**Behavior:**
- Looks up the table by name in the database map
- Resolves column names to indices
- Projects each row to only the requested columns
- Throws `std::runtime_error` if column or table is not found

### 2.4 REPL (`main`)

**Responsibility:** Accept user input in a loop, invoke the parser and executor, and print results.

**Behavior:**
- Pre-loads CSV files into the in-memory database on startup
- Reads input line by line
- Formats output as a bordered table with dynamic column widths
- Exits on `exit` command

## 3. File Structure

```
ProjectB/
├── src/
│   └── main.cpp          # All components (CSV loader, parser, executor, REPL)
├── test/
│   └── test_select.cpp   # Google Test unit tests
├── data/
│   └── sales_data.csv    # Sample dataset
├── docs/                 # Project documentation
└── .github/
    └── workflows/
        └── ci.yml        # GitHub Actions CI pipeline
```

## 4. Error Handling

| Scenario | Error Message |
|---|---|
| File not found | `Cannot open file: <path>` |
| Missing SELECT keyword | `Expected SELECT` |
| Missing FROM keyword | `Expected FROM` |
| Missing table name | `Expected table name after FROM` |
| Column not in table | `Column not found: <col>` |
| Table not in database | `Table not found: <table>` |
