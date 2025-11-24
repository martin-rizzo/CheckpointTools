/*
| File    : argument.h
| Purpose : 
| Author  : Martin Rizzo | <martinrizzo@gmail.com>
| Date    : Nov 24, 2025
| Repo    : https://github.com/martin-rizzo/CheckpointTools
| License : MIT
|- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
|                              CheckpointTools
|      CLI tools for inspecting and manipulating model checkpoint files
\_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
#pragma once
#ifndef ARGUMENT_H_
#define ARGUMENT_H_
#include "common.h"

/**
 * A command-line argument parser providing methods to extract its name and value.
 *
 * This class encapsulates the information related to a single command-line
 * argument, enabling easy extraction of its name and any associated value.
 * It supports various formats, including options with an embedded value
 * ("--option=value"), separated values ("--option value"), or options without
 * any associated value (e.g., "--option").
 *
 * The Argument class also provides methods for identifying whether a given
 * argument is  an option, as well as checking if it matches specific short
 * or long names. This facilitates parsing and handling of command-line
 * options in applications.
 * 
 * Example:
 * @code{.cpp}
 *
 * int main(int argc, char* argv[]) {
 *     std::string output;
 *     bool show_help{ false };
 * 
 *     for (int i = 1; i < argc; ++i)
 *     {
 *         Argument arg(i, argc, argv);
 *         if( arg.is("-o","--output") ) {
 *             output = arg.value(i);
 *         } 
 *         else if( arg.is("--help") ) {
 *             show_help = true;
 *         }
 *         ...
 * 
 *         // when the user provides a value to an option that does not expect it
 *         if( !arg.was_value_consumed() ) {
 *            std::cerr << "The argument no expect a value: " << arg.name() << std::endl;
 *         }
 *     }
 *     ...
 * }
 * @endcode
 */
class Argument
{
// CONSTRUCTION/DESTRACTION
public:
    Argument(int i, int argc, char* argv[]);
    ~Argument() = default;

// ATTRIBUTES
public:
    [[nodiscard]] String name() const noexcept;
    [[nodiscard]] String value(int& i) const noexcept;
    [[nodiscard]] bool   has_value() const noexcept;
    [[nodiscard]] bool   was_value_consumed() const noexcept;

// OPTION IDENTIFICATION
public:
    [[nodiscard]] bool is_option() const noexcept;
    [[nodiscard]] bool is(const char* name) const noexcept;
    [[nodiscard]] bool is(const char* shortname, const char* longname) const noexcept;


// IMPLEMENTATION
private:
    StringView   _name;
    StringView   _value;
    bool         _valueIsEmbedded           = false;
    mutable bool _valueExistsAndNotConsumed = false;
};


#endif // ARGUMENT_H_