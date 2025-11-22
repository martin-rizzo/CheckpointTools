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

enum class UseColor {
    AUTO,
    NEVER,
    ALWAYS
};
inline std::string
to_string(UseColor color) {
    switch (color) {
        case UseColor::AUTO  : return "UseColor::AUTO";
        case UseColor::NEVER : return "UseColor::NEVER";
        case UseColor::ALWAYS: return "UseColor::ALWAYS";
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
    Command command = Command::LIST_TENSORS;
    std::string filename = "";             ///< The name of the file to read
    std::string name = "";                 ///< The name of the tensor to print
    std::string prefix = "";               ///< Only print tensors with this prefix
    int depth = 0;                         ///< The depth of the tree to print
    Format format = Format::HUMAN;         ///< Output format
    UseColor use_color = UseColor::AUTO;   ///< Whether to use color in output
    bool help = false;                     ///< true = print usage and exit
    bool version = false;                  ///< true = print version and exit
    const char * const help_message;
};

std::ostream& operator<<(std::ostream& os, const CkShowArgs& args);

#endif // CKSHOW_ARGS_H_