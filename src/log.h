#pragma once

// the individual levels of logging
extern enum log_level
{
    LOG_VERBOSE,
    LOG_INFO,
    LOG_BATCH,
    LOG_WARN,
    LOG_ERROR,
    LOG_QUIET
} LOG_LEVEL;

// some output codes for Unix-like terminals
#define LOG_BOLD "\033[1m"
#define LOG_DARK "\033[2m"
#define LOG_RESET "\033[0m"

// set the log level
void log_level(enum log_level);

// print a verbose message
void verbose(const char* msg, ...);

// print an informative message
void info(const char* msg, ...);

// print a warning message
void warn(const char* msg, ...);

// print an error message and exit
void error(const char* msg, ...);

// print an error message in a file and exit
void errorf(const char* file, size_t line, const char* msg, ...);

// print an internal error message and exit
void errori(const char* msg, ...);

// print an internal error message in a file and exit
void errorfi(const char* file, size_t line, const char* msg, ...);

// mute standard output
void mute();

// unmute standard output
void unmute();
