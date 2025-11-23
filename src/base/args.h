/*
| File    : args.h
| Purpose : Base class for args parsers with some useful functions for that purpose 
| Author  : Martin Rizzo | <martinrizzo@gmail.com>
| Date    : Nov 22, 2025
| Repo    : https://github.com/martin-rizzo/CheckpointTools
| License : MIT
|- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
|                              CheckpointTools
|      CLI tools for inspecting and manipulating model checkpoint files
\_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
#pragma once
#ifndef ARGS_H_
#define ARGS_H_
#include "common.h"


/**
 * Provides utility functions for parsing command-line options and arguments.
 *
 * This class contains a set of static methods designed to facilitate the
 * extraction and handling of options and their associated values from a
 * command-line argument list. It supports various formats, including those
 * with an immediate value ("--option=value"), separated value ("--option value"),
 * or no value at all (e.g., "--option").
 *
 * The class is intended to be used as a base for classes that need to parse 
 * command-line arguments in a more manual and controlled manner. It provides
 * methods to parse individual options, retrieve their names, extract their
 * values, and handle positional arguments.
 */
class Args
{
public:
    using Option = std::pair<StringView, StringView>;
public:
    static Option     parse_option(int i, int argc, char* argv[]);
    static StringView get_name(const Option& option, int i);
    static StringView get_value(Option& option, int& i);
};


#endif // ARGS_H_