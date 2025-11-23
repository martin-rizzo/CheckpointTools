/*
| File    : common.cpp
| Purpose : Common types and constants used throughout the project.
|           (this file is a template to be used by Meson to generate the actual header file)
| Author  : Martin Rizzo | <martinrizzo@gmail.com>
| Date    : Nov 21, 2025
| Repo    : https://github.com/martin-rizzo/CheckpointTools
| License : MIT
|- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
|                              CheckpointTools
|      CLI tools for inspecting and manipulating model checkpoint files
\_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
#include <charconv>
#include "common.h"

int
to_integer(StringView str,
           int        defaultValue // = 0
){
    int  result;
    auto resultInfo = std::from_chars(str.data(), str.data() + str.size(), result);
    return resultInfo.ec == std::errc{} ? result : defaultValue;
}
