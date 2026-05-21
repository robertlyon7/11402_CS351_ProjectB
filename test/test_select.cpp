#include <gtest/gtest.h>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <sstream>

// ── Inline the core logic so tests can access it without linking main() ───────

struct Table {
    std::string name;
    std::vector<std::string> columns;
    std::vector<std::vector<std::string>> rows;
};

std::vector<std::string> splitCSVLine(const std::string& line) {
    std::vector<std::string> fields;
    std::stringstream ss(line);
    std::string field;
    while (std::getline(ss, field, ',')) fields.push_back(field);
    return fields;
}

Table loadCSV(const std::string& filepath, const std::string& tableName) {
    std::ifstream file(filepath);
    if (!file.is_open()) throw std::runtime_error("Cannot open file: " + filepath);
    Table table;
    table.name = tableName;
    std::string line;
    bool firstLine = true;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        auto fields = splitCSVLine(line);
        if (firstLine) { table.columns = fields; firstLine = false; }
        else table.rows.push_back(fields);
    }
    return table;
}

struct SelectQuery {
    bool selectAll;
    std::vector<std::string> columns;
    std::string tableName;
};

std::string toUpper(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    return s;
}

std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end   = s.find_last_not_of(" \t\r\n");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

SelectQuery parseSelect(const std::string& query) {
    std::istringstream ss(query);
    std::string token;
    SelectQuery q;
    q.selectAll = false;
    ss >> token;
    if (toUpper(token) != "SELECT") throw std::runtime_error("Expected SELECT");
    std::string columnSection;
    while (ss >> token && toUpper(token) != "FROM") columnSection += token + " ";
    if (toUpper(token) != "FROM") throw std::runtime_error("Expected FROM");
    if (trim(columnSection) == "*") {
        q.selectAll = true;
    } else {
        std::replace(columnSection.begin(), columnSection.end(), ',', ' ');
        std::istringstream colStream(columnSection);
        std::string col;
        while (colStream >> col) q.columns.push_back(col);
    }
    if (!(ss >> token)) throw std::runtime_error("Expected table name after FROM");
    q.tableName = token;
    return q;
}

// Returns the projected rows (without printing) so tests can assert on values
std::vector<std::vector<std::string>> executeSelect(
    const SelectQuery& query,
    const std::unordered_map<std::string, Table>& db)
{
    auto it = db.find(query.tableName);
    if (it == db.end()) throw std::runtime_error("Table not found: " + query.tableName);

    const Table& table = it->second;
    std::vector<int> colIndices;

    if (query.selectAll) {
        for (int i = 0; i < (int)table.columns.size(); ++i) colIndices.push_back(i);
    } else {
        for (const auto& col : query.columns) {
            auto cit = std::find(table.columns.begin(), table.columns.end(), col);
            if (cit == table.columns.end())
                throw std::runtime_error("Column not found: " + col);
            colIndices.push_back((int)(cit - table.columns.begin()));
        }
    }

    std::vector<std::vector<std::string>> result;
    for (const auto& row : table.rows) {
        std::vector<std::string> projected;
        for (int idx : colIndices)
            projected.push_back(idx < (int)row.size() ? row[idx] : "");
        result.push_back(projected);
    }
    return result;
}

// ── Test Fixture ──────────────────────────────────────────────────────────────

class SelectTest : public ::testing::Test {
protected:
    std::unordered_map<std::string, Table> db;

    void SetUp() override {
        // Build an in-memory table matching data/sales_data.csv
        Table t;
        t.name    = "sales_data";
        t.columns = {"order_id", "customer_name", "product", "total", "status"};
        t.rows    = {
            {"1001", "Alice Chen",    "Wireless Mouse",      "59.98",  "Completed"},
            {"1002", "Brian Lee",     "Standing Desk",       "349.00", "Completed"},
            {"1003", "Carol Nguyen",  "Notebook Set",        "42.50",  "Completed"},
            {"1004", "David Kim",     "USB-C Hub",           "45.00",  "Refunded"},
            {"1005", "Emma Brown",    "Ergonomic Chair",     "529.00", "Pending"},
            {"1006", "Frank Wu",      "Mechanical Keyboard", "119.99", "Completed"},
        };
        db["sales_data"] = t;
    }
};

// ── Test Cases ────────────────────────────────────────────────────────────────

// TC1: SELECT * returns all columns and all rows
TEST_F(SelectTest, SelectAllReturnsAllColumnsAndRows) {
    auto q      = parseSelect("SELECT * FROM sales_data");
    auto result = executeSelect(q, db);

    EXPECT_EQ(result.size(), 6u);
    EXPECT_EQ(result[0].size(), 5u);  // 5 columns
    EXPECT_EQ(result[0][0], "1001");
    EXPECT_EQ(result[0][1], "Alice Chen");
}

// TC2: Column projection returns only specified columns
TEST_F(SelectTest, ColumnProjectionReturnsOnlySpecifiedColumns) {
    auto q      = parseSelect("SELECT customer_name, total FROM sales_data");
    auto result = executeSelect(q, db);

    EXPECT_EQ(result.size(), 6u);
    EXPECT_EQ(result[0].size(), 2u);
    EXPECT_EQ(result[0][0], "Alice Chen");
    EXPECT_EQ(result[0][1], "59.98");
}

// TC3: Single column selection
TEST_F(SelectTest, SingleColumnSelection) {
    auto q      = parseSelect("SELECT product FROM sales_data");
    auto result = executeSelect(q, db);

    EXPECT_EQ(result.size(), 6u);
    EXPECT_EQ(result[0].size(), 1u);
    EXPECT_EQ(result[0][0], "Wireless Mouse");
}

// TC4: Invalid column name throws an error
TEST_F(SelectTest, InvalidColumnThrowsError) {
    auto q = parseSelect("SELECT nonexistent FROM sales_data");
    EXPECT_THROW(executeSelect(q, db), std::runtime_error);

    try {
        executeSelect(q, db);
    } catch (const std::runtime_error& e) {
        EXPECT_STREQ(e.what(), "Column not found: nonexistent");
    }
}

// TC5: Invalid table name throws an error
TEST_F(SelectTest, InvalidTableThrowsError) {
    auto q = parseSelect("SELECT * FROM unknown_table");
    EXPECT_THROW(executeSelect(q, db), std::runtime_error);

    try {
        executeSelect(q, db);
    } catch (const std::runtime_error& e) {
        EXPECT_STREQ(e.what(), "Table not found: unknown_table");
    }
}

// TC6: Missing FROM keyword throws a parse error
TEST_F(SelectTest, MissingFromThrowsParseError) {
    EXPECT_THROW(parseSelect("SELECT customer_name sales_data"), std::runtime_error);

    try {
        parseSelect("SELECT customer_name sales_data");
    } catch (const std::runtime_error& e) {
        EXPECT_STREQ(e.what(), "Expected FROM");
    }
}

// TC7: Empty input is handled gracefully (no crash)
TEST_F(SelectTest, EmptyInputIsHandledGracefully) {
    EXPECT_THROW(parseSelect(""), std::runtime_error);
}

// ── Main ──────────────────────────────────────────────────────────────────────

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
