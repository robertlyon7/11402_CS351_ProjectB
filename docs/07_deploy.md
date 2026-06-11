# Deployment

## Requirements

| Tool | Version | Purpose |
|---|---|---|
| g++ (MinGW or GCC) | 6.3+ | C++ compiler |
| C++17 standard | — | Language standard |
| Google Test | latest | Required for running tests only |

## Build and Run (Local)

**1. Compile the engine:**
```bash
g++ -std=c++17 -o engine src/main.cpp
```

**2. Run the REPL:**
```bash
./engine
```

Make sure `data/sales_data.csv` exists in the working directory before running.

## Build and Run Tests (Local)

Google Test must be installed first:
```bash
# Ubuntu / Debian
sudo apt-get install libgtest-dev cmake
cd /usr/src/gtest && sudo cmake . && sudo make
sudo cp lib/*.a /usr/lib

# Compile and run tests
g++ -std=c++17 -o test_runner test/test_select.cpp -lgtest -lgtest_main -pthread
./test_runner
```

## CI/CD (GitHub Actions)

Tests run automatically on every push and pull request via `.github/workflows/ci.yml`.

**Pipeline steps:**
1. Checkout repository
2. Install Google Test on the Ubuntu runner
3. Compile `test/test_select.cpp`
4. Run all test cases

Results are visible under the **Actions** tab in the GitHub repository.

## Adding a New CSV Table

To load an additional table, add it to the pre-load block in `src/main.cpp`:
```cpp
db["your_table"] = loadCSV("data/your_table.csv", "your_table");
```
