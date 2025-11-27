/*
| File    : colors.cpp
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
#include <mutex>   // for std::once_flag
#include <memory>  // for std::unique_ptr
#include "colors.h"

//=============================== SINGLETON ===============================//

/**
 * Returns a reference to the singleton instance of Colors.
 * @return Reference to the single Colors instance.
 * @note This method is thread-safe (because if we're going to do it, let's get it right!)
 */
Colors&
Colors::instance() noexcept {
    static std::once_flag          flag;
    static std::unique_ptr<Colors> instancePtr;
    std::call_once(flag, []() {
        instancePtr = std::unique_ptr<Colors>( new Colors() );
    });
    return *instancePtr;
}

//=========================== DISABLING COLORS ============================//

/**
 * Disable all colors
 */
void
Colors::disable_colors() noexcept {
    _primary = _secondary = _highlight = _success = _warning = _error = _info = \
    _reset = _ansiRed = _ansiYellow = _ansiGreen = _ansiCyan = "";
}


/**
 * Check if color codes are enabled
 * @return true if colors are enabled, false otherwise
 */
bool
Colors::are_colors_enabled() const noexcept {
    return !_ansiRed.empty() || !_ansiGreen.empty();
}
