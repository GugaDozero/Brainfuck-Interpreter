#ifndef UTILS_H
#define UTILS_H

#if defined(_WIN32) || defined(WIN32)

    #include <conio.h>

    #define INIT()
    #define FINISH()

    #define PUTCHAR(ch) putchar(ch)
    #define PRINT_ERR(str, err) std::cerr << str << err.what() << std::flush
    #define PRINT(str) std::cout << str << std::flush

#elif defined(linux) || defined(__unix__)

    #include <curses.h>
    #include <stdio_ext.h>
    #include <cstdlib> // __fpurge

    #define INIT() initscr()
    #define FINISH() \
        getch(); \
        endwin()

    #define PUTCHAR(ch) \
        addch(m_cell[m_cx]); \
        refresh();

    #define PRINT_ERR(str, err) \
        printw(std::string(str + "%s").c_str(), err.what()); \
        refresh();

    #define PRINT(str) \
        printw(str.c_str()); \
        refresh();

#else

    #line 162
    #error "No library found to perform getch() on this platform."

#endif

/* My Itoa Implementation */

extern void itoa( signed num, char *buff );

#endif // UTILS_H
