/*
| File    : table.h
| Purpose : A class for easily display column-aligned data.
| Author  : Martin Rizzo | <martinrizzo@gmail.com>
| Date    : Nov 27, 2025
| Repo    : https://github.com/martin-rizzo/CheckpointTools
| License : MIT
|- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
|                              CheckpointTools
|      CLI tools for inspecting and manipulating model checkpoint files
\_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
#pragma once
#ifndef TABLE_H_
#define TABLE_H_
#include <iostream>   // for std::ostream, std::cout
#include <vector>     // for std::vector
#include <string>     // for std::string
#include <functional> // for std::function

/**
 * A table class for easily display column-aligned data.
 * 
 * This class allows storing rows of string data and displaying them in a
 * formatted table with proper column alignment.
 * 
 * Example usage:
 * @code{cpp}
 * #include <iostream>
 * #include "table.h"
 * 
 * int main() {
 *     Table table;
 *     
 *     // Add rows
 *     table.add_row({"Name", "Age", "City"});
 *     table.add_row({"John", "25", "New York"});
 *     table.add_row({"Alice", "30", "London"});
 *     table.add_row({"Bob", "22", "Paris"});
 *     
 *     // Display table
 *     std::cout << table;
 *     
 *     return 0;
 * }
 * @endcode
 */
class Table
{
public:
    enum class Align {
        LEFT, RIGHT, CENTER
    };
    using Row          = std::vector<std::string>;
    using Rows         = std::vector<Row>;
    using Width        = int;
    using Widths       = std::vector<Width>;
    using Alignments   = std::vector<Align>;
    using Colorizer    = std::function<std::string(int column, const std::string& text)>;

// CONSTRUCTION/DESTRUCTION
public:
    Table(const Rows& rows);
    Table(const Table& other) = default;
    Table(Table&& other) noexcept = default;
    Table() = default;
    ~Table() = default;

// ASSIGNMENT OPERATORS
public:
    Table& operator=(const Table& other) = default;
    Table& operator=(Table&& other) noexcept = default;

// CAPACITY
public:
    size_t number_of_rows() const;
    size_t number_of_columns() const;
    bool   empty() const;
    void   reserve(size_t numberOfRows);

// FORMAT
public:
    void set_alignments(const std::initializer_list<Align>& alignments);
    void set_min_widths(const std::initializer_list<Width>& minWidths);
    void set_max_widths(const std::initializer_list<Width>& maxWidths);
    void set_colorizer(const Colorizer& colorizer);  

// MODIFIERS
public:
    void add_row(const Row& row);
    //void add_row(std::initializer_list<std::string> row);
    void add_row(std::initializer_list<std::string_view> values);
    void clear();

// OUTPUT
public:
    void print(std::ostream& out = std::cout) const;

// IMPLEMENTATION
private:
    void   _print_row(std::ostream& out, const Row& row, const Widths& columnWidths, const Alignments& columnAlignments) const;
    Widths _calculate_column_widths(const Rows& rows) const;
private:
    Rows       _rows;
    size_t     _numberOfColumns = 0;
    Alignments _columnAlignments;
    Widths     _minColumnWidths;
    Widths     _maxColumnWidths;
    Colorizer  _colorizer; 
};


//================================ INLINES ================================//

inline
std::ostream& operator<<(std::ostream& os, const Table& table) {
    table.print(os);
    return os;
}


 #endif // TABLE_H_
