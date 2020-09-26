/* vim:ts=2:ww=0:
 *
**
**  WARNING: program in semi-stable (read - BETA) state at the moment...it's usable, but a few bugs remain.
**
**  Released under the GPL version 2, Copyright (c) 2002 by Eric Gillespie
**  Last updated 2020, made ready to import to github repository
*/
#ifndef __linux__
#ifndef __OpenBSD__  /* Then is it NetBSD? */
#ifndef __NetBSD__ /* Nope, FreeBSD? */
#ifndef __FreeBSD__  /* No, try DOS */ 
#ifndef __DJGPP__ /* Must be minix... */
#include <lib.h>  
/* Sheesh - want to #define _MINIX but I'm striking trouble,
   this file is the only one to #define _MINIX
*/
#endif  /* !DJGPP (i.e. MINIX) */
#endif  /* !BSD - this doesn't take DOS into account */
#endif /* !FreeBSD */
#endif /* !OpenBSD */
#endif /* !__linux__ */
#ifndef _MINIX  /* because MINIX hasn't got a basename() */
#include <libgen.h>  /* Just for basename() */
#endif
#include <stdlib.h>
#include <errno.h> /* For ... you guessed it! */
#include <string.h> /* ... though I thought basename was in here... */
#include <fcntl.h>
#include <curses.h>
#include <ctype.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>  /*  Do we need this here, or in filebrowse? */
/* User includes */
#include "list.h"
#include "dequeue.h"

/* User Definitions */
/* Have stuck this here, seeing as list.h is shared between here and filebrowse,
 * and it is a little inappropriate to have two declarations of the same function
 */
#ifdef _MINIX
/* Have had to define this here, 'cos there isn't such a function in minix */
void getmaxyx(WINDOW *win, int Cols, int Rows)  {
 Cols = (win)->_maxx;
 Rows = (win)->_maxy;
}
int has_colors(void){
  return 0;  /* returns 0 if no color, 1 otherwise... */
  }

void start_color(void) {
}

/* minix pukes on this next definition.. the compiler packs a SIGSEV */
void init_color(int First, int Second, int Third) { /* There! Will this cure the problem? */
}
#endif

/* Ah! This is here, 'cos I can't define it in a shared header file */
static size_t fsetup(FILE *, size_t);

