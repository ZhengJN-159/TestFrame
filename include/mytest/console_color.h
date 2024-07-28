#ifndef MYTEST_CONSOLE_COLOR_H
#define MYTEST_CONSOLE_COLOR_H

#ifdef _WIN32
#include <windows.h>
void setConsoleColor(WORD color);
#else
void setConsoleColor(const std::string& colorCode);
#endif

#endif // MYTEST_CONSOLE_COLOR_H
