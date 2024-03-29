# List, a text and hexdump program

## Description

list is a curses-based interactive file listing program that combines the functions of less and hexdump into one handy program. It displays text on the screen in one of two different formats, either in hexadecimal dump format (a la MS-DOS debug.com format or xxd), or in text format, similar to what the programs less and more do. More or less.

The original list program that I saw started out as an excellent program under MS-DOS created by Vernon Buerg, to whom I acknowledge the initial idea. However, there was not an equivalent program under Linux, although people had tried. There are two hexdump programs which provide something like what I wanted, but I also wanted to have a text display at the press of one key (without macros, please...). I note that Midnight Commander also provides the same ability but wrapped up in a nice file manager too.

list is (pretty much) all my own code, but is based on routines slipped in from the dequeue, hexdump and srchfile packages from SNIPPETS, which was maintained by Bob Stout until his death. Mirrors of the SNIPPETS code still exist as of 2020 over on GitHub. List was first created to run under DJGPP, and then morphed into a UNIX C/curses program. A number of bugs appeared to have been squashed in this move, but, needless to say, several still remain.

Naturally, things have evolved since then, but I've finally recovered a copy of this program's source, and will continue to work on it, as and when I get better at coding.
## Build status
![Static Badge](badge-img/Linux-pass.svg)
![Static Badge](badge-img/FreeDOS-pass.svg)
![Static Badge](badge-img/Minix-pass.svg)
![Static Badge](badge-img/FreeBSD-pass.svg)
![Static Badge](badge-img/OpenBSD-pass.svg)

![Static Badge](badge-img/NetBSD-not_compiled.svg)
![Static Badge](badge-img/Solaris-not_compiled.svg)



## Common requirements for compiling
To compile list properly, you will need a compiler and development files for the ncurses (or pdcurses)
library. I don't expect too much fancy stuff, this is a console (terminal) program after all, not a fancy 
GTK/GUI/Qt/X11/Wayland program. For a compiler you'll need either of the GCC suite of tools installed or
the LLVM suite of tools with the clang compiler. You'll also need a make program of some sort.

### Compiling under Linux (most distributions)
At least for the moment, the Makefile will expect to use the GCC suite of tools. If you have the clang
LLVM compiler suite and don't also have the GCC suite of tools, you'll need to modify the Makefile to
reflect this. This will use GNU's make program.

### Compiling under FreeBSD, OpenBSD, NetBSD or Minix
Most of the time, the compile will now use the clang LLVM compiler. If you still have GCC and haven't got
clang installed, you'll need to edit the BSDMakefile to reflect this. This will use BSD's make program,
although it is possible to use GNU's make program instead.

### Compiling under MS-DOS or FreeDOS
For the MS-DOS port, I would suggest using pdcurses, and DJGPP or Watcom. You will need to install or
compile the pdcurses lib to compile this project with Open Watcom. The makefile.wcc file should describe
where I normally expect things to be found, this can be freely changed as you see fit. For this, you'll
need to use wmake.

If you still have the DJGPP environment installed, or perhaps the cygwin environment, you may be able
to utilise the curses available there, but the MS-DOS compile expects to use pdcurses when compiling
with Watcom. I haven't entirely decided what to do about cygwin or DJGPP, which both use the GCC suite
of tools and GNU's make program.

## Bugs, ErROrS and Omisons
list is most definitely still beta software. it is also only designed to reliably handle 7-bit printable ASCII characters, at least under UNIX (this includes Linux). 8-bit and non-printing ASCII characters in files may not be correctly handled. This will depend on the environment where you run the program, but I have tried to sort out the worst offenders (those that would change screen positions).  Some byte sequences in binary files can cause ncurses to go crazy, possibly because these sequences match up with ANSI console commands. As for FreeDOS, I can't say for certain.

I haven't confirmed that this'll work under anything much aside from gcc, clang or Watcom (on FreeDOS).

