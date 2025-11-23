/*
| File    : args.cpp
| Purpose : Base class for args parsers with some useful functions for that purpose 
| Author  : Martin Rizzo | <martinrizzo@gmail.com>
| Date    : Nov 22, 2025
| Repo    : https://github.com/martin-rizzo/CheckpointTools
| License : MIT
|- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
|                              CheckpointTools
|      CLI tools for inspecting and manipulating model checkpoint files
\_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
#include "common.h"
#include "args.h"
using Option = Args::Option;

/**
 * Parses an option from the command-line arguments.
 *
 * This function parses an individual option from the provided command line 
 * arguments array. It supports various formats for specifying options:
 * - `--option=value` (the code should later call get_value to extract the value)
 * - `--option value` (the code should later call get_value to extract the value) 
 * - `--option`
 *
 * If an argument does not represent an option (i.e., it doesn't start with "-"),
 * this function returns an Option object where the name is set to the argument
 * and the value is empty. This ensures that calling `get_name()` on such an
 * Option will return the original argument string.
 *
 * @param i    The index of the argument to parse in the command line arguments array.
 * @param argc Total number of arguments passed via the command line (as received by main).
 * @param argv Array containing the strings of all arguments passed in the command line (as received by main).
 * @return
 *     An Option object representing the parsed option and its associated value, if any.
 */
Option
Args::parse_option(int i, int argc, char* argv[]) {

    // by default, assume the associated value is the next argument
    StringView curr{ argv[i] };
    StringView value{ (i+1) < argc && argv[i+1][0] != '-' ? argv[i+1] : "" };

    // if the option starts with "--" and contains an "=",
    // extract the value (including the "=")
    if( curr.starts_with("--") ) {
        if( auto it = curr.find('='); it != StringView::npos) {
            value = curr.substr(it);
            curr  = curr.substr(0, it);
        }
    }
    // if the option does not start with "--", assume no associated value
    else { value = ""; }
    return Option{ curr, value };
}

/**
 * Retrieves the name of an option.
 *
 * Given an Option object obtained from parse_option(), this function returns
 * the name part. The name is always extracted correctly regardless of whether
 * it was passed as "--option=value" or  separately as "--option value".
 *
 * @param option The Option object containing the information.
 * @param i      Current argument index (present for consistency but not used in this function).
 * @return Extracted name portion of the option.
 */
StringView
Args::get_name(const Option& option, int i) {
    (void)i;
    return option.first;
}

/**
 * Retrieves the value associated with an option.
 *
 * This function processes and returns the value part from an Option object.
 * If the original command line specified the option in "option=value" format,
 * it extracts just the value portion.
 * For options given as separate arguments (e.g., "--option arg"), this function
 * increments the argument index to point to the next expected option or argument.
 *
 * @param option Reference to the Option object containing the information.
 * @param i      Reference to the current argument index. Will be
 *               incremented if needed.
 * @return Extracted value associated with the given option.
 */
StringView
Args::get_value(Option& option, int& i) {
    StringView value = option.second;

    // if the value starts with "=", extract the portion after "="
    if( value.starts_with('=') ) { value = value.substr(1); }
    // if no leading '=', assume value was provided as a separate argument and advance 'i'
    else { i++; }

    // clear the option's second element to indicate the value has been processed
    option.second = "";
    return value;
}
