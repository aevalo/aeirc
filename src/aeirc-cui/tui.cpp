//
// aeirc CUI library
//
// Implementation
//


#include <ctype.h>
#define PDC_DLL_BUILD
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#if defined(__unix) && !defined(__DJGPP__)
#include <unistd.h>
#endif
#include "tui.hpp"
#include "tui_internal.hpp"


namespace aeirc
{
  namespace tui
  {
    void statusmsg(char*);
    int waitforkey();
    void rmerror();

    void clsbody()
    {
      werase(wbody);
      wmove(wbody, 0, 0);
    }

    int bodylen()
    {
#ifdef PDCURSES
      return getmaxy(wbody);
#else
      int maxy, maxx;
      getmaxyx(wbody, maxy, maxx);
      return maxy;
#endif
    }

    WINDOW* bodywin()
    {
      return wbody;
    }

    void rmerror()
    {
      internal::rmline(wstat, 0);
    }

    void rmstatus()
    {
      internal::rmline(wstat, 1);
    }

    void titlemsg(char* msg)
    {
      mvwaddstr(wtitl, 0, 2, internal::padstr(msg, bw - 3));
      wrefresh(wtitl);
    }

    void bodymsg(char* msg)
    {
      waddstr(wbody, msg);
      wrefresh(wbody);
    }

    void errormsg(char* msg)
    {
      beep();
      mvwaddstr(wstat, 0, 2, internal::padstr(msg, bw - 3));
      wrefresh(wstat);
    }

    void statusmsg(char* msg)
    {
      mvwaddstr(wstat, 1, 2, internal::padstr(msg, bw - 3));
      wrefresh(wstat);
    }

    bool keypressed()
    {
      ch = wgetch(wbody);
      return ch != ERR;
    }

    int getkey()
    {
      int c = ch;
      ch = ERR;
#ifdef ALT_X
      quit = (c == ALT_X);    // PC only !
#endif
      return c;
    }

    int waitforkey()
    {
      do internal::idle(); while (!keypressed());
      return getkey();
    }

    void DoExit()   // terminate program
    {
      quit = TRUE;
    }

    void domenu(menu* mp)
    {
      int y, x, nitems, barlen, mheight, mw, old = -1, cur = 0, cur0;
      bool stop = FALSE;
      WINDOW* wmenu;

      curs_set(0);
      internal::getmenupos(&y, &x);
      internal::menudim(mp, &nitems, &barlen);
      mheight = nitems + 2;
      mw = barlen + 2;
      wmenu = newwin(mheight, mw, y, x);
      internal::colorbox(wmenu, SUBMENUCOLOR, 1);
      internal::repaintmenu(wmenu, mp);

      key = ERR;
      while (!stop && !quit)
      {
        if (cur != old)
        {
          if (old != -1)
            mvwaddstr(wmenu, old + 1, 1, internal::prepad(internal::padstr(mp[old].name, barlen - 1), 1));

          internal::setcolor(wmenu, SUBMENUREVCOLOR);
          mvwaddstr(wmenu, cur + 1, 1, internal::prepad(internal::padstr(mp[cur].name, barlen - 1), 1));

          internal::setcolor(wmenu, SUBMENUCOLOR);
          statusmsg(mp[cur].desc);

          old = cur;
          wrefresh(wmenu);
        }

        switch (key = ((key != ERR) ? key : waitforkey()))
        {
          case '\n':          // menu item selected
            touchwin(wbody);
            wrefresh(wbody);
            internal::setmenupos(y + 1, x + 1);
            rmerror();

            key = ERR;
            curs_set(1);
            (mp[cur].func)();   // perform function
            curs_set(0);

            internal::repaintmenu(wmenu, mp);

            old = -1;
            break;

          case KEY_UP:
            cur = (cur + nitems - 1) % nitems;
            key = ERR;
            break;

          case KEY_DOWN:
            cur = (cur + 1) % nitems;
            key = ERR;
            break;

          case KEY_ESC:
          case KEY_LEFT:
          case KEY_RIGHT:
            if (key == KEY_ESC)
              key = ERR;  // return to prev submenu

            stop = TRUE;
            break;

          default:
            cur0 = cur;
            do
            {
              cur = (cur + 1) % nitems;
            } while ((cur != cur0) && (internal::hotkey(mp[cur].name) != toupper((int)key)));
            key = (internal::hotkey(mp[cur].name) == toupper((int)key)) ? '\n' : ERR;
        }
      }

      rmerror();
      delwin(wmenu);
      touchwin(wbody);
      wrefresh(wbody);
    }