int main(int argc, char **argv)
{
  char *bufp;
  char *tempstring;  /* Don't forget to malloc later on... */
  int c, y=0, x=0;
  /* I have to fix this - I HAVE to initialise this struct before referencing it... */
/*   struct stat FileInfo; / * stat from <sys/stat.h> - 2 lines - bits to delete... */
/*   struct stat *ListFileInfo; / * ... and create an associated pointer... */

  struct FileData ListFile = {
    .DumpMode = 't', /* DumpMode - set to text by default */
    .DumpFlag = '0', /* DumpFlag */ /* It's official - this wants to be a char */
    .SearchDirection = '\0', /* SearchDirection */
    };  /* ... the end of the struct init */
  /* Initialises struct to empty - yep, everything! - lclint complains*/
  struct FileData *fi; /* Define an associated pointer... doesn't this have to be alloc'ed? */
  fi=&ListFile; /* ... and point it to our just-filled ListFile struct */
  fi->FileInfoPtr = &fi->FileInfo;
  tempstring = (char *) malloc(sizeof tempstring * 1024); /* Does this limit need re-evaluating? */
  if(tempstring == NULL) Bye(1, __LINE__); /* died due to lack of memory */
  /* ... otherwise, continue as planned... */
  (void) Q_Init(&fi->DirQueuePtr); /* Initialises the queue created - lclint suggests return a void */

  /* getopt parser */
  debug=0;
  opterr = 0;

  if(argc < 2) {
    errno = ENOENT;
    Bye(12, __LINE__); /* 'Nother Dion bug squashed */
  }
  /* Now copy the argc and argv to our localspace */
  fi->MyArgc = argc;
  fi->MyArgv = argv; /* Will this work?  I don't know... */
  /* Okay - should we shove all THIS into a separate function too? */
  while ((c=getopt(fi->MyArgc, fi->MyArgv, "vf:s:r:c:n:b:eqltx78dmh?"))!= -1) {
/* v - version  f: filename s: start_byte r: rows to display  c: columns to display n: num_bytes b: taB size
 * e - show line Endings q - QuickHexmode l - show line numbers in text mode  t - text  x - hexadecimal
 * 7/8 - 7/8-bit display d - debug m - monochrome usage h/? - Usage 
 *
 */
    switch(c) {
      case 'v':    /* Spits out version information */
           printf("list: Version %1d.%1d.%1d, compiled on %s at %s, by %s\n", LISTVERSION, LISTMAJOR, LISTMINOR, __DATE__, __TIME__, "Eric Gillespie");  /* Why don't we run this through Bye()? Because there's no ERROR, that's why!!! */
           exit(0);
           break;  /* never gets to this line... 8-) this should be tidier */
      case 'f': /* Load following filename */
           /* This gets duplicated down the bottom if the -f switch wasn't used... hmmm */
           NewFile(fi, optarg);
           break;
      case 's': /* Start position */
           fi->Start=strtol(optarg, &bufp, 0);
           break;
      case 'r': /* Set number of rows (lines) different from present screen */
           y=strtol(optarg, &bufp, 0);
           if(y < fi->Scrn_y)
              fi->Scrn_y = y;
           break;
      case 'c': /*  Set number of columns different from present screen  */
           x=strtol(optarg, &bufp, 0);
           if(x < fi->Scrn_x)
               fi->Scrn_x = x;
           break;
      case 'n': /* Count off (N)um bytes */
           fi->Count=strtol(optarg, &bufp, 0);
           break;
      case 'b': /* ta(B) size */
           fi->TabSize = strtol(optarg, &bufp, 0);
           break;
      case 'e': /* Show line ends */
           fi->DumpFlag = '*';
           break;
      case 'l': /* Show line numbers in text mode */
           fi->LineNumbers = 1;
           break;
      case 't': /* Start in Text mode (default) */
           if(!fi->Quick)
             fi->DumpMode = 't';
           else  {
                   fi->DumpMode = 'x';
                   printf("You selected Quickmode - displaying hex only\n");
           }
           break;
/*    case 'A':   80x86 asm mode (Intel or AT&T?) : see how biew has done it...
      case 'a':
           fi->DumpMode='a';
           break;             end of comment */
      case 'x': /* Start in Hexdump mode */
           fi->DumpMode='x';
           break;
      case '7': /* Use 7-bit (Unixy) dump mode */
      /* This displays only characters in the range 0x20 - 0x7E, all others show as '.' */
           fi->DumpFlag = '7';
           break;
      case '8': /* Use 8-bit (DOS) mode - display all characters except 0x0 - 0x1F, 0x7F
               This mode works well in situations where 0x80 - 0xFF map exactly to IBM's
               idea of what they should be, alas in the Unix world, this is very rarely
               the case.  In fact, a lot of the time, they're not even like that in the
               DOS world if codepages are installed, or when Windows has a different charset */
           fi->DumpFlag = '8';
           break;
      case 'd': /* Turn on the debugging flag - beware - this singlesteps most main procedures */
           debug++;
           break;
      case 'm':   /* this line added to deal with debugging for minix when in Linux */
           fi->Monochrome++; /* Oooops - this value is asked for before we set it - naughty... */
           break;
      case 'q': /* This is only in while I get around the textwrap problems... */
           fi->Quick++;
           fi->DumpMode='x'; /* Force this into hexmode dump */
           break;
      case 'h': /* Well, I guess this gives a helpscreen and exits */
      case '?':
 /* Added by Dion the Bugfinder - after all, we want to see the helpscreen before it gets whisked away. */
/*           c = getch();  kinda made irrelevant by making the Usage into
 *           Printf */
           Usage();
/* After exiting from Usage(), could I go to the file selection screen?
 * Maybe not, if my options are too seriously screwed up ... */
           return (EXIT_SUCCESS); /* (Not FAILURE) After all, it did what we wanted... */
           break;
      default: /* Well, we don't know what this is... */
           refresh();
           endwin();
           printf("list: Unknown switch\n");
           Usage();
           return (EXIT_FAILURE);
    }  /* end of switch statement */
  } /* end of while loop */
  /* Process any non-flag arguments following the flags using optind
     as index to next argument.
     getopt() sets the global variable optind set to the next
     argument to process.  After the loop above it is pointing to the
     first command line argument following processed flags and their args. */
/* Perhaps I should change this to if(!fi->FPtr) { NewFile(fi, fi->MyArgv[optind]; } */
/* Old line: */
  for( ; optind < fi->MyArgc; optind++ ) {
    if(!fi->FPtr) {
/* This line below is incorrect - it should be Newfile on the first filename only...
 * and besides which, what happens when I go beyond argc?
 */
      NewFile(fi, fi->MyArgv[optind]); /* This is where all the file opening work is done */
    /* Another point - this will end up opening the *last* file on the commandline -
     * I should shove these into a linked list dequeue-style, perhaps add a new function - AddFile()?
     * Each list would have filename, previous filename, and next filename in its entry
     * Of course, I guess I could call Dump() on each...
     * Hey, this might fit in better with DirRead() and filebrowse.c ...*/
     }
  }
  if(fi->FName==0) {  /* If nothing else specified on commandline .. */

    /* fi->FPtr=stdin;   /x* We might have to scrap this, I think... */
    /* One way of dealing with the stdin would be (borrowed from an MS-DOS idea)
           * is to copy the stdin to a temporary file, or to allocate a large array */
    NewFile(fi, "stdin"); /* I should probably copy pipe data to a temp buffer - either memory or tempfile */
          fflush(fi->FPtr); /* Just to satisfy the purists... */
          /* MS-DOSism   setmode(fileno(stdin), O_BINARY);  ... try the above instead.. */
          /*    addstr("list: No filename specified on commandline\n"); */
					CloseNCurses();
    Bye(12, __LINE__); /* We dump out by telling the user we can't open file at the moment... */
  }
  /* ncurses code can FINALLY start here! */
  initscr();
  if(!fi->Monochrome) { /* 1s If we're NOT in mono ... */
    if( has_colors()) {  /* 2s - test we have color available. Hey, what happens if i want to run in monochrome? */
      start_color();
      init_pair(COLOR_BLACK, COLOR_BLACK, COLOR_BLACK);
      init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLUE);
      init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
      init_pair(COLOR_GREEN, COLOR_YELLOW, COLOR_RED);  /* Set this to unusual pairing */
      init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
      init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
      init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLUE);
      init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLUE);
    } /* 2e */
  } /* 1e */
