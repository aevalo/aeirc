//
// aeirc CUI library
//
// Header file
//
// Based in TUI example from PDCurses
//

#ifndef AEIRC_TUI_HPP_INCLUDED
#define AEIRC_TUI_HPP_INCLUDED

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

    const int MAXSTRLEN = 256;
    const char KEY_ESC  = 0x1b;     // Escape

    typedef void (*FUNC)();

    typedef struct
    {
      char* name; // item label
      FUNC  func; // (pointer to) function
      char* desc; // function description
    } menu;

    // ANSI C function prototypes:
    void    clsbody();
    int     bodylen();
    WINDOW* bodywin();

    void    rmerror();
    void    rmstatus();

    void    titlemsg(char* msg);
    void    bodymsg(char* msg);
    void    errormsg(char* msg);
    void    statusmsg(char* msg);

    bool    keypressed();
    int     getkey();
    int     waitforkey();

    void    DoExit();
    void    startmenu(menu* mp, char* title);
    void    domenu(menu* mp);

    int     weditstr(WINDOW* win, char* buf, int field);
    WINDOW* winputbox(WINDOW* win, int nlines, int ncols);
    int     getstrings(char* desc[], char* buf[], int field);

    // Check the functions below, wether they are usefull
    inline int editstr(char* s, int f) {return weditstr(stdscr,s,f);}
    inline int mveditstr(int y, int x, char* s, int f)     {return (move(y,x)==ERR?ERR:editstr(s,f));}
    inline int mvweditstr(WINDOW* w, int y, int x, char* s, int f)  {return (wmove(w,y,x)==ERR?ERR:weditstr(w,s,f));}

    inline WINDOW* inputbox(int l, int c)          {return winputbox(stdscr,l,c);}
    inline WINDOW* mvinputbox(int y, int x, int l, int c)    {return (move(y,x)==ERR?w:inputbox(l,c));}
    inline WINDOW* mvwinputbox(WINDOW* w, int y, int x, int l, int c) {return (wmove(w,y,x)==ERR?w:winputbox(w,l,c));}
  }
}

#endif // AEIRC_TUI_HPP_INCLUDED
