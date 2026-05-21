#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <stdexcept>

// ── CSV Storage ──────────────────────────────────────────────────────────────

struct Table {
    std::string name;
    std::vector<std::string> columns;
    std::vector<std::vector<std::string>> rows;
};

std::vector<std::string> splitCSVLine(const std::string& line) {
    std::vector<std::string> fields;
    std::stringstream ss(line);
    std::string field;
    while (std::getline(ss, field, ',')) {
        fields.push_back(field);
    }
    return fields;
}

Table loadCSV(const std::string& filepath, const std::string& tableName) {
    std::ifstream file(filepath);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file: " + filepath);

    Table table;
    table.name = tableName;

    std::string line;
    bool firstLine = true;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        auto fields = splitCSVLine(line);
        if (firstLine) {
            table.columns = fields;
            firstLine = false;
        } else {
            table.rows.push_back(fields);
        }
    }
    return table;
}

// ── Query Parser ─────────────────────────────────────────────────────────────

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

// Tokenize by whitespace, treating commas as separators within column list
SelectQuery parseSelect(const std::string& query) {
    std::istringstream ss(query);
    std::string token;
    SelectQuery q;
    q.selectAll = false;

    // Expect SELECT
    ss >> token;
    if (toUpper(token) != "SELECT")
        throw std::runtime_error("Expected SELECT");

    // Read column list until FROM
    std::string columnSection;
    while (ss >> token && toUpper(token) != "FROM") {
        columnSection += token + " ";
    }
    if (toUpper(token) != "FROM")
        throw std::runtime_error("Expected FROM");

    // Parse column list
    if (trim(columnSection) == "*") {
        q.selectAll = true;
    } else {
        // Remove commas and split
        std::replace(columnSection.begin(), columnSection.end(), ',', ' ');
        std::istringstream colStream(columnSection);
        std::string col;
        while (colStream >> col) {
            q.columns.push_back(col);
        }
    }

    // Read table name
    if (!(ss >> token))
        throw std::runtime_error("Expected table name after FROM");
    q.tableName = token;

    return q;
}

// ── Query Executor ────────────────────────────────────────────────────────────

void printRow(const std::vector<std::string>& row, const std::vector<int>& colIndices, const std::vector<int>& widths) {
    for (size_t i = 0; i < colIndices.size(); ++i) {
        int idx = colIndices[i];
        std::string val = (idx < (int)row.size()) ? row[idx] : "";
        std::cout << "| " << val;
        std::cout << std::string(widths[i] - val.size(), ' ') << " ";
    }
    std::cout << "|\n";
}

void executeSelect(const SelectQuery& query, const std::unordered_map<std::string, Table>& db) {
    auto it = db.find(query.tableName);
    if (it == db.end())
        throw std::runtime_error("Table not found: " + query.tableName);

    const Table& table = it->second;

    // Resolve column indices
    std::vector<int> colIndices;
    std::vector<std::string> colNames;

    if (query.selectAll) {
        for (int i = 0; i < (int)table.columns.size(); ++i)
            colIndices.push_back(i);
        colNames = table.columns;
    } else {
        for (const auto& col : query.columns) {
            auto cit = std::find(table.columns.begin(), table.columns.end(), col);
            if (cit == table.columns.end())
                throw std::runtime_error("Column not found: " + col);
            colIndices.push_back((int)(cit - table.columns.begin()));
            colNames.push_back(col);
        }
    }

    // Calculate column widths for formatting
    std::vector<int> widths(colNames.size());
    for (size_t i = 0; i < colNames.size(); ++i)
        widths[i] = colNames[i].size();

    for (const auto& row : table.rows) {
        for (size_t i = 0; i < colIndices.size(); ++i) {
            int idx = colIndices[i];
            int len = (idx < (int)row.size()) ? row[idx].size() : 0;
            widths[i] = std::max(widths[i], len);
        }
    }

    // Print header
    std::string separator = "+";
    for (int w : widths) separator += std::string(w + 2, '-') + "+";
    std::cout << separator << "\n";

    for (size_t i = 0; i < colNames.size(); ++i) {
        std::cout << "| " << colNames[i]
                  << std::string(widths[i] - colNames[i].size(), ' ') << " ";
    }
    std::cout << "|\n" << separator << "\n";

    // Print rows
    for (const auto& row : table.rows)
        printRow(row, colIndices, widths);

    std::cout << separator << "\n";
    std::cout << table.rows.size() << " row(s) returned.\n";
}

// ── REPL ──────────────────────────────────────────────────────────────────────

int main() {
    // Pre-load CSV files into the in-memory database
    std::unordered_map<std::string, Table> db;
    try {
        db["sales_data"] = loadCSV("data/sales_data.csv", "sales_data");
    } catch (const std::exception& e) {
        std::cerr << "Warning: " << e.what() << "\n";
    }

    std::cout << "CSV Query Engine\n";
    std::cout << "Loaded tables: ";
    for (auto& kv : db) std::cout << kv.first << " ";
    std::cout << "\nType a SELECT query or 'exit' to quit.\n\n";

    std::string line;
    while (true) {
        std::cout << ">> ";
        if (!std::getline(std::cin, line)) break;
        line = trim(line);
        if (line.empty()) continue;
        if (toUpper(line) == "EXIT") break;

        try {
            SelectQuery q = parseSelect(line);
            executeSelect(q, db);
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
        std::cout << "\n";
    }

    return 0;
}
