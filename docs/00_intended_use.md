# Intended Use

## Purpose

This project is a lightweight CSV-based mini database and query engine built in C++. It allows users to query structured data stored in plain `.csv` files using a SQL-like syntax — without requiring a database server.

## Target Users

- Students and developers learning how database query engines work internally
- Anyone who needs a simple, portable tool to query CSV data from the terminal

## Use Cases

- Load a CSV file and run `SELECT` queries interactively via the REPL
- Filter and project columns from tabular data without installing a full database
- Serve as a learning platform for extending SQL features (WHERE, INSERT, UPDATE, DELETE)

## Scope

The engine currently supports the `SELECT` statement with full column projection (`SELECT *` and named columns). It is not intended to replace production databases and does not support concurrent access, transactions, or large-scale datasets.

## Course Context

**Course:** 11402 CS351 — AI-Assisted Software Development
**Project:** B (CSV Mini Database and Query Engine)
**Institution:** Yuan Ze University (YZU) Exchange Program, Spring 2026
