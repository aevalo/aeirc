//
// aeirc CUI library
//
// Implementation - internals
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
#include "tui_internal.hpp"


namespace aeirc
{
  namespace tui
  {
    namespace internal
    {
      int nexty, nextx;

#ifndef PDCURSES
      char wordchar()
      {
        return 0x17;    // ^W
      }
#endif

      char* padstr(char* s, int length)
      {
        static char buf[MAXSTRLEN];
        char fmt[10];
        sprintf(fmt, (int)strlen(s) > length ? "%%.%ds" : "%%-%ds", length);
        sprintf(buf, fmt, s);
        return buf;
      }

      char* prepad(char* s, int length)
      {
        int i;
        char* p = s;
        if (length > 0)
        {
          memmove((void *)(s + length), (const void *)s, strlen(s) + 1);
          for (i = 0; i < length; i++)
            *p++ = ' ';
        }
        return s;
      }

      void rmline(WINDOW* win, int nr)   // keeps box lines intact
      {
        mvwaddstr(win, nr, 1, padstr(" ", bw - 2));
        wrefresh(win);
      }

      void initcolor()
      {
  #ifdef A_COLOR
        if (has_colors())
          start_color();

        // foreground, background
        init_pair(TITLECOLOR       & ~A_ATTR, COLOR_BLACK, COLOR_CYAN);
        init_pair(MAINMENUCOLOR    & ~A_ATTR, COLOR_WHITE, COLOR_CYAN);
        init_pair(MAINMENUREVCOLOR & ~A_ATTR, COLOR_WHITE, COLOR_BLACK);
        init_pair(SUBMENUCOLOR     & ~A_ATTR, COLOR_WHITE, COLOR_CYAN);
        init_pair(SUBMENUREVCOLOR  & ~A_ATTR, COLOR_WHITE, COLOR_BLACK);
        init_pair(BODYCOLOR        & ~A_ATTR, COLOR_WHITE, COLOR_BLUE);
        init_pair(STATUSCOLOR      & ~A_ATTR, COLOR_WHITE, COLOR_CYAN);
        init_pair(INPUTBOXCOLOR    & ~A_ATTR, COLOR_BLACK, COLOR_CYAN);
        init_pair(EDITBOXCOLOR     & ~A_ATTR, COLOR_WHITE, COLOR_BLACK);
  #endif
      }

      void setcolor(WINDOW* win, chtype color)
      {
        chtype attr = color & A_ATTR;  // extract Bold, Reverse, Blink bits

  #ifdef A_COLOR
        attr &= ~A_REVERSE;  // ignore reverse, use colors instead!
        wattrset(win, COLOR_PAIR(color & A_CHARTEXT) | attr);
  #else
        attr &= ~A_BOLD;     // ignore bold, gives messy display on HP-UX
        wattrset(win, attr);
  #endif
      }

      void colorbox(WINDOW* win, chtype color, int hasbox)
      {
        int maxy;
  #ifndef PDCURSES
        int maxx;
  #endif
        chtype attr = color & A_ATTR;  // extract Bold, Reverse, Blink bits

        setcolor(win, color);

  #ifdef A_COLOR
        if (has_colors())
          wbkgd(win, COLOR_PAIR(color & A_CHARTEXT) | (attr & ~A_REVERSE));
        else
  #endif
          wbkgd(win, attr);

        werase(win);

  #ifdef PDCURSES
        maxy = getmaxy(win);
  #else
        getmaxyx(win, maxy, maxx);
  #endif
        if (hasbox && (maxy > 2))
          box(win, 0, 0);

        touchwin(win);
        wrefresh(win);
      }

      void idle()
      {
        char buf[MAXSTRLEN];
        time_t t;
        struct tm* tp;

        if (time (&t) == -1)
          return;  // time not available

        tp = localtime(&t);
        sprintf(buf, " %.2d-%.2d-%.4d  %.2d:%.2d:%.2d",
                tp->tm_mday, tp->tm_mon + 1, tp->tm_year + 1900,
                tp->tm_hour, tp->tm_min, tp->tm_sec);

        mvwaddstr(wtitl, 0, bw - strlen(buf) - 2, buf);
        wrefresh(wtitl);
      }

