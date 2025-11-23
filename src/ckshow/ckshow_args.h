/*
| File    : ckshow_args.h
| Purpose : The arguments of the `ckshow` command line
| Author  : Martin Rizzo | <martinrizzo@gmail.com>
| Date    : Nov 20, 2025
| Repo    : https://github.com/martin-rizzo/CheckpointTools
| License : MIT
|- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
|                              CheckpointTools
|      CLI tools for inspecting and manipulating model checkpoint files
\_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
#pragma once
#ifndef CKSHOW_ARGS_H_
#define CKSHOW_ARGS_H_
#include <iostream>
#include "common.h"

enum class Command {
    LIST_TENSORS,
    LIST_METADATA,
    EXTRACT_THUMBNAIL
};
inline String
to_string(Command command) {
    switch (command) {
        case Command::LIST_TENSORS     : return "Command::LIST_TENSORS";
        case Command::LIST_METADATA    : return "Command::LIST_METADATA";
        case Command::EXTRACT_THUMBNAIL: return "Command::EXTRACT_THUMBNAIL";
        default: return "<unknown>";
    }
}

enum class Format {
    HUMAN,
    PLAIN,
    JSON
};
inline std::string
to_string(Format format) {
    switch (format) {
        case Format::HUMAN: return "Format::HUMAN";
        case Format::PLAIN: return "Format::PLAIN";
        case Format::JSON : return "Format::JSON";
        default: return "<unknown>";
    }
}


struct CkShowArgs
{
// CONSTRUCTION/DESTRUCTION
public:
    CkShowArgs(int argc, char* argv[]);
    CkShowArgs() = default;
    CkShowArgs(const CkShowArgs&) = default;
    CkShowArgs(CkShowArgs&&) noexcept = default;
    ~CkShowArgs() = default;

// PUBLIC MEMBERS
public:
    Command command    = Command::LIST_TENSORS;
    String  filename   = "";            ///< The name of the file to read
    String  name       = "";            ///< The name of the tensor to print
    String  prefix     = "";            ///< Only print tensors with this prefix
    String  when_color = "auto";        ///< When to use color in output
    int     depth      = 0;             ///< The depth of the tree to print
    Format  format     = Format::HUMAN; ///< Output format
    bool    help       = false;         ///< true = print usage and exit
    bool    version    = false;         ///< true = print version and exit
    const char * const help_message;
};

/**
 * Overloads the insertion operator (<<) for printing CkShowArgs objects to an output stream.
 *
 * This function provides a way to print the contents of an Args object in a 
 * human-readable format. It outputs each member variable with its name and
 * value to the specified output stream, facilitating debugging or logging.
 *
 * @param os   The output stream where the Args data will be printed.
 * @param args The Args object being printed to the stream.
 * @return A reference `os` for chaining.
 */
inline std::ostream&
operator<<(std::ostream& os, const CkShowArgs& args) {
    os << "Args:"                                         << std::endl;
    os << "  command: "     << to_string(args.command)    << std::endl;
    os << "  filename: "    << args.filename              << std::endl;
    os << "  name: "        << args.name                  << std::endl;
    os << "  prefix: "      << args.prefix                << std::endl;
    os << "  when_color: "  << args.when_color            << std::endl;
    os << "  depth: "       << args.depth                 << std::endl;
    os << "  format: "      << to_string(args.format)     << std::endl;
    os << "  help: "        << to_string(args.help)       << std::endl;
    os << "  version: "     << to_string(args.version);
    return os;
}

#endif // CKSHOW_ARGS_H_