/*    Bye(6, __LINE__);  / * This line will be replaced soon - just give me time to write the monochrome equivalents */
  noecho();
  cbreak();
  keypad(stdscr, TRUE);
  nonl();
/*  bkgd(A_ALTCHARSET); */
 /* This line tell us the real screensize -in fact I've just found a duplication with a line inside Dump() */
  getmaxyx(stdscr, fi->Scrn_y, fi->Scrn_x);
  
  if(debug) { /* We will have a future conflict here when we finally include stdin */
    sprintf(tempstring,"FName: %s          DumpMode: %c\n\rStart: %ld        Count: %ld\n\r",
            fi->FName, fi->DumpMode, fi->Start, fi->Count);
    debug_function(tempstring, 0, fi->Scrn_y, __LINE__);
  }
  /* end of getopt section - this had better work...
   * Incidentally, I've noticed OpenBSD 2.7 doesn't
	 * I don't know about any other forms of BSD
   * handle getopt parameters the same way as Linux.
  */
  /* First, a check on the fi->FName, to see if it has budged from 0 */
  if(!(fi->FPtr==stdin)) { /* if the FPtr is NOT == stdin, then ... */
    if( !( fi->FPtr = fopen( fi->FName, "rb"))) { /* ...find out if the file can be opened for reading, if not ... */
			CloseNCurses();
      Bye(7, __LINE__);
    } /* End of failure to open */
    if(fi->Scrn_x < 40) { /* Darn - this witty comment gets lost in the Bye() call */
/*      addstr("list: Um, I don't have enough columns to display the file in hex format,");
      addstr("      So am defaulting to text mode!  Hope you don't mind!");  */
			CloseNCurses();
      Bye(9,__LINE__);
    }
    if(fi->Scrn_y < 24) { /* Darn - this witty comment gets lost in the Bye() call */
/*            addstr("list: Err, there aren't enough lines here to do justice to this");
            addstr("      file - I need to get out of here! (Reason:Claustrophobia)"); */
			CloseNCurses();
      Bye(10,__LINE__);
    }
/*    stat(fi->FName, ListFileInfo); / * Second time this file is stat'ed - is this necessary? */
    /* Here are a few sanity checks... */
    if(fi->FileInfo.st_size == 0) {  /* get the filesize... */
      addstr("list: Hmmm, the file appears to be of zero length!!");
      refresh();
      endwin();
      exit(EXIT_FAILURE);
    } /* End of "File is zero bytes in size" */
    else if(fi->Count < 1) {
      addstr("list: You're trying to ask for too few bytes...!!");
      refresh();
      endwin();
      exit(EXIT_FAILURE);
    }
    else if(fi->Start > fi->FileInfo.st_size) {
      addstr("list: You're asking for bytes starting past the end of the file...");
      refresh();
      endwin();
      exit(EXIT_FAILURE);
    }
    fi->FEnd=fi->FileInfo.st_size;
  }
  else { /* Huh? input _IS_ stdin? */
    addstr("list: Seeking to end...");
    refresh();
    cbreak();
    noecho();
    /*      nodelay(); */
    /*    if((c=getchar())=='Q') Bye(2, __LINE__); */
/*    while(c!=EOF) c=getchar();  wait for key ... */
    fseek(fi->FPtr, 0, SEEK_END);
    if(!(fi->FEnd=ftell(fi->FPtr))) {
      addstr("list: Ooops - selected file is empty\n");
      refresh();
      endwin();
      exit(EXIT_FAILURE);
    }  /* end of if file empty */
  } /* end of else file==STDIN */
  /* Ummm, seems these lines are not needed...they already get seen to in NewFile() */
  /******************************************************/
  /*  ScanForCr(fi);    / * this should hopefully give me the correct
                     * number of lines in my data * /
  / * looks like I'm getting a mismatch - revamp the routine?  * /
  if(!(fi->CrArray=calloc(fi->FLines, sizeof(u_long)))) Bye(1, __LINE__);
  fi->CrArray=AllocateLines(fi); / * This allocates the line ends */
  /*******************************************************/
  if (fi->Count==0)
    fi->Count = fi->FEnd;
  if(fi->DumpFlag!='7' && fi->DumpFlag != '8')
    fi->DumpFlag = '7';  /* sets allchars/printchars switch - make it 7 just for safety on binary files */
  /* Go ahead and show the file on the screen */
  Dump(fi);
  /* Now shut down and tidy everything up */
  fclose(fi->FPtr);
  /*    fflush(stdout); *  is this needed? */
  /*    setmode(fileno(stdin), O_TEXT); * Ooops, another DOS-ism crept in here */
  free(fi->CrArray);
  clear();
  refresh();
  endwin();
  return (EXIT_SUCCESS);
}  /* End of main() */