      void menudim(menu* mp, int* lines, int* columns)
      {
        int n, l, mmax = 0;

        for (n=0; mp->func; n++, mp++)
          if ((l = strlen(mp->name)) > mmax) mmax = l;

        *lines = n;
        *columns = mmax + 2;
      }

      void setmenupos(int y, int x)
      {
        nexty = y;
        nextx = x;
      }

      void getmenupos(int* y, int* x)
      {
        *y = nexty;
        *x = nextx;
      }

      int hotkey(const char* s)
      {
        int c0 = *s;    // if no upper case found, return first char

        for (; *s; s++)
          if (isupper((unsigned char)*s))
            break;

        return *s ? *s : c0;
      }

      void repaintmenu(WINDOW* wmenu, menu* mp)
      {
        int i;
        menu* p = mp;

        for (i = 0; p->func; i++, p++)
          mvwaddstr(wmenu, i + 1, 2, p->name);

        touchwin(wmenu);
        wrefresh(wmenu);
      }

      void repaintmainmenu(int width, menu* mp)
      {
        int i;
        menu* p = mp;

        for (i = 0; p->func; i++, p++)
          mvwaddstr(wmain, 0, i * width, prepad(padstr(p->name, width - 1), 1));

        touchwin(wmain);
        wrefresh(wmain);
      }

      void mainhelp()
      {
  #ifdef ALT_X
        statusmsg("Use arrow keys and Enter to select (Alt-X to quit)");
  #else
        statusmsg("Use arrow keys and Enter to select");
  #endif
      }

      void mainmenu(menu* mp)
      {
        int nitems, barlen, old = -1, cur = 0, c, cur0;

        menudim(mp, &nitems, &barlen);
        repaintmainmenu(barlen, mp);

        while (!quit)
        {
          if (cur != old)
          {
            if (old != -1)
            {
              mvwaddstr(wmain, 0, old * barlen, prepad(padstr(mp[old].name, barlen - 1), 1));
              statusmsg(mp[cur].desc);
            }
            else
              mainhelp();

            setcolor(wmain, MAINMENUREVCOLOR);

            mvwaddstr(wmain, 0, cur * barlen, prepad(padstr(mp[cur].name, barlen - 1), 1));

            setcolor(wmain, MAINMENUCOLOR);
            old = cur;
            wrefresh(wmain);
          }

          switch (c = (key != ERR ? key : waitforkey()))
          {
            case KEY_DOWN:
            case '\n':              // menu item selected
              touchwin(wbody);
              wrefresh(wbody);
              rmerror();
              setmenupos(th + mh, cur * barlen);
              curs_set(1);
              (mp[cur].func)();   // perform function
              curs_set(0);

              switch (key)
              {
                case KEY_LEFT:
                  cur = (cur + nitems - 1) % nitems;
                  key = '\n';
                  break;

                case KEY_RIGHT:
                  cur = (cur + 1) % nitems;
                  key = '\n';
                  break;

                default:
                  key = ERR;
              }

              repaintmainmenu(barlen, mp);
              old = -1;
              break;

            case KEY_LEFT:
              cur = (cur + nitems - 1) % nitems;
              break;

            case KEY_RIGHT:
              cur = (cur + 1) % nitems;
              break;

            case KEY_ESC:
              mainhelp();
              break;

            default:
              cur0 = cur;

              do
              {
                cur = (cur + 1) % nitems;
              } while ((cur != cur0) && (hotkey(mp[cur].name) != toupper(c)));

              if (hotkey(mp[cur].name) == toupper(c))
                key = '\n';
          }
        }

        rmerror();
        touchwin(wbody);
        wrefresh(wbody);
      }

      void cleanup()   // cleanup curses settings
      {
        if (incurses)
        {
          delwin(wtitl);
          delwin(wmain);
          delwin(wbody);
          delwin(wstat);
          curs_set(1);
          endwin();
          incurses = FALSE;
        }
      }
    }
  }
}
