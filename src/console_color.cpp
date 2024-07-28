#include "../include/mytest/console_color.h"

#ifdef _WIN32
void setConsoleColor(WORD color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}
#else
#include <iostream>
void setConsoleColor(const std::string& colorCode) {
    std::cout << colorCode;
}
#endif
