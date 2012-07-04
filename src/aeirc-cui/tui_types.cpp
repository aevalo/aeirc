//
// aeirc CUI library
//
// Implementation
//
// Types
//

#define PDC_DLL_BUILD
#include <curses.h>
#include "tui_types.hpp"

namespace aeirc
{
  namespace tui
  {
    WINDOW *wtitl, *wmain, *wbody, *wstat; // title, menu, body, status win
    int key = ERR, ch = ERR;
    bool quit = FALSE;
    bool incurses = FALSE;
  }
}
