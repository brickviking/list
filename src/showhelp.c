#include <stdio.h>
#include <stdlib.h>
#ifndef _MINIX  /* because MINIX hasn't got a basename() */
#include <libgen.h> /* ...but everything else I know, has a basename() */
#endif 
#include <ctype.h>
#include <curses.h>
#ifndef __DOS__
#include <menu.h>  /* not strictly necessary yet */
#include <dirent.h>
#else
#include <direct.h>
#endif
#include <sys/stat.h>
#include "list.h"

/* Split off from screen.c for the moment to see if it compiles separately */
void ShowHelp(struct FileData *fi) {
/* char tmpstr[COLS+1]; */
  int c, middle, x, y, EightBitDisplay;
  WINDOW *HelpWin;
  char *tempstring = (char *) malloc(sizeof(tempstring) * 1024);  /* To give us some room */
  if (tempstring == NULL)
    /* We should be failing if we can't malloc here */
    Bye(BR_NOMEM, __LINE__);
  if(fi->Scrn_x < 80 || fi->Scrn_y < 25) { /* This shouldn't be a show stopper */
    debug_function("Cannot display help - screen too small", 0, fi->Scrn_y, __LINE__);
    refresh();
    return;
  }
/* LINES is supplied by ncurses, but this would change on a SIGWINCH */
  middle = (((fi->Scrn_y > 24) ? fi->Scrn_y : 24) / 2) ;
/* Falls over if LINES is < 24, so we provide some protection */
  x = 0; y = 0;
  HelpWin = newwin(24, 80, (middle - 12), 0);
/*  wclear(HelpWin); */
  if(!fi->Monochrome) {
    wbkgd(HelpWin, ' ' | COLOR_PAIR(COLOR_BLUE));
  }
  else {
    wbkgd(HelpWin, A_BOLD);
  }
#ifdef __linux__
 /* I really really shouldn't have to do this... but BSD being what it is... */
  if(getenv("DISPLAY") ==  NULL)
    EightBitDisplay=1;
  else
#endif
    EightBitDisplay = 0; /* This is to sort out those cases in which I don't
                 * have Linux on board - it's a safety measure */
  if(EightBitDisplay)
    if(!fi->Monochrome) {
      wattron(HelpWin, A_ALTCHARSET | COLOR_PAIR(COLOR_YELLOW) | A_BOLD);
    }
    else {  /* We're in mono */
      wattron(HelpWin, A_ALTCHARSET |  A_BOLD);
    }
  else { /* Can't use ALT_CHARSET */
    if(!fi->Monochrome) {
      wattron(HelpWin, COLOR_PAIR(COLOR_YELLOW) | A_BOLD);
    }
    else {  /* We're in mono */
      wattron(HelpWin,  A_BOLD);
    }
  }
/* All these wmove/waddstr's can be replaced by a single call! *
 * mvwaddstr(WIN, y, x, string) */
/*  wmove(HelpWin, 0,18); */
  /*   standout(); */
  x = 1; /* Starts "list" in the middle, increment y each line */
  if(EightBitDisplay) {
    mvwaddstr(HelpWin, y++, x, "                 °°   °°                °°                                  ");  /* Ln 0 */
    mvwaddstr(HelpWin, y++, x, "                 °°                     °°                                  ");  /* Ln 1 */
    mvwaddstr(HelpWin, y++, x, "                 ±±   ±±   Ü±±±±±±Ü  ±±±±±±±±         By                    ");  /* Ln 2 */
    mvwaddstr(HelpWin, y++, x, "                 ±±   ±±   ±      ß     ±±       brickviking             ");  /* Ln 3 */
    mvwaddstr(HelpWin, y++, x, "                 ²²   ²²    ²²²²²²      ²²                                  ");  /* Ln 4 */
    mvwaddstr(HelpWin, y++, x, "                 ²²   ²²   Ü      ²     ²²                                  ");  /* Ln 5 */
    mvwaddstr(HelpWin, y++, x, "                 ÛÛ   ÛÛ   ßÛÛÛÛÛÛß     ÛÛ                                  ");  /* Ln 6 */
    mvwaddstr(HelpWin, y++, x, "                 ÛÛ                                                         ");  /* Ln 7 */
    mvwaddstr(HelpWin, y, x,   "                 ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ                               ");  /* Ln 8 */
  }
  else {
    mvwaddstr(HelpWin, y++, x, "   aawaawaa             _aasasa.                       .wwwaww,            ");
    mvwaddstr(HelpWin, y++, x, "   mWWWQWW#    By       jQWWWWW:                       :QWWWQW(            ");
    mvwaddstr(HelpWin, y++, x, "   mQQQQQQ#             )TY?Y?Y`                       :QQQQQQ(            ");
    mvwaddstr(HelpWin, y++, x, "   mQQQQQQ#             _______       ._aaaaas,,.    wawQQQQQQwaac         ");
    mvwaddstr(HelpWin, y++, x, "   mQQQQQQ# brickviking jQQQQQQ;    <yQQQWWWWWQQQw,  QWWQQQQQQQQQk         ");
    mvwaddstr(HelpWin, y++, x, "   mQQQQQQ#             jQQQQQQ;   jWQQQQW\?\?YQWQWWQ, ^~)QQQQQQr~^`         ");
    mvwaddstr(HelpWin, y++, x, "   mQQQQQQ#             jQQQQQQ;   mWQQQQQws,==----    :QQQQQQ(            ");
    mvwaddstr(HelpWin, y++, x, "   mQQQQQQ#             jQQQQQQ;   ]QQQQQQWWQQQmwa,    :QQQQQQ(            ");
    mvwaddstr(HelpWin, y++, x, "   mQQQQQQ#             jQQQQQQ;    -?YUWWQQQQQQQQQw   :QQQQQQ(            ");
    mvwaddstr(HelpWin, y++, x, "   mQQQQQQ#             jQQQQQQ;  __i___s ~\"?QWQQQQQ   :QQQQQQ[   ._,      ");
    mvwaddstr(HelpWin, y++, x, "   mQQQQQQQaaaawaaaaw,  jQQQQQQ;  -QQQQQQga_amQQQQQD   :QQQQQQmawmQQQg     ");
    mvwaddstr(HelpWin, y++, x, "   mQQQQQQWQQQQWWQQQW(  jQQQQQQ;   \"9QQQQQWQQQQQQ@!`   -$QQQQQQWWQQQQ\?     ");
    mvwaddstr(HelpWin, y++, x, "   \?\?\?\?\?\?\?\?\?\?\?\?\?\?\?\?\?\?'  )!!!!!!`     -~\"\?\?T\?T\?!\"`        ~\"!\?\?\?\?\?\?\?\"`     ");
                                                                 
  }
/*
  else {
    mvwaddstr(HelpWin, y++, x, "                 ..   ..                ..                                  ");         / * Ln 0 * /
    mvwaddstr(HelpWin, y++, x, "                 ::                     ::                                  ");        / * Ln 1 * /
    mvwaddstr(HelpWin, y++, x, "                 %%   ;;    ;;;;;;   ;;;;;;;;         By                    "); / * Ln 2 * /
    mvwaddstr(HelpWin, y++, x, "                 %%   %%   ;      '     %%          brickviking             "); / * Ln 3 * /
    mvwaddstr(HelpWin, y++, x, "                 &&   %%    %%%%%%      %%                                  "); / * Ln 4 * /
    mvwaddstr(HelpWin, y++, x, "                 &&   &&   ,      :     &&                                  "); / * Ln 5 * /
    mvwaddstr(HelpWin, y++, x, "                 ##   ##    ######      ##                                  "); / * Ln 6 * /
    mvwaddstr(HelpWin, y++, x, "                 ##                                                         "); / * Ln 7 * /
    mvwaddstr(HelpWin, y, x,   "                 ############################                               "); / * Ln 8 * /
/ * Gawd, this needs some work! * /
  } End of commented out code */
  /*   standend(); */
  if(!fi->Monochrome) {
    wattroff( HelpWin,  COLOR_PAIR(COLOR_YELLOW)); /* reset attribs to cyan on blue */
    if(EightBitDisplay) {
        wattron( HelpWin, A_ALTCHARSET | COLOR_PAIR(COLOR_CYAN));
      }
    else {
      wattron( HelpWin, COLOR_PAIR(COLOR_CYAN));
    }
  }
  else {
    wattroff( HelpWin, A_BOLD);
  }
  if(debug) {
    y = 13; x = 17;
    sprintf(tempstring, "Debug version %1d.%1d.%1d %s %s build", LISTVERSION, LISTMAJOR, LISTMINOR, __DATE__, __TIME__); /* Ln 10 */
    mvwaddstr(HelpWin, x, y, tempstring);
  }
  x = 5; y = 13;
/* We have a problem: what about screens that are NOT 80x25?  */
/* Here's one solution...keyed off LINES (a constant of ncurses) */
  mvwaddstr(HelpWin, y++, x, "Keys to use within list: (Q to exit help, any other key to continue)"); /* Ln 12 */
  x = 1;   /* reset x to left hand side of the screen plus one to balance */
  if(EightBitDisplay) {
    mvwaddstr(HelpWin, y++, x, "ÚÄÄÄÄÄÂÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÂÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿"); /* Ln 13 */
    mvwaddstr(HelpWin, y++, x, "³ Key ³ Mode ³         Action         ³  Key  ³ Mode ³       Action          ³"); /* Ln 14 */
    mvwaddstr(HelpWin, y++, x, "ÃÄÄÄÄÄÂÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÂÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´"); /* Ln 15 */
    mvwaddstr(HelpWin, y, x,   "³  B  ³ Text ³ Move back one page     ³   B   ³ Hex  ³ Move back");
    sprintf(tempstring, "%-5d", -(fi->Scrn_y - 1 -(ruler)) * fi->ScrnWide);
    x += 64;
    mvwaddstr(HelpWin, y, x, tempstring);
    x += 6;
    mvwaddstr(HelpWin, y++, x,                    " bytes ³");  /* Ln 16 */
    x = 1; /* Reset back to the left of the screen plus one for balancing */
/*    mvwaddstr(HelpWin, x++, y, "³ Up (j) ³  16 bytes   ³    One line backward       ³(E)nd ³    End of file    ³"); / * Ln 17 */
    mvwaddstr(HelpWin, y, x,   "³   Up   ³");
    x += 10;
    sprintf(tempstring, "  %-4d", fi->ScrnWide);
    mvwaddstr(HelpWin, y, x, tempstring);
    x = 17; /* 63 chars from the right hand side */
    mvwaddstr(HelpWin, y++, x, " bytes ³    One line backward       ³(E)nd ³   End of file   ³"); /* Ln 17 */
    x = 1;
    mvwaddstr(HelpWin, y, x, "³  Down  ³");
    x += 10;
    sprintf(tempstring, "  %-4d", fi->ScrnWide);
    mvwaddstr(HelpWin, y, x, tempstring);
    x = 17; /* 63 chars from the right hand side */
    mvwaddstr(HelpWin, y++, x, " bytes ³    One line forward        ³(G)o  ³ Goto byte/line  ³"); /* Ln 18 */
    x = 1;
    mvwaddstr(HelpWin, y, x,   "³ Space  ³ +");
    sprintf(tempstring, "%-5d", (fi->Scrn_y - 1 - (ruler)) * fi->ScrnWide);
    waddstr(HelpWin, tempstring);
    x += 17;
    mvwaddstr(HelpWin, y, x,                    "bytes ³ 1 page (");
    x += 35;
    sprintf(tempstring, "%-4d", (int)fi->Scrn_y - 1 - ruler);
    waddstr(HelpWin, tempstring);
    waddstr(HelpWin,                                               " lines) forward");
    mvwaddstr(HelpWin, y++, x,                                                     "³  %   ³ Goto % of file  ³"); /* Ln 19 */
    x = 1;  /* Reset back to the left of the screen plus one for balancing */
    mvwaddstr(HelpWin, y, x,   "ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ"); /* Ln 25 */
  } /* now, that deals with a non-X screen... what about a screen under X? */
  else { /* Here goes ... */
    mvwaddstr(HelpWin, y++, x, ".________._____________.____________________________.______._________________.");
    mvwaddstr(HelpWin, y++, x, "| Key    |  Hex mode   |       Text mode            | Keys |  Location       |"); /* Ln 14 */
    mvwaddstr(HelpWin, y++, x, "+--------+-------------+----------------------------+------+-----------------+"); /* Ln 15 */
    mvwaddstr(HelpWin, y, x,   "|(B)ackUp|");
    sprintf(tempstring, "%-5d", -(fi->Scrn_y - 1 - (ruler)) * fi->ScrnWide);
    x += 11;
    mvwaddstr(HelpWin, y, x, tempstring);
    x += 5;
    mvwaddstr(HelpWin, y, x,                " bytes | 1 page (");
    x += 17;
    sprintf(tempstring, "%-4d", (int)fi->Scrn_y - 1 - ruler);
    mvwaddstr(HelpWin, y, x, tempstring);
    x += 3;
                                     mvwaddstr(HelpWin, y, x, "lines) backward");
    x += 16;
    mvwaddstr(HelpWin, y++, x,                                                     "|(H)ome|  Start  of file |");
    x = 1; /* Reset back to the left of the screen plus one for balancing */
/*    mvwaddstr(HelpWin, x++, y, "| Up (j) |  16 bytes   |    One line backward       |(E)nd |    End of file    |"); */
/*    mvwaddstr(HelpWin, x++, y, "|Down(k) |  16 bytes   |    One line forward        |(G)o  |  Goto byte/line   |"); */
    mvwaddstr(HelpWin, y, x,   "|   Up   |");
    x += 10;
    sprintf(tempstring, "%-4d", fi->ScrnWide);
    mvwaddstr(HelpWin, y, x, tempstring);
    x = 17; /* 63 chars from the right hand side */
    mvwaddstr(HelpWin, y++, x, " bytes |     One line backward      |(E)nd |   End of file   |"); /* Ln 17 */
    x = 1;
    mvwaddstr(HelpWin, y, x, "|  Down  |");
    x += 10;
    sprintf(tempstring, "  %-4d", fi->ScrnWide);
    mvwaddstr(HelpWin, y, x, tempstring);
    x = 17; /* 63 chars from the right hand side */
    mvwaddstr(HelpWin, y++, x, " bytes |     One line forward       |(G)o  | Goto byte/line  |"); /* Ln 18 */
    x = 1;
    mvwaddstr(HelpWin, y, x,   "| Space  | +");
    x += 12;
    sprintf(tempstring, "%-5d", (fi->Scrn_y - 1 - (ruler)) * fi->ScrnWide);
    mvwaddstr(HelpWin, y, x, tempstring);
    x += 5;
    mvwaddstr(HelpWin, y, x,                   "bytes | 1 page (");
    x += 16;
    sprintf(tempstring, "%-4d", (int)fi->Scrn_y - 1 - ruler);
    mvwaddstr(HelpWin, y, x, tempstring);
    x += 3;
    mvwaddstr(HelpWin, y, x,                                       " lines) forward");
    x += 16;
    mvwaddstr(HelpWin, y++, x,                                                     "|  %   | Goto % of file  |");
    x = 1;  /* Reset back to the left of the screen */
    mvwaddstr(HelpWin, y, x,   "`============================================================================'");
  }
  /* We preferably don't want the help screen itself larger than 24 lines */
  /*  move(24,39);
  addstr("Q to quit help screen"); */
  wrefresh(HelpWin);
  /*  if(FPtr!=stdin) */  /* Hmmm, will the next line work in STDIN mode? */
   if( QUIT_CHAR != ( c = getch())) { /* use this char to decide whether we display the next help screen... */
/*    wclear(HelpWin); */
    if(!fi->Monochrome) {
      if(EightBitDisplay) {
        wattron(HelpWin, A_ALTCHARSET | COLOR_PAIR(COLOR_CYAN) | A_BOLD);
      }
      else {
        wattron(HelpWin, COLOR_PAIR(COLOR_CYAN) | A_BOLD);
      }
    } else {
      if(EightBitDisplay) {
        wattron(HelpWin, A_ALTCHARSET | A_BOLD);
      }
      else {
        wattron(HelpWin,  A_BOLD);
      }
    }  
     
    y = 0, x = 1;
    if(EightBitDisplay) {
      mvwaddstr(HelpWin, y++, x, "ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿"); /* Ln 1 */
      mvwaddstr(HelpWin, y++, x, "³                               Other keys                                   ³"); /* Ln 2 */
      mvwaddstr(HelpWin, y++, x, "ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´"); /* Ln 3 */
      mvwaddstr(HelpWin, y++, x, "³   Z/z: toggle debug flag     H/h: this help screen   S: statistics on file ³"); /* Ln 4 */
      mvwaddstr(HelpWin, y++, x, "³   O/o: toggles offset address in hex mode between decimal and hexadecimal  ³"); /* Ln 5 */
      mvwaddstr(HelpWin, y++, x, "³   7: toggle printable chars    8: toggle all chars    *: show line ends    ³"); /* Ln 6 */
      mvwaddstr(HelpWin, y++, x, "³   l: toggle line numbers (only in text mode)                               ³"); /* Added: Ln 7 */
      mvwaddstr(HelpWin, y++, x, "³   t/x: toggles text/hex mode  Q/q: Quit (Hasta La Vista!)                  ³"); /* Ln 8 */
      mvwaddstr(HelpWin, y++, x, "³   /  to search forward                     \\ to search backwards           ³"); /* Ln 9 */
      mvwaddstr(HelpWin, y++, x, "³   n  repeats a previous search in the same direction                       ³"); /* Ln 10 */
      mvwaddstr(HelpWin, y++, x, "³           Other stuff will be put in here soon, as I make up the code      ³"); /* Ln 11 */
      mvwaddstr(HelpWin, y++, x, "³                                                                            ³"); /* Ln 12 */
      mvwaddstr(HelpWin, y++, x, "³   e  fires up an editor on the file     w  write bug report                ³"); /* Ln 13 */
      for(y = 13; y < 22; y++) {
        mvwaddstr(HelpWin, y, x, "³                                                                            ³");
      }  /* Gives us the next few lines the lazy way... */
      mvwaddstr(HelpWin, y++, x, "³                     Hit any key to continue                                ³"); /* Ln 24 */
      y = 23;
      mvwaddstr(HelpWin, y, x,   "ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ"); /* Ln 25 */
      wrefresh(HelpWin);
      c = getch();
    }
    else {
      mvwaddstr(HelpWin, y++, x, ".----------------------------------------------------------------------------."); /* Ln 1 */
      mvwaddstr(HelpWin, y++, x, "|                            Other keys                                      |"); /* Ln 2 */
      mvwaddstr(HelpWin, y++, x, ":----------------------------------------------------------------------------:"); /* Ln 3 */
      mvwaddstr(HelpWin, y++, x, "|   Z/z: toggle debug flag    H/h: this help screen   S: statistics on file  |"); /* Ln 4 */
      mvwaddstr(HelpWin, y++, x, "|   O/o: toggles address in hex mode between decimal and hexadecimal         |"); /* Ln 5 */
      mvwaddstr(HelpWin, y++, x, "|   7: toggle printable chars    8: toggle all chars  *: show line ends      |"); /* Ln 6 */
      mvwaddstr(HelpWin, y++, x, "|   l: toggle line numbers (only in text mode)                               |"); /* Added: Ln 7 */
      mvwaddstr(HelpWin, y++, x, "|   t/x: toggles text/hex mode  Q/q: Quit (Hasta La Vista!)                  |"); /* Ln 8 */
      mvwaddstr(HelpWin, y++, x, "|   /  to search forward   \\ to search backwards      n  repeats search      |"); /* Ln 9  */
      mvwaddstr(HelpWin, y++, x, "| e  fires up an editor on the file   w  write bug report to /tmp/list.debug |"); /* Ln 10 */
      mvwaddstr(HelpWin, y++, x, ":----------------------------------------------------------------------------:"); /* Ln 11 */
      mvwaddstr(HelpWin, y++, x, "|           Other stuff will be put in here soon, as I make up the code      |"); /* Ln 12 */
      mvwaddstr(HelpWin, y++, x, "|                                                                            |"); /* Ln 13 */
      for(y = 12; y < 20; y++) {
        mvwaddstr(HelpWin, y, x, "|                                                                            |"); /* Ln 14-21 */
      }  /* Gives us the next few lines the lazy way... */
      mvwaddstr(HelpWin, y++, x, ":----------------------------------------------------------------------------:"); /* Ln 22 */
      mvwaddstr(HelpWin, y++, x, "|                     Hit any key to continue                                |"); /* Ln 23 */
      mvwaddstr(HelpWin, y, x,   "`----------------------------------------------------------------------------'"); /* Ln 24 */
      wrefresh(HelpWin);
      c = getch();
    }
  } /* ... otherwise, finish up with the ShowHelp() routine... */
/*  clear();
  refresh(); */
   delwin(HelpWin);  /* That takes care of removing the window - no steenkin' memory leaks here */
}
