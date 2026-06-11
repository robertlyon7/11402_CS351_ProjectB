# Plan

## Project Overview

Build a CSV-based mini database engine in C++ that supports SQL-like queries through an interactive REPL, backed by automated testing via GitHub Actions CI/CD.

## Milestones

| Milestone | Description | Status |
|---|---|---|
| FM11402-10 | Repository preparation and README | Done |
| FM11402-13 | Code implementation for SELECT | Done |
| FM11402-14 | Test cases for SELECT | In Progress |

## Task Breakdown

### FM11402-10 — Repo Preparation
- Initialize GitHub repository
- Write project README with overview, architecture, and usage

### FM11402-13 — SELECT Implementation
- Implement CSV loader (`loadCSV`)
- Implement query parser (`parseSelect`)
- Implement query executor (`executeSelect`)
- Implement interactive REPL loop

### FM11402-14 — Test Cases
- Write 7 unit tests using Google Test
- Set up GitHub Actions CI/CD pipeline
- Create `docs/` folder with project documentation

## Branch Strategy

Each Jira ticket maps to its own branch. Branches are merged into `main` via pull requests.

## Tech Stack

| Component | Technology |
|---|---|
| Language | C++ (C++17) |
| Testing | Google Test (gtest) |
| CI/CD | GitHub Actions |
| Data Format | CSV |
| Version Control | Git / GitHub |
