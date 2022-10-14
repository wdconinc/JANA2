
// Copyright 2021, Jefferson Science Associates, LLC.
// Subject to the terms in the LICENSE file found in the top-level directory.


#include "JTablePrinter.h"
#include "JANA/JLogger.h"


JTablePrinter::Column& JTablePrinter::AddColumn(std::string header, Justify justify, int desired_width) {
    columns.emplace_back();
    Column& col = columns.back();
    col.header = std::move(header);
    col.contents_width = col.header.size();
    col.justify = justify;
    col.desired_width = desired_width;
    col.use_desired_width = (desired_width != 0);
    return col;
}

size_t JTablePrinter::GetLinesInRow(size_t row) {
    size_t lines = 1;
    for (const Column& c : columns) {
        if (c.use_desired_width == false) continue;
        if (c.desired_width == 0) continue;
        size_t len = c.values[row].size();  // This probably doesn't handle non-ASCII character encodings correctly
        size_t col_lines = (len % c.desired_width == 0) ? len/c.desired_width : len/c.desired_width+1;
        if (col_lines > lines) lines = col_lines;
    }
    return lines;
}

void JTablePrinter::FormatCell(std::ostream& os, size_t line, std::string contents, int max_width, Justify justify) {
    auto cs = contents.size();
    size_t start = max_width*line; // Inclusive
    std::string line_contents = (start < cs) ? contents.substr(start, max_width) : "";

    if (justify == Justify::Left) {
        os << std::left << std::setw(max_width) << line_contents;
    }
    else if (justify == Justify::Right) {
        os << std::right << std::setw(max_width) << line_contents;
    }
    else {
        int lpad = (max_width-cs)/2; // center slightly to the left
        int rpad = (max_width-cs) - lpad;
        for (int i=0; i<lpad; ++i) os << " ";
        os << line_contents;
        for (int i=0; i<rpad; ++i) os << " ";
    }
}

void JTablePrinter::Render(std::ostream& os) {

    int total_rows = current_row;
    current_row = 0;

    // Calculate table width
    int table_width = 0;
    for (const Column& col : columns) {
        table_width += col.use_desired_width ? col.desired_width : col.contents_width;
    }
    table_width += (columns.size()-1)*cell_margin;

    // Print top rule
    if (top_rule == RuleStyle::Broken) {
        for (int i = 0; i<indent; ++i) os << " ";
        for (const auto& col : columns) {
            int underline_len = (col.use_desired_width)?col.desired_width:col.contents_width;
            for (int i=0; i<underline_len; ++i) os << "-";
            for (int i=0; i<cell_margin; ++i) os << " ";
        }
        os << std::endl;
    }
    else if (top_rule == RuleStyle::Across) {
        for (int i = 0; i<indent; ++i) os << " ";
        for (int i=0; i<table_width; ++i) os << "-";
        os << std::endl;
    }

    // Print headings
    for (int i = 0; i<indent; ++i) os << " ";
    for (const auto& col : columns) {
        // os << col.header;
        FormatCell(os, 0, col.header, col.use_desired_width?col.desired_width:col.contents_width, Justify::Center);
        for (int i=0; i<cell_margin; ++i) os << " ";
    }
    os << std::endl;

    // Print header rule
    if (header_rule == RuleStyle::Across) {
        for (int i = 0; i<indent; ++i) os << " ";
        for (int i = 0; i<table_width; ++i) os << "-";
        os << std::endl;
    }
    else if (header_rule == RuleStyle::Broken) {
        for (int i = 0; i<indent; ++i) os << " ";
        for (const auto& col : columns) {
            int underline_len = (col.use_desired_width)?col.desired_width:col.contents_width;
            for (int i=0; i<underline_len; ++i) os << "-";
            for (int i=0; i<cell_margin; ++i) os << " ";
        }
        os << std::endl;
    }

    // Print rows
    for (int row = 0; row < total_rows; ++row) {
        size_t lines = GetLinesInRow(row);
        for (size_t line=0; line < lines; ++line) {
            for (int i = 0; i<indent; ++i) os << " ";
            for (const auto& col : columns) {
                FormatCell(os, line, col.values[row], col.use_desired_width?col.desired_width:col.contents_width, col.justify);
                for (int i=0; i<cell_margin; ++i) os << " ";
            }
            os << std::endl;

        }
    }

    // Print bottom rule
    if (bottom_rule == RuleStyle::Broken) {
        for (int i = 0; i<indent; ++i) os << " ";
        for (const auto& col : columns) {
            int underline_len = (col.use_desired_width)?col.desired_width:col.contents_width;
            for (int i=0; i<underline_len; ++i) os << "-";
            for (int i=0; i<cell_margin; ++i) os << " ";
        }
        os << std::endl;
    }
    else if (bottom_rule == RuleStyle::Across) {
        for (int i = 0; i<indent; ++i) os << " ";
        for (int i = 0; i<table_width; ++i) os << "-";
        os << std::endl;
    }
}

