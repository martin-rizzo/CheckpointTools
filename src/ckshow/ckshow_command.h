/*
| File    : ckshow_command.h
| Purpose : The `ckshow` command line tool.
| Author  : Martin Rizzo | <martinrizzo@gmail.com>
| Date    : Nov 20, 2025
| Repo    : https://github.com/martin-rizzo/CheckpointTools
| License : MIT
|- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
|                              CheckpointTools
|      CLI tools for inspecting and manipulating model checkpoint files
\_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
#pragma once
#ifndef CKSHOW_COMMAND_H_
#define CKSHOW_COMMAND_H_
#include <tin/readerror.h>  // for tin::ReadError
#include <tin/tensormap.h>  // for tin::TensorMap
#include "ckshow_args.h"    // for CkShowArgs
using tin::TensorMap;
using tin::ReadError;

class CkShowCommand
{
// MAIN
public:
    CkShowCommand(const CkShowArgs& args);
    [[nodiscard]] int run();

// SUBCOMMANDS
public:
    void list_tensors_columns(const TensorMap& tensorMap) const;
    void list_tensors_csv(const TensorMap& tensorMap, bool includeHeaders=true) const;
    void list_metadata(const TensorMap& tensorMap) const;
    void print_metadata(const TensorMap& tensorMap, const std::string& key) const;

// HELPERS
public:
    static void print_help();
    [[noreturn]] static void fatal_read_error(ReadError error);


// IMPLEMENTATION
private:
    const CkShowArgs _args;
};

#endif // CKSHOW_COMMAND_H_
