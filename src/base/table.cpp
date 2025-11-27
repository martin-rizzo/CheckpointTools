/*
| File    : table.cpp
| Purpose : A class for easily display column-aligned data.
| Author  : Martin Rizzo | <martinrizzo@gmail.com>
| Date    : Nov 27, 2025
| Repo    : https://github.com/martin-rizzo/CheckpointTools
| License : MIT
|- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
|                              CheckpointTools
|      CLI tools for inspecting and manipulating model checkpoint files
\_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
#include "table.h"
#include <format>    // for std::format [C++20]
#include <algorithm> // for std::max

//======================= CONSTRUCTION/DESTRUCTION ========================//

/**
 * Constructs a Table object with initial rows.
 * @param rows Initial rows to populate the table
 */
Table::Table(const Rows& rows
): _rows(rows)
{
    // calculate the number of columns based on initial rows
    _numberOfColumns = 0;
    for( const auto& row : _rows ) {
        _numberOfColumns = std::max(_numberOfColumns, row.size());
    }
}

//=============================== CAPACITY ================================//

/**
 * Get the number of rows in the table
 * @return Number of rows
 */
size_t
Table::number_of_rows() const {
    return _rows.size();
}

size_t
Table::number_of_columns() const {
    return _numberOfColumns;
}

/**
 * Check if the table is empty
 * @return `true` if table has no rows, false otherwise
 */
bool Table::empty() const {
    return _rows.empty();
}

/**
 * Reserve space for a specified number of rows.
 */
void Table::reserve(size_t numberOfRows) {
    _rows.reserve(numberOfRows);
}

//================================ FORMAT =================================//

void Table::set_alignments(const std::initializer_list<Align>& alignments) {
    _columnAlignments = alignments;
}

void Table::set_min_widths(const std::initializer_list<Width>& minWidths) {
    _minColumnWidths = minWidths;
}

void Table::set_max_widths(const std::initializer_list<Width>& maxWidths) {
    _maxColumnWidths = maxWidths;
}

void Table::set_colorizer(const Colorizer& colorizer) {
    _colorizer = colorizer;
}

//=============================== MODIFIERS ===============================//

/**
 * Add a row to the table
 * @param row Vector of strings representing a row
 */
void
Table::add_row(const Row& row) {
    _rows.push_back(row);
    _numberOfColumns = std::max(_numberOfColumns, row.size());
}

void Table::add_row(std::initializer_list<std::string_view> values) {
    Row row;
    row.reserve(values.size());
    for( auto& strview : values ) { row.push_back(std::string(strview));  }
    _rows.push_back( std::move(row) );
    _numberOfColumns = std::max(_numberOfColumns, values.size());
}

/**
 * Clear all rows from the table
 */
void Table::clear() {
    _rows.clear();
    _numberOfColumns = 0;
}

//================================ OUTPUT =================================//

// display the table
// outputs the table row by row with proper column alignment.
void
Table::print(std::ostream& out // = std::cout
) const {

    // if the table is empty there's nothing to do
    if( _rows.empty() ) { return; }

    // calculate widths of each column and output rows with proper alignment
    auto columnWidths = _calculate_column_widths(_rows);
    for( const auto& row : _rows ) {
        _print_row(out, row, columnWidths, _columnAlignments);
    }
}

//============================ IMPLEMENTATION =============================//

/**
 * private funciton Print a single row with proper alignment
 * @param row Row to print
 */
void
Table::_print_row(std::ostream&     os,
                  const Row&        row,
                  const Widths&     columnWidths,
                  const Alignments& columnAlignments
) const {
    static const std::string Separator = " ";
    std::string text;

    for( size_t i=0 ; i<row.size() ; ++i )
    {
        if( i>0 ) { os << Separator; }
        const Width width = i<columnWidths.size() ? columnWidths[i] : 0;
        std::string text = row[i];

        // if the width is 0 (or invalid), then print the text as is
        // (this should never happen, but just in case)
        if( width<=0 ) {
            os << (_colorizer ? _colorizer(i,text) : text);
            continue;
        }

        // apply alignment to the test
        const auto align = i<columnAlignments.size() ? columnAlignments[i] : Align::LEFT;
        switch(align) {
            case Align::LEFT:
                text = std::format("{:<{}}", text, width);
                break;
            case Align::RIGHT:
                text = std::format("{:>{}}", text, width);
                break;
            case Align::CENTER:
                text = std::format("{:^{}}", text, width);
                break;
        }
        // print colorized text
        os << (_colorizer ? _colorizer(i,text) : text);
    }
    os << std::endl;
}

/**
 * Calculates column widths based on content of all rows.
 *
 * This method determines the maximum width needed for each column by analyzing 
 * the length of content in every row. It ensures that calculated widths do not 
 * exceed specified maximums or fall below minimums per column.
 *
 * @param rows The input table rows to analyze for determining column widths.
 * @return A vector containing the calculated optimal width for each column.
 */
Table::Widths
Table::_calculate_column_widths(const Rows& rows) const {
    Widths widths;
   
    // if the table is empty there's nothing to do
    if( _rows.empty() ) { return widths; }

    // apply minimum column widths first
    widths = _minColumnWidths;
    
    // iterate over all rows to calculate the optimal column widths
    for( const auto& row : _rows )
    {
        // make sure the `widths` vector is at least as large as the current row
        if( row.size()>widths.size() ) { widths.resize( row.size(), 0 ); }
        
        // update column widths to be the maximum
        for( size_t column = 0 ; column < row.size() ; ++column ) {
            Width textLength( row[column].length() );
            widths[column] = std::max( widths[column], textLength );
        }
    }
    // ensure no calculated column width exceeds its specified maximum
    const size_t arraySize = std::min( widths.size(), _maxColumnWidths.size() );
    for( size_t i = 0; i < arraySize ; ++i ) {
        auto maxWidth = _maxColumnWidths[i];
        if( maxWidth > 0 ) {
            widths[i] = std::min(widths[i], maxWidth);
        }
    }

    return widths;
}



