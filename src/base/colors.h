/*
| File    : colors.h
| Purpose : A class for handling terminal colors.
|           This class is a singleton and can be accessed through the instance() method.
| Author  : Martin Rizzo | <martinrizzo@gmail.com>
| Date    : Nov 20, 2025
| Repo    : https://github.com/martin-rizzo/CheckpointTools
| License : MIT
|- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
|                              CheckpointTools
|      CLI tools for inspecting and manipulating model checkpoint files
\_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
#pragma once
#ifndef COLORS_H_
#define COLORS_H_
#include <string_view>


/**
 * ANSI color codes manager for terminal output
 * 
 * This class provides a thread-safe singleton instance for managing ANSI color
 * codes for terminal output. It supports disabling colors and provides various
 * color codes for different text formatting.
 * 
 * Example usage:
 * @code{.cpp}
 * #include <iostream>
 *
 * int main() {
 *     auto c = Colors::instance();
 *
 *     std::cout << c.red()   << "This is red text" << c.reset()   << std::endl;
 *     std::cout << c.green() << "This is green text" << c.reset() << std::endl;
 *
 *     // Disable color output
 *     c.disable_colors();
 *     std::cout << c.red() << "This will not be colored" << c.reset() << std::endl;
 *
 *     return 0;
 * }
 * @endcode
 */
class Colors
{
// SINGLETON
public:
     [[nodiscard]] static Colors& instance() noexcept;

// COPY/ASSIGNMENT/DESTRUCTION
public:
    Colors(const Colors&)                = default;
    Colors(Colors&&) noexcept            = default;
    Colors& operator=(const Colors&)     = default;
    Colors& operator=(Colors&&) noexcept = default;
    ~Colors() = default;

// GETTING STYLE COLORS
public:
    [[nodiscard]] std::string_view primary()   const noexcept;
    [[nodiscard]] std::string_view secondary() const noexcept;
    [[nodiscard]] std::string_view highlight() const noexcept;
    [[nodiscard]] std::string_view success()   const noexcept;
    [[nodiscard]] std::string_view warning()   const noexcept;
    [[nodiscard]] std::string_view error()     const noexcept;
    [[nodiscard]] std::string_view info()      const noexcept;
    [[nodiscard]] std::string_view reset()     const noexcept;

// GETTING SPECIFIC ANSI COLOR CODES
public:
    [[nodiscard]] std::string_view ansi_red()    const noexcept;
    [[nodiscard]] std::string_view ansi_yellow() const noexcept;
    [[nodiscard]] std::string_view ansi_green()  const noexcept;
    [[nodiscard]] std::string_view ansi_cyan()   const noexcept;

// DISABLING COLORS
public:
    void disable_colors() noexcept;
    [[nodiscard]] bool are_colors_enabled() const noexcept;


// IMPLEMENTATION
private:
    Colors() = default;
private:
    std::string_view _primary   { "\x1b[35m" };   // Magenta color
    std::string_view _secondary { "\x1b[36m" };   // Cyan color
    std::string_view _highlight { "\x1b[44m" };   // Blue background
    std::string_view _success   { "\x1b[32m" };   // Green text
    std::string_view _warning   { "\x1b[33m" };   // Yellow text
    std::string_view _error     { "\x1b[31m" };   // Red text
    std::string_view _info      { "\x1b[34m" };   // Blue text
    std::string_view _reset     { "\x1b[0m"  };   // Reset all previous colors
    std::string_view _ansiRed   { "\x1b[91m" };
    std::string_view _ansiYellow{ "\x1b[93m" };
    std::string_view _ansiGreen { "\x1b[92m" };
    std::string_view _ansiCyan  { "\x1b[96m" };
};


//===================== INLINES: GETTING STYLE COLORS =====================//

/** Returns the primary color code */
inline std::string_view Colors::primary() const noexcept { return _primary; }

/** Returns the secondary color code */
inline std::string_view Colors::secondary() const noexcept { return _secondary; }

/** Returns the highlight color code */
inline std::string_view Colors::highlight() const noexcept { return _highlight; }

/** Returns the success color code */
inline std::string_view Colors::success() const noexcept { return _success; }

/** Returns the warning color code */
inline std::string_view Colors::warning() const noexcept { return _warning; }

/** Returns the error color code */
inline std::string_view Colors::error() const noexcept { return _error; }

/** Returns the info color code */
inline std::string_view Colors::info() const noexcept { return _info; }

/** Returns the color code that resets the color */
inline std::string_view Colors::reset() const noexcept { return _reset; }


//================== INLINES: SPECIFIC ANSI COLOR CODES ===================//

/** Returns the red color code */
inline std::string_view Colors::ansi_red() const noexcept { return _ansiRed; }

/** Returns the yellow color code */
inline std::string_view Colors::ansi_yellow() const noexcept { return _ansiYellow; }

/** Returns the green color code */
inline std::string_view Colors::ansi_green() const noexcept { return _ansiGreen; }

/** Returns the cyan color code */
inline std::string_view Colors::ansi_cyan() const noexcept { return _ansiCyan; }



#endif // COLORS_H_