Also, when going to the End of a file with the End key, you need to go down an extra line to actually display the last line. I haven't tried getting Colour curses modes to work well. In particular, reverse video is not always rendered correctly on some terminal types, and xterms. I have also noticed that xterms do not handle the ALT_GR charset well, unless a suitable font is used in the .Xdefaults file for xterm fonts. But far be it for me to supply a suitable font - let it suffice to say that I developed this originally to display IBM-charset on the screen.

How I would convert to using under an EBCDIC system, I would have no idea about! I have heard that a font called vga will display generally all of the correct high-bit characters (those above 127) unless you have these re-mapped, as most European languages do. I have also not managed to reliably make the code accept input from STDIN, or from any sort of a pipe. This requires expertise that I don't currently have, and my C language skills are limited enough, thank you. Still, this is my first major project - yeah, I would choose such a beast. Wish me luck... Also, it will throw a wobbly if you don't have vim on the system.

## Authors
Thanks to the author of the original MS-DOS LIST program, Vernon Buerg, for providing me with an incentive to "roll my own" version of his program. Authors are of course, myself, and also Dion Bonner for debugging. Bob Stout and Paul Edwards provided some of the original routines in the SNIPPETS collection of public-domain source code, which has been seriously hacked on just to provide what I wanted. The rest is, of course, provided by me.

## The Future, or Things to add

* A filebrowser function, to show a list of files on the screen to choose from. This will be a long way off.

* Better handling in xterms etc. It can also be messy under a telnet session if the client hasn't been written well, to handle clearline and clearscreen or scrollscreen sequences.

* Handle multiple files specified on the commandline.

* Better help screen. I wrote it in the days when I didn't know better. I'd like it to look better than that now.

* Compressed file support. Tricky. After all, I have wanted to peek at the raw file, not always the uncompressed contents.

* Mouse support, to scroll up and down within the file.

## Things I won't be adding

* LISTR/LISTS - at the moment, my code roughly duplicates what's available with LISTS (small) but without the size limits. 

* There will not be a dialer. I mean, who actually has a physical analog dialup modem these days anyway?

* No management of files either, a la Norton's COMMANDER/Midnight Commander or even XTree Gold.

* It has no screensaver function, and no drop-to-DOS (or shell, in this case). 

* The help screen is already minimal, but that's because I haven't written a large amount of content in the program.

* No GUI. This is first and foremost a console program much like the original DOS environment would have been. It won't have widgets nor a pretty toolbar, nice though those things would be to have.

* No CUI support (i.e. Ctl-V/Ctl-C/Ctl-X). It's not an editor, folks. If you want an editor, then list can run one for you, it'll be vim. Additionally, the CUI keys are likely to be poached by the terminal you run this in anyhow.

* (Currently) no preferences support. I don't know how to create them, I don't know how to use them. There may be some support for that in the distant future, but first I will square away the code so that it doesn't fall over at a sneeze. And initially, there won't even be any sanity checking on the preferences, like deciding you want white text on white background. 

* There won't be any 256-colour support. This is because the console in DOS didn't have it. CGA was four colours, EGA was (I think) 16 colours for DOS. I don't even know how I'd get more than that out of curses anyhow, though it's more common now than it ever used to be.

* Raw screen writes. That was for DOS, and Linux doesn't really let you do that these days, at least not without dancing through hoops to get there.

* BIOS screen writes. Same reason as above.

* Windows support. No. Just…no. There are too many variants of the Windows API (Windows 3.1 - Windows 11), and four separate versions of the shells that ran under Windows (COMMAND.COM (MS-DOS 2.0 - 7.0), CMD.EXE, powershell.exe, pwsh.exe). At best, this _may_ work under the terminal shells available for
Windows.

* Telephone, FAX, and BBS support. Do you really want to dial me up in New Zealand at 3:30 am to tell me you can't run this program? These days the support (what little there is) is severely hobbled by my lack of coding knowledge. Yes, I'll get better, but it'll take time. I don't even _have_ a FAX. Meanwhile, post an issue to the repository, I'll probably see it.
