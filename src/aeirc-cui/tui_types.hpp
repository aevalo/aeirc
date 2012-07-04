//
// aeirc CUI library
//
// Header file
//
// Types
//

#ifndef AEIRC_TUI_TYPES_HPP_INCLUDED
#define AEIRC_TUI_TYPES_HPP_INCLUDED

#include <curses.h>

namespace aeirc
{
  namespace tui
  {
    #ifdef A_COLOR
    #define A_ATTR  (A_ATTRIBUTES ^ A_COLOR)  // A_BLINK, A_REVERSE, A_BOLD
    #else
    #define A_ATTR  (A_ATTRIBUTES)            // standard UNIX attributes
    #endif

    #ifdef A_COLOR
    # define TITLECOLOR       1       /* color pair indices */
    # define MAINMENUCOLOR    (2 | A_BOLD)
    # define MAINMENUREVCOLOR (3 | A_BOLD | A_REVERSE)
    # define SUBMENUCOLOR     (4 | A_BOLD)
    # define SUBMENUREVCOLOR  (5 | A_BOLD | A_REVERSE)
    # define BODYCOLOR        6
    # define STATUSCOLOR      (7 | A_BOLD)
    # define INPUTBOXCOLOR    8
    # define EDITBOXCOLOR     (9 | A_BOLD | A_REVERSE)
    #else
    # define TITLECOLOR       0       /* color pair indices */
    # define MAINMENUCOLOR    (A_BOLD)
    # define MAINMENUREVCOLOR (A_BOLD | A_REVERSE)
    # define SUBMENUCOLOR     (A_BOLD)
    # define SUBMENUREVCOLOR  (A_BOLD | A_REVERSE)
    # define BODYCOLOR        0
    # define STATUSCOLOR      (A_BOLD)
    # define INPUTBOXCOLOR    0
    # define EDITBOXCOLOR     (A_BOLD | A_REVERSE)
    #endif

    #define th 1     /* title window height */
    #define mh 1     /* main menu height */
    #define sh 2     /* status window height */
    #define bh (LINES - th - mh - sh)   /* body window height */
    #define bw COLS  /* body window width */

    const int MAXSTRLEN = 256;
    const char KEY_ESC  = 0x1b;     // Escape

    typedef void (*FUNC)();

    typedef struct
    {
      char* name; // item label
      FUNC  func; // (pointer to) function
      char* desc; // function description
    } menu;

    extern WINDOW *wtitl, *wmain, *wbody, *wstat; // title, menu, body, status win
    extern int key, ch;
    extern bool quit;
    extern bool incurses;
  }
}

#endif // AEIRC_TUI_TYPES_HPP_INCLUDED
