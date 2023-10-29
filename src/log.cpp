#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include "log.h"

cin::log::Level g_level{cin::log::Level::info};

namespace
{
    enum class Color {
        blue,
        green,
        yellow,
        red,
    };

    void print_colored(std::FILE *fp, Color color, const char* s)
    {
        const bool is_tty{isatty(fp == stdout ? 1 : 2) == 1};

        if (is_tty) {
            switch (color) {
                case Color::blue:
                    fmt::print(fp, "\u001b[34m");
                    break;
                case Color::green:
                    fmt::print(fp, "\u001b[32m");
                    break;
                case Color::yellow:
                    fmt::print(fp, "\u001b[33m");
                    break;
                case Color::red:
                    fmt::print(fp, "\u001b[31m");
                    break;
            }
        }

        fmt::print(fp, s);

        if (is_tty) {
            fmt::print(fp, "\u001b[0m");
        }
    }
}

void cin::log::init()
{
    const char *var{std::getenv("CINEMO_LOG")};

    if (var == nullptr) {
        return;
    }

    if (std::strcmp(var, "debug") == 0) {
        g_level = Level::debug;
    }
    else if (std::strcmp(var, "info") == 0) {
        g_level = Level::info;
    }
    else if (std::strcmp(var, "warn") == 0) {
        g_level = Level::warning;
    }
    else if (std::strcmp(var, "error") == 0) {
        g_level = Level::error;
    }
}

void cin::log::detail::log(Level level, fmt::string_view format, fmt::format_args args)
{
    if (level < g_level) {
        return;
    }

    switch (level) {
        case Level::debug:
            print_colored(stdout, Color::blue, "DEBUG ");
            fmt::vprint(format, args);
            break;
        case Level::info:
            print_colored(stdout, Color::green, "INFO  ");
            fmt::vprint(format, args);
            break;
        case Level::warning:
            print_colored(stderr, Color::yellow, "WARN  ");
            fmt::vprint(stderr, format, args);
            break;
        case Level::error:
            print_colored(stderr, Color::red, "ERROR ");
            fmt::vprint(stderr, format, args);
            break;
    }

    fmt::print("\n");
}