    void startmenu(menu* mp, char* mtitle)
    {
      initscr();
      incurses = TRUE;
      internal::initcolor();

      wtitl = subwin(stdscr, th, bw, 0, 0);
      wmain = subwin(stdscr, mh, bw, th, 0);
      wbody = subwin(stdscr, bh, bw, th + mh, 0);
      wstat = subwin(stdscr, sh, bw, th + mh + bh, 0);

      internal::colorbox(wtitl, TITLECOLOR, 0);
      internal::colorbox(wmain, MAINMENUCOLOR, 0);
      internal::colorbox(wbody, BODYCOLOR, 0);
      internal::colorbox(wstat, STATUSCOLOR, 0);

      if (mtitle)
        titlemsg(mtitle);

      cbreak();              // direct input (no newline required)...
      noecho();              // ... without echoing
      curs_set(0);           // hide cursor (if possible)
      nodelay(wbody, TRUE);  // don't wait for input...
      halfdelay(10);         // ...well, no more than a second, anyway
      keypad(wbody, TRUE);   // enable cursor keys
      scrollok(wbody, TRUE); // enable scrolling in main window

      leaveok(stdscr, TRUE);
      leaveok(wtitl, TRUE);
      leaveok(wmain, TRUE);
      leaveok(wstat, TRUE);

      internal::mainmenu(mp);

      internal::cleanup();
    }

    static void repainteditbox(WINDOW* win, int x, char* buf)
    {
#ifndef PDCURSES
      int maxy;
#endif
      int maxx;

#ifdef PDCURSES
      maxx = getmaxx(win);
#else
      getmaxyx(win, maxy, maxx);
#endif
      werase(win);
      mvwprintw(win, 0, 0, "%s", internal::padstr(buf, maxx));
      wmove(win, 0, x);
      wrefresh(win);
    }

