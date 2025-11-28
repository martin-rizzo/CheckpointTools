/*
| File    : ckshow.h
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
#ifndef CKSHOW_H_
#define CKSHOW_H_
#include <tin/readerror.h>  // for tin::ReadError
#include <tin/tensormap.h>  // for tin::TensorMap
#include "common.h"
#include "ckshow_args.h"    // for CkShowArgs
using tin::TensorMap;
using tin::ReadError;

class CkShow
{
// MAIN
public:
    CkShow(const CkShowArgs& args);
    [[nodiscard]] int run();

// SUBCOMMANDS
public:
    void list_tensors(const TensorMap& tensorMap) const;
    void list_tensors_columns(const TensorMap& tensorMap) const;
    void list_tensors_csv(const TensorMap& tensorMap, bool includeHeaders=true) const;
    void list_metadata(const TensorMap& tensorMap) const;
    void print_metadata(const TensorMap& tensorMap, StringView key) const;

// HELPERS
public:
    const String& to_string(tin::StorageType storageType) const noexcept;
    void print_help() const noexcept;
    void print_version() const noexcept;
    [[noreturn]] static void fatal_read_error(ReadError error);


// IMPLEMENTATION
private:
    const CkShowArgs _args;
};

#endif // CKSHOW_H_
