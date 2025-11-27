/*
| File    : colors.h
| Purpose : A class for handling terminal colors.
|           This class is a singleton and can be accessed through the `Colors::instance()` method.
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
#include <string>


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
 * #include "colors.h"
 *
 * int main() {
 *     auto& c = Colors::instance();
 *
 *     std::cout << c.primary()   << "This is primary colored text"        << c.reset() << std::endl;
 *     std::cout << c.highlight() << "This is highlighted important info"  << c.reset() << std::endl;
 *     std::cout << c.group()     << "This represents a group of items"    << c.reset() << std::endl;
 *     std::cout << c.data()      << "Displaying some data here"           << c.reset() << std::endl;
 *     std::cout << c.data2()     << "Alternative shade for data display"  << c.reset() << std::endl;
 *     std::cout << c.success()   << "Operation succeeded!"                << c.reset() << std::endl;
 *     std::cout << c.warning()   << "Something to be aware of"            << c.reset() << std::endl;
 *     std::cout << c.error()     << "There was an error, please fix it."  << c.reset() << std::endl;
 *     std::cout << c.info()      << "General information is here"         << c.reset() << std::endl;
 * 
 *     // Disable color output
 *     c.disable_colors();
 *     std::cout << c.success() << "This will not be colored" << c.reset() << std::endl;
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
    [[nodiscard]] const std::string& primary()   const noexcept;
    [[nodiscard]] const std::string& highlight() const noexcept;
    [[nodiscard]] const std::string& group()     const noexcept;
    [[nodiscard]] const std::string& data()      const noexcept;
    [[nodiscard]] const std::string& data2()     const noexcept;
    [[nodiscard]] const std::string& success()   const noexcept;
    [[nodiscard]] const std::string& warning()   const noexcept;
    [[nodiscard]] const std::string& error()     const noexcept;
    [[nodiscard]] const std::string& info()      const noexcept;
    [[nodiscard]] const std::string& reset()     const noexcept;

// GETTING SPECIFIC ANSI COLOR CODES
public:
    [[nodiscard]] const std::string& ansi_red()    const noexcept;
    [[nodiscard]] const std::string& ansi_yellow() const noexcept;
    [[nodiscard]] const std::string& ansi_green()  const noexcept;
    [[nodiscard]] const std::string& ansi_cyan()   const noexcept;

// DISABLING COLORS
public:
    void disable_colors() noexcept;
    [[nodiscard]] bool are_colors_enabled() const noexcept;


// IMPLEMENTATION
private:
    Colors() = default;
private:
    std::string _primary   { "\x1b[;37m"   };
    std::string _highlight { "\x1b[;97m"   };
    std::string _group     { "\x1b[;94m"   };
    std::string _data      { "\x1b[;32m"   };
    std::string _data2     { "\x1b[;33m"   };
    std::string _success   { "\x1b[;1;32m" };
    std::string _error     { "\x1b[;1;31m" };
    std::string _info      { "\x1b[;1;34m" };
    std::string _warning   { "\x1b[;1;33m" };
    std::string _reset     { "\x1b[0m"     };
    std::string _ansiRed   { "\x1b[;31m"   };
    std::string _ansiYellow{ "\x1b[;33m"   };
    std::string _ansiGreen { "\x1b[;32m"   };
    std::string _ansiCyan  { "\x1b[;36m"   };
};


//===================== INLINES: GETTING STYLE COLORS =====================//

/**
 * Returns the primary color.
 * @details This color is typically used as the main color throughout the output.
 */
inline const std::string&
Colors::primary() const noexcept { return _primary; }

/**
 * Returns the highlight color.
 * @details This color is used to emphasize or draw attention to specific
 *          elements within the output. It complements the primary color and
 *          helps to distinguish important details within the output.
 */
inline const std::string&
Colors::highlight() const noexcept { return _highlight; }

/**
 * Returns the group text color.
 * @details This color is intended for representing groups of items, like
 *          directories, collections, etc., in a distinct way.
 */
inline const std::string&
Colors::group() const noexcept { return _group; }

/**
 * Returns the data text color.
 * @details This color is used for displaying textual data such as values of
 *          variables, numerical information, etc.
 */
inline const std::string&
Colors::data() const noexcept { return _data; }

/**
 * Returns an alternative data text color.
 * @details Similar to the 'data()' method but provides a different shade for
 *          variety in displaying textual data values.
 */
inline const std::string&
Colors::data2() const noexcept { return _data2; }

/**
 * Returns the success indicator color.
 * @details This color is used to denote successful operations or positive
 *          outcomes in the application.
 */
inline const std::string&
Colors::success() const noexcept { return _success; }

/**
 * Returns the error indicator color.
 * @details This color signifies that an operation has failed or there's a
 *          problem, alerting users to potential issues.
 */
inline const std::string&
Colors::error() const noexcept { return _error; }

/**
 * Returns the informational text color.
 * @details This color is used for providing general information or explanatory
 *          messages to the user.
 */
inline const std::string&
Colors::info() const noexcept { return _info; }

/**
 * Returns the warning text color.
 * @details Represents cautionary or advisory messages that may require user
 *          attention but are not critical errors.
 */
inline const std::string&
Colors::warning() const noexcept { return _warning; }

/**
 * Returns the ANSI code that reset any applied text styles to default.
 * @details This is useful for ending a series of colored text, ensuring the
 *          following text appears in the standard format.
 */
inline const std::string&
Colors::reset() const noexcept { return _reset; }


//================== INLINES: SPECIFIC ANSI COLOR CODES ===================//

/** Returns the ansi code for red */
inline const std::string&
Colors::ansi_red() const noexcept { return _ansiRed; }

/** Returns the ansi code for yellow */
inline const std::string&
Colors::ansi_yellow() const noexcept { return _ansiYellow; }

/** Returns the ansi code for green */
inline const std::string&
Colors::ansi_green() const noexcept { return _ansiGreen; }

/** Returns the ansi code for cyan */
inline const std::string&
Colors::ansi_cyan() const noexcept { return _ansiCyan; }


#endif // COLORS_H_