    //
    //    weditstr()     - edit string
    //
    //  Description:
    //    The initial value of 'str' with a maximum length of 'field' - 1,
    //    which is supplied by the calling routine, is editted. The user's
    //    erase (^H), kill (^U) and delete word (^W) chars are interpreted.
    //    The PC insert or Tab keys toggle between insert and edit mode.
    //    Escape aborts the edit session, leaving 'str' unchanged.
    //    Enter, Up or Down Arrow are used to accept the changes to 'str'.
    //    NOTE: editstr(), mveditstr(), and mvweditstr() are macros.
    //
    //  Return Value:
    //    Returns the input terminating character on success (Escape,
    //    Enter, Up or Down Arrow) and ERR on error.
    //
    //  Errors:
    //    It is an error to call this function with a NULL window pointer.
    //    The length of the initial 'str' must not exceed 'field' - 1.
    //
    int weditstr(WINDOW* win, char* buf, int field)
    {
      char org[MAXSTRLEN], *tp, *bp = buf;
      bool defdisp = TRUE, stop = FALSE, insert = FALSE;
      int cury, curx, begy, begx;
      chtype oldattr;
      WINDOW* wedit;
      int c = 0;

      if ((field >= MAXSTRLEN) || (buf == NULL) || ((int)strlen(buf) > field - 1))
        return ERR;

      strcpy(org, buf);   // save original

      wrefresh(win);
      getyx(win, cury, curx);
      getbegyx(win, begy, begx);

      wedit = subwin(win, 1, field, begy + cury, begx + curx);
      oldattr = wedit->_attrs;
      internal::colorbox(wedit, EDITBOXCOLOR, 0);

      keypad(wedit, TRUE);
      curs_set(1);

      while (!stop)
      {
        internal::idle();
        repainteditbox(wedit, bp - buf, buf);

        switch (c = wgetch(wedit))
        {
          case ERR:
            break;

          case KEY_ESC:
            strcpy(buf, org);   // restore original
            stop = TRUE;
            break;

          case '\n':
          case KEY_UP:
          case KEY_DOWN:
            stop = TRUE;
            break;

          case KEY_LEFT:
            if (bp > buf)
              bp--;
            break;

          case KEY_RIGHT:
            defdisp = FALSE;
            if (bp - buf < (int)strlen(buf))
              bp++;
            break;

          case '\t':            // TAB -- because insert is broken on HPUX
          case KEY_IC:          // enter insert mode
          case KEY_EIC:         // exit insert mode
            defdisp = FALSE;
            insert = !insert;
            curs_set(insert ? 2 : 1);
            break;

          default:
            if (c == erasechar())       // backspace, ^H
            {
              if (bp > buf)
              {
                memmove((void *)(bp - 1), (const void *)bp, strlen(bp) + 1);
                bp--;
              }
            }
            else if (c == killchar())   // ^U
            {
              bp = buf;
              *bp = '\0';
            }
            else if (c == wordchar())   // ^W
            {
              tp = bp;
              while ((bp > buf) && (*(bp - 1) == ' '))
                bp--;
              while ((bp > buf) && (*(bp - 1) != ' '))
                bp--;
              memmove((void *)bp, (const void *)tp, strlen(tp) + 1);
            }
            else if (isprint(c))
            {
              if (defdisp)
              {
                bp = buf;
                *bp = '\0';
                defdisp = FALSE;
              }
              if (insert)
              {
                if ((int)strlen(buf) < field - 1)
                {
                  memmove((void *)(bp + 1), (const void *)bp, strlen(bp) + 1);
                  *bp++ = c;
                }
              }
              else if (bp - buf < field - 1)
              {
                // append new string terminator
                if (!*bp)
                  bp[1] = '\0';

                *bp++ = c;
              }
            }
        }
      }

      curs_set(0);

      wattrset(wedit, oldattr);
      repainteditbox(wedit, bp - buf, buf);
      delwin(wedit);

      return c;
    }

    WINDOW* winputbox(WINDOW* win, int nlines, int ncols)
    {
      WINDOW* winp;
      int cury, curx, begy, begx;

      getyx(win, cury, curx);
      getbegyx(win, begy, begx);

      winp = newwin(nlines, ncols, begy + cury, begx + curx);
      internal::colorbox(winp, INPUTBOXCOLOR, 1);

      return winp;
    }

    int getstrings(char* desc[], char* buf[], int field)
    {
      WINDOW *winput;
      int oldy, oldx, maxy, maxx, nlines, ncols, i, n, l, mmax = 0;
      int c = 0;
      bool stop = FALSE;

      for (n = 0; desc[n]; n++)
        if ((l = strlen(desc[n])) > mmax)
          mmax = l;

      nlines = n + 2; ncols = mmax + field + 4;
      getyx(wbody, oldy, oldx);
      getmaxyx(wbody, maxy, maxx);

      winput = mvwinputbox(wbody, (maxy - nlines) / 2, (maxx - ncols) / 2, nlines, ncols);

      for (i = 0; i < n; i++)
        mvwprintw(winput, i + 1, 2, "%s", desc[i]);

      i = 0;

      while (!stop)
      {
        switch (c = mvweditstr(winput, i+1, mmax+3, buf[i], field))
        {
          case KEY_ESC:
            stop = TRUE;
            break;

          case KEY_UP:
            i = (i + n - 1) % n;
            break;

          case '\n':
          case '\t':
          case KEY_DOWN:
            if (++i == n)
              stop = TRUE;    // all passed?
        }
      }

      delwin(winput);
      touchwin(wbody);
      wmove(wbody, oldy, oldx);
      wrefresh(wbody);

      return c;
    }
  }
}
