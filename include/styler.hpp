#ifndef STYLER_HPP
#define STYLER_HPP

#if defined(__unix__) || defined(__unix) || defined(__linux__)
#define STYLER_LINUX
#elif defined(WIN32) || defined(_WIN32) || defined(_WIN64)
#define STYLER_WINDOWS
#elif defined(__APPLE__) || defined(__MACH__)
#define STYLER_MACOSX
#else
#error Unknown Platform
#endif

#include <algorithm>
#include <atomic>
#include <iostream>
#include <type_traits>

namespace styler
{
enum class Style
{
    Reset,
    Bold,
    Dim,
    Italic,
    Underline,
    Blink,
    RBlink,
    Reversed,
    Conceal,
    Crossed
};

enum class Foreground
{
    Black,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    Gray,
    Reset
};

enum class Background
{
    Black,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    Gray,
    Reset
};

//! Behavior of styler function calls.
enum class Control
{
    Off,   //! Toggle off rang style/color calls
    Auto,  //! (Default) auto-detect terminal and colorize if needed
    Force  //! Force ansi color output to non terminal streams
};

//! Terminal mode of Windows.
enum class WinTerm
{
    Auto,   // (Default) Automatically detects whether ANSI or Native API
    Ansi,   // Force use ANSI API
    Native  // Force use Native API
};

namespace internal
{
inline std::atomic<Control>& GetControlMode() noexcept
{
    static std::atomic<Control> controlMode{ Control::Auto };
    return controlMode;
}

inline std::atomic<WinTerm>& GetWinTermMode() noexcept
{
    static std::atomic<WinTerm> termMode{ WinTerm::Auto };
    return termMode;
}

inline bool IsSupportColor() noexcept
{
#if defined(STYLER_LINUX) || defined(STYLER_MACOSX)
    const char* terms[] = { "ansi",    "color",  "console", "cygwin", "gnome",
                            "konsole", "kterm",  "linux",   "msys",   "putty",
                            "rxvt",    "screen", "vt100",   "xterm" };

    const char* envParam = std::getenv("TERM");
    if (!envParam)
    {
        return false;
    }

    return std::any_of(std::begin(terms), std::end(terms),
                       [&](const char* term) {
                           return std::strstr(envParam, term) != nullptr;
                       });
#elif defined(STYLER_WINDOWS)
    // All windows versions support colors through native console methods
    return true;
#endif
}

template <typename T>
using IsValid =
    typename std::enable_if<std::is_same<T, Style>::value ||
                                std::is_same<T, Foreground>::value ||
                                std::is_same<T, Background>::value,
                            std::ostream&>::type;
}  // namespace internal

template <typename T>
internal::IsValid<T> operator<<(std::ostream& os, const T value)
{
    (void)value;

    const Control controlMode = internal::GetControlMode();

    if (controlMode == Control::Auto)
    {
    }

    if (controlMode == Control::Force)
    {
    }

    return os;
}

inline void SetControlMode(Control value) noexcept
{
    internal::GetControlMode() = value;
}

inline void SetWinTermMode(WinTerm value) noexcept
{
    internal::GetWinTermMode() = value;
}

}  // namespace styler

#endif