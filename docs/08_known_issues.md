# Known Issues

## Current Limitations

| ID | Issue | Severity | Notes |
|---|---|---|---|
| KI-01 | CSV parser does not support quoted fields containing commas | Medium | e.g. `"Smith, John"` would be split incorrectly |
| KI-02 | No support for `WHERE` filtering | Medium | Planned for a future milestone |
| KI-03 | No support for `ORDER BY` or `LIMIT` | Low | Planned for a future milestone |
| KI-04 | All CSV tables must be manually registered in `main.cpp` | Low | No auto-discovery of CSV files in the `data/` folder |
| KI-05 | Column names are case-sensitive | Low | `SELECT Customer_Name` would fail if the header is `customer_name` |
| KI-06 | No support for `INSERT`, `UPDATE`, or `DELETE` | Medium | Read-only engine at this stage |
| KI-07 | Large CSV files (100k+ rows) may be slow | Low | No indexing or lazy loading implemented |

## Resolved Issues

None yet.

## Reporting Issues

Please open a GitHub Issue in the repository or create a new Jira ticket under the FM11402 project.
