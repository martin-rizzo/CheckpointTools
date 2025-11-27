/*
| File    : messages.coo
| Purpose : Simple class with static methods to display messages in the console.
| Author  : Martin Rizzo | <martinrizzo@gmail.com>
| Date    : Nov 20, 2025
| Repo    : https://github.com/martin-rizzo/CheckpointTools
| License : MIT
|- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
|                              CheckpointTools
|      CLI tools for inspecting and manipulating model checkpoint files
\_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
#include <string_view> // for std::string_view
#include <vector>      // for std::vector
#include <iostream>    // for std::cerr
#include "colors.h" 
#include "messages.h"


/**
 * Displays a warning message to the console.
 *
 * This method is used to output messages that serve as warnings. These messages
 * indicate potential problems or non-critical information, allowing execution
 * to continue.
 * @param message The content of the warning message.
 */
void
Messages::warning(Text message) {
    auto c = Colors::instance();
    std::cerr << c.warning() << "[WARNING]" << c.reset() << " " << message << std::endl;
}

/**
 * Displays an error message to the console.
 *
 * This method is used to output messages indicating errors encountered during
 * execution. These errors are important but do not necessarily require
 * immediate termination of the program.
 * @param message The content of the error message.
 */
void
Messages::error(Text message) {
    auto c = Colors::instance();
    std::cerr << c.error() << "[ERROR]" << c.reset() << " " << message << std::endl;
}

/**
 * Displays a fatal error message to the console and exits the program.
 *
 * This method is used for displaying critical errors that necessitate the
 * immediate termination of the application. It begins by outputting a primary
 * error message, followed by any supplementary informative messages provided
 * as additional context.
 * 
 * After displaying these messages, it terminates the program execution using
 * `std::exit`.
 *
 * @param message      The main content of the fatal error message.
 * @param infoMessages An optional vector containing additional texts that
 *                     provide more context or details about the fatal error.
 * @param exitCode     An optional integer representing the termination
 *                     status of the program. 
 */
void
Messages::fatal_error(Text message,
                      const std::vector<Text>& infoMessages, // = {},
                      int                      exitCode      // = 1
){
    error(message);
    
    // Print additional messages if any
    auto c = Colors::instance();
    for (const auto& info : infoMessages) {
        std::cerr << " " << c.info() << "\xF0\x9F\x9B\x88 " << info << c.reset() << std::endl;
    }
    std::exit( exitCode>=1 ? exitCode : 1 );
}
