//
// aeirc CUI library
//
// Header - internals
//

#ifndef AEIRC_TUI_INTERNAL_HPP_INCLUDED
#define AEIRC_TUI_INTERNAL_HPP_INCLUDED

#include <curses.h>
#include "tui_types.hpp"
#include "tui.hpp"

namespace aeirc
{
  namespace tui
  {
    namespace internal
    {
#ifndef PDCURSES
      char wordchar();
#endif
      char* padstr(char* s, int length);
      char* prepad(char* s, int length);
      void rmline(WINDOW* win, int nr);
      void initcolor();
      void setcolor(WINDOW* win, chtype color);
      void colorbox(WINDOW* win, chtype color, int hasbox);
      void idle();
      void menudim(menu* mp, int* lines, int* columns);
      void setmenupos(int y, int x);
      void getmenupos(int* y, int* x);
      int hotkey(const char* s);
      void repaintmenu(WINDOW* wmenu, menu* mp);
      void repaintmainmenu(int width, menu* mp);
      void mainhelp();
      void mainmenu(menu* mp);
      void cleanup();
    }
  }
}


#endif // AEIRC_TUI_INTERNAL_HPP_INCLUDED
