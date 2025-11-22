/*
| File    : messages.h
| Purpose : Simple class with static methods to display messages in the console.
| Author  : Martin Rizzo | <martinrizzo@gmail.com>
| Date    : Nov 20, 2025
| Repo    : https://github.com/martin-rizzo/CheckpointTools
| License : MIT
|- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
|                              CheckpointTools
|      CLI tools for inspecting and manipulating model checkpoint files
\_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
#pragma once
#ifndef MESSAGES_H_
#define MESSAGES_H_
#include <string_view> // for std::string_view
#include <vector>      // for std::vector


/**
 * SSimple class with static methods to display messages in the console.
 *
 * This class provides static methods to display different types of messages:
 *  - Warnings    : Informative messages that do not interrupt the flow of the program.
 *  - Errors      : Issues encountered during execution that may require attention but are not critical.
 *  - Fatal Errors: Critical issues that warrant immediate termination of the program.
 */
class Messages
{
public:
    using Text = std::string_view;
public:
    static              void warning    (Text message);
    static              void error      (Text message);
    [[noreturn]] static void fatal_error(Text message,
                                         const std::vector<Text>& infoMessages = {},
                                         int                      exitCode     = 1);
};


#endif // MESSAGES_H_
