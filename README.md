# Project B - CSV Mini Database and Query Engine

## Overview

Project B implements a lightweight database engine that uses CSV files as persistent storage. The system provides a SQL-like query interface for creating, reading, updating, and deleting records stored in plain CSV files — no external database server required.

## Objectives

- Build a file-based data storage layer backed by CSV files
- Implement a query parser that accepts a subset of SQL syntax
- Support core database operations: `SELECT`, `INSERT`, `UPDATE`, `DELETE`
- Handle basic filtering (`WHERE`), column projection, and result ordering (`ORDER BY`)
- Provide a command-line REPL for interactive querying

## Features

| Feature | Description |
|---|---|
| CSV Storage | Each table maps to a `.csv` file on disk; the header row defines the schema |
| Query Parser | Parses SQL-like statements into an abstract syntax tree (AST) |
| Query Engine | Executes the AST against the in-memory table representation |
| WHERE Filtering | Supports `=`, `!=`, `<`, `>`, `<=`, `>=` comparisons and `AND`/`OR` logic |
| ORDER BY | Ascending and descending sort on any column |
| LIMIT / OFFSET | Result pagination |
| REPL | Interactive shell with command history and formatted table output |

## Supported SQL Subset

```sql
-- Create a table (defines the CSV header)
CREATE TABLE students (id, name, grade, score);

-- Insert a row
INSERT INTO students VALUES (1, 'Alice', 'A', 95);

-- Query with filtering and ordering
SELECT name, score FROM students WHERE grade = 'A' ORDER BY score DESC LIMIT 10;

-- Update records
UPDATE students SET score = 100 WHERE name = 'Alice';

-- Delete records
DELETE FROM students WHERE score < 60;

-- Drop a table
DROP TABLE students;
```

## Project Structure

```
ProjectB/
├── src/
│   ├── parser/       # Tokenizer and SQL parser → AST
│   ├── engine/       # Query executor, table scan, filter, sort
│   ├── storage/      # CSV read/write layer
│   └── repl/         # Interactive shell
├── data/             # CSV data files (auto-created at runtime)
├── tests/            # Unit and integration tests
└── README.md
```

## Getting Started

```bash
# Install dependencies
pip install -r requirements.txt   # or: npm install

# Launch the interactive REPL
python main.py

# Run the test suite
pytest tests/
```
