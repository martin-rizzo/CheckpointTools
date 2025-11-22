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

// GETTING COLORS
public:
    [[nodiscard]] std::string_view red()    const noexcept;
    [[nodiscard]] std::string_view yellow() const noexcept;
    [[nodiscard]] std::string_view green()  const noexcept;
    [[nodiscard]] std::string_view cyan()   const noexcept;
    [[nodiscard]] std::string_view reset()  const noexcept;
    [[nodiscard]] std::string_view get_color_code(const std::string_view& colorName) const noexcept;

// DISABLING COLORS
public:
    void disable_colors() noexcept;
    [[nodiscard]] bool are_colors_enabled() const noexcept;


// IMPLEMENTATION
private:
    Colors() = default;
private:
    const char* _red   { "\x1b[91m" };
    const char* _yellow{ "\x1b[93m" };
    const char* _green { "\x1b[92m" };
    const char* _cyan  { "\x1b[96m" };
    const char* _reset { "\x1b[0m"  };
};

//================================ INLINES ================================//

/** Returns the red color code */
inline std::string_view Colors::red() const noexcept { return _red; }

/** Returns the yellow color code */
inline std::string_view Colors::yellow() const noexcept { return _yellow; }

/** Returns the green color code */
inline std::string_view Colors::green() const noexcept { return _green; }

/** Returns the cyan color code */
inline std::string_view Colors::cyan() const noexcept { return _cyan; }

/** Returns the color code that resets the color */
inline std::string_view Colors::reset() const noexcept { return _reset; }


#endif // COLORS_H_