void CartWheel(void)  {
  int i;
  char tst[2];
  char *tstr=&tst[0];
  char whirl[] = { '\\', '|', '/', '-' };
  for(i = 0; i <= 3; i++) {
    move(22,39);
    sprintf(tstr,"%c", whirl[i]);
    addstr(tstr);
    refresh();
    /*    delay(100); */
  }
}

/*
 * Is the specified file a directory?
 * (This code is lifted straight out of less 3.58 and de-K&R'ed)
 */
#ifndef S_ISDIR
#define S_ISDIR(m)      (((m) & S_IFMT) == S_IFDIR)
#endif
#ifndef S_ISREG
#define S_ISREG(m)      (((m) & S_IFMT) == S_IFREG)
#endif

int is_dir(char *filename) { /* Hey, why do I need this routine, anyway? */
  int isdir = 0;

  filename = unquote_file(filename);  /* this routine I'll have to duplicate too... */
  {
    int r;
    struct stat statbuf;

    r = stat(filename, &statbuf);
    isdir = (r >= 0 && S_ISDIR(statbuf.st_mode));
  }
  free(filename);
  return (isdir);
}

/*
 *  * Remove quotes around a filename.
 *   */
char * unquote_file(char *str) {
  char *name;
  char *p;
  char openquote = '"';
  char closequote = openquote;

  if (*str != openquote)
  return (save(str));
  name = (char *) ecalloc(strlen(str), sizeof(char));
  strcpy(name, str+1);
  p = name + strlen(name) - 1;
  if (*p == closequote)
  *p = '\0';
  return (name);
}


