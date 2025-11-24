/*
| File    : argument.cpp
| Purpose : 
| Author  : Martin Rizzo | <martinrizzo@gmail.com>
| Date    : Nov 24, 2025
| Repo    : https://github.com/martin-rizzo/CheckpointTools
| License : MIT
|- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
|                              CheckpointTools
|      CLI tools for inspecting and manipulating model checkpoint files
\_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
#include "argument.h"


//======================= CONSTRUCTION/DESTRUCTION ========================//

/**
 * Constructor for the Argument class.
 * 
 * @param i    The index of the current argument in the argv array.
 * @param argc The total number of arguments passed to the program (as passed to main).
 * @param argv The array of strings with the command-line arguments (as passed to main).
 */
Argument::Argument(int i, int argc, char* argv[]) {
    StringView curr{ argv[i]                       };
    StringView next{ (i+1) < argc ? argv[i+1] : "" };

    // by default, assume the associated value is the next non-option argument
    _name = curr;
    if( !next.starts_with("-") ) { _value = next; }

    // if the current argument starts with "-" and contains an "=",
    // extract the embedded value
    if( curr.starts_with("--") ) {
        auto it = curr.find('=');
        if( it != StringView::npos ) {
            _name  = curr.substr(0, it);
            _value = curr.substr(it+1);
            _valueIsEmbedded           = true;
            _valueExistsAndNotConsumed = true;
        }
    }

    // if the current argument isn't a option
    // force the value to be empty
    if( !curr.starts_with("-") ) {
        _value = "";
    }
}


//============================== ATTRIBUTES ===============================//

/**
 * Gets the name of the argument.
 * @returns The name of this argument.
 */
String
Argument::name() const noexcept {
    return String{_name};
}

/**
 * Returns the value associated with this argument, adjusting the provided index.
 * 
 * This method is intended to be used in loops where each iteration processes
 * one command-line argument.
 *
 * Example:
 * @code
 * for( int i=1; i < argc; ++i) {
 *     Argument arg{i, argc, argv};
 *     if(arg.is("--name")) { name = arg.value(i); }
 *     ....
 * }
 * @endcode
 *
 * @param i Reference to the index of the current argument.
 *          (this value will be updated if the argument's value was not embedded)
 * @return The value of this argument.
 */
String
Argument::value(int& i) const noexcept {
    StringView value{ _value };

    // if the value is not embedded in this argument,
    // increment the argument index since we have taken it from the next one
    if( !_valueIsEmbedded ) { ++i; }

    // flag value as consumed and return
    _valueExistsAndNotConsumed = false;
    return String{_value};
}

/**
 * Checks whether this argument has an associated value.
 */
bool
Argument::has_value() const noexcept {
    return !_value.empty();
}

/**
 * Indicates whether the value of this argument was previously consumed.
 */
bool
Argument::was_value_consumed() const noexcept {
    return !_valueExistsAndNotConsumed;
}

//========================= OPTION IDENTIFICATION =========================//

/**
 * Determines whether this argument represents a command-line option (starts with "-").
 */
bool
Argument::is_option() const noexcept {
    return _name.starts_with("-");
}

/**
 * Checks if this argument matches the specified name.
 * @param name The name to compare against.
 * @return `true` if the names match, otherwise false.
 */
bool
Argument::is(const char* name) const noexcept {
    return _name == name;
}

/**
 * Checks whether this argument matches either the shortname or longname provided.
 * @param shortname The potential short form of the option (e.g., "-s").
 * @param longname  The potential long form of the option (e.g., "--setting").
 * @return `true` if the current argument matches either shortname or longname, otherwise false.
 */
bool
Argument::is(const char* shortname, const char* longname) const noexcept {
    return _name == shortname || _name == longname;
}