/*
 * Copy a string to a "safe" place
 * (that is, to a buffer allocated by calloc).
 */
char *save(char *s) {
   register char *p;

   p = (char *) ecalloc(strlen(s)+1, sizeof(char));
   strcpy(p, s);
   return (p);
}

/*
 * Allocate memory.
 * Like calloc(), but never returns an error (NULL).
 */
void *ecalloc(int count, unsigned int size) {
  register void *p;

  p = (void *) calloc(count, size);
  if (p != NULL)
    return (p);
/*  error("Cannot allocate memory", NULL_PARG);
  quit(QUIT_ERROR);   */
  Bye(1, __LINE__);
  /*NOTREACHED*/
  exit(1);
}


/*
 *  Allocate a big buffer, use it to buffer a specified stream
 *  - but why does it have to be declared static?
 */

static size_t fsetup(FILE *fp, size_t minbuf) {
  register size_t bufsize;
  register char *buffer;

/* Allocate the largest buffer we can */

  for (bufsize = 0x4000; bufsize >= minbuf; bufsize >>= 1) {
  if (NULL != (buffer = (char *) malloc(bufsize)))
    break;
  }
  if (NULL == buffer)
    return 0;

  /* Use the buffer to buffer the file */

  if (0L == setvbuf(fp, buffer, _IOFBF, bufsize))
    return bufsize;
  else  return 0;
}

/*
**  Search a file for a pattern match (forward)
**
**  Arguments: FILE pointer
**             pattern to search for
**             size of pattern
**             find Nth occurrence
**
**  Returns: -1L if pattern not found
**           -2L in case of error
*/

long ffsearch(struct FileData *fi, int N) {  /* 1st br */
  /* Hmmm, seems we don't need size, we can surely get that from strlen(SearchString) */
  long pos = -2L, tempOffset = 0L;
  char *sbuf, *p;
  size_t skip, size;
  int ch = 0;
  size = strlen(fi->SearchString);  /* We want the length of the string... 8-) */

/* Allocate a search buffer */

  if (NULL == (sbuf = (char *)malloc(size - 1)))
    goto FDONE; /* Whoops, couldn't malloc, fall off end */

  /* Buffer the file and position us within it */

  if (0 == fsetup(fi->FPtr, size))
    goto FDONE; /* Fall off end, might have had a malloc failure, or size = 0 */
  pos = -1L;
  fseek(fi->FPtr, fi->FPosn, SEEK_SET);

  /* Set up for smart searching */

  if (1 < strlen(fi->SearchString) && NULL != (p = strchr(fi->SearchString + 1, *fi->SearchString)))
    skip = p - (char *)fi->SearchString;
  else  skip = strlen(fi->SearchString);

  /* Look for the pattern */

  while (EOF != ch) {  /* 2nd in */
    if (EOF == (ch = fgetc(fi->FPtr)))
      break;
    if ((int)*fi->SearchString == ch) {  /* 3rd in */
       tempOffset = ftell(fi->FPtr);
       if (size - 1 > fread(sbuf, sizeof(char), size - 1, fi->FPtr))
         goto FDONE;
       if (0L == memcmp(sbuf, &fi->SearchString[1], size - 1)) {  /* 4th in */
         if (0L == --N) {  /* 5th in */
           pos = tempOffset - 1L;
     goto  FDONE;
         }  /* 5th out */
       }  /* 4th out */
       fi->SPosn = ftell(fi->FPtr);  /* uh?  this doesn't seem to work... */

     } /* 3rd out */
  } /* 2nd out */
/* Clean up and leave */

FDONE:
  free(sbuf);
  return pos;
} /* end of ffsearch */


/*
**  Search a file for a pattern match (backwards)
**
**  Arguments: FILE pointer
**             pattern to search for
**             size of pattern
**             find Nth occurrence
**
**  Returns: -1L if pattern not found
**           -2L in case of error
*/

long rfsearch(struct FileData *fi, int N) {
/* Again, do we need size?  Again, size can be got from strlen(fi->SearchString) -
 * in fact, now I come to think of it, fi->SearchString could provide *fi->Search
 */
  long pos = -2L, tempos;
  char *sbuf, *p;
  size_t skip, size;
  int ch = 0;
  size = strlen(fi->SearchString);
  /* Allocate a search buffer */

  if (NULL == (sbuf = (char *)malloc(size - 1)))
    goto RDONE; /* Whoops, couldn't malloc, fall off end */

  /* Buffer the file and position us within it */

  if (0 == fsetup(fi->FPtr, size))
    goto RDONE;
  pos = -1L;
      fseek(fi->FPtr, fi->FPosn, SEEK_SET);
      tempos = ftell(fi->FPtr) - strlen(fi->SearchString);

      /* Set up for smart searching */

      if (1 < strlen(fi->SearchString) && NULL != (p = strrchr(fi->SearchString + 1, *fi->SearchString)))
            skip = strlen(fi->SearchString) - (p - (char *)fi->SearchString);
      else  skip = strlen(fi->SearchString);

      /* Look for the pattern */

      while (0L <= tempos)
      {
            fseek(fi->FPtr, tempos, SEEK_SET);
            if (EOF == (ch = fgetc(fi->FPtr)))
                  break;
            if ((int)*fi->SearchString == ch)
            {
                  if (size - 1 <= fread(sbuf, sizeof(char), size - 1, fi->FPtr))
                  {
                        if (0L == memcmp(sbuf, &fi->SearchString[1], size - 1))
                        {
                              if (0 == --N)
                              {
                                    pos = tempos;
                                    goto RDONE;
                              }
                        }
                  }
                  tempos -= skip;
            }
            else  --tempos;
      }

      /* Clean up and leave */

RDONE:
      free(sbuf);
      return pos;
}

char *SearchParse(const char *Search)  {
   char *NewSearch;
   NewSearch = (char *) malloc(sizeof(NewSearch) * (strlen(Search) + 1));
   /* This is where things need to be looked for, such as \ processing */
   return NewSearch;
}

int CmdMode(struct FileData *fi)  { /* 1s */
/* In here, I'd like to define a vim-like interface for adding commands like (N)ext file/(P)rev file... */
/* First, steal a little code, massage a little code... */
/* Various switches go through here...
  N - next file
  P - Prev file
  * - else, back to program
  Q - obvious
 */
  int c;
  char *WhatDoIPutInHere = (char *) malloc(sizeof(WhatDoIPutInHere) * PATH_MAX);
  char *OldFName = (char *) malloc(sizeof(OldFName) * PATH_MAX);
  strcpy(OldFName, fi->FName);  /* This is supposed to provide a fallback filename */
/* Assign WhatDoIPutInHere to a member of the array of filenames we were fed
 * at startup */

  switch(c=getch()) { /* 2 */
    /* We need processing for esc chars here... */
  case 'n':          /* next file */
  case 'N':
  /* Incidentally, what is argc at this stage? */
    NewFile(fi, WhatDoIPutInHere);
    break;
  case 'p':          /* next file */
  case 'P':
  /* Incidentally, what is argc at this stage? */
    NewFile(fi, WhatDoIPutInHere);
    break;
/*  case 'Q':          / * Quit - (Hasta La Vista!) * /
  case 'q':
    return 0;  / * Hmmm, why should we quit from here? */
  default:
    return 0;
    break;
  } /* 2e */
 return 0;
}

/*
void AsciiTable(void) {
char    squaretop=" 218 ÚÄÄÄÄÄÂÄÄÄ¿ 191 ";
char     whatever=" 179 ³ 196 194 ³ 179 ";
char   squarejoin=" 195 Ã 197 Å   ´ 180";
char   squareside=" 179 ³ 193     ³ ";
char squarebottom=" 192 ÀÄÄÁÄÄÄÄÄÄÙ 217";
}
*/

