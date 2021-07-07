# List, a text and hexdump program

## Description

list is a curses-based interactive file listing program that combines the functions of less and hexdump into one handy program. It displays text on the screen in one of two different formats, either in hexadecimal dump format (a la MS-DOS debug.com format), or in text format, similar to what the programs less and more do.

The original list program that I saw started out as a program under MS-DOS, created by Vernon Buerg, to whom I acknowledge the initial idea. However, there was not an equivalent program under Linux, though people had tried. There is a hexdump program, which provides something like what I wanted, but I also wanted to have a text display at the press of one key (without macros, please...) I note there's also
the same ability but wrapped up in a nice file manager too, Midnight Commander.

This is (pretty much) all my own code, based on routines slipped in from the dequeue, hexdump and srchfile packages from SNIPPETS, which was maintained by Bob Stout until his death. Mirrors of the SNIPPETS code still exist as of 2020 over on GitHub. List was first converted to run under DJGPP, and then again into a UNIX C/curses program. A number of bugs appeared to have been squashed in this move, but, needless to say, several still remain.

Naturally, things have evolved since then, but I've finally recovered a copy of this program's source, and will continue to work on it, as and when I get better at coding.

## Bugs, ErROrS and Omisons
list is most definitely still beta software. it is also only designed to reliably handle 7-bit printable ASCII characters, at least under UNIX (this includes Linux). 8-bit and non-printing ASCII characters in files may not be correctly handled. This will depend on the environment where you run the program, but I have tried to sort out the worst offenders (those that would change screen positions). For porting back to MS-DOS, I would suggest using pdcurses, and DJGPP. Some byte sequences in binary files can cause ncurses to go crazy, possibly because these sequences match up with ANSI console commands.

Also, when going to the End of a file with the End key, you need to go down an extra line to actually display the last line. I haven't tried getting Colour curses modes to work well. In particular, reverse video is not always rendered correctly on some terminal types, and xterms. I have also noticed that xterms do not handle the ALT_GR charset well, unless a suitable font is used in the .Xdefaults file for xterm fonts. But far be it for me to supply a suitable font - let it suffice to say that I developed this originally to display IBM-charset on the screen.

How I would convert to using under an EBCDIC system, I would have no idea about! I have heard that a font called vga will display generally all of the correct high-bit characters (those above 127) unless you have these re-mapped, as most European languages do. I have also not managed to reliably make the code accept input from STDIN, or from any sort of a pipe. This requires expertise that I don't currently have, and my C language skills are limited enough, thank you. Still, this is my first major project - yeah, I would choose such a beast. Wish me luck... Also, it will throw a wobbly if you don't have vim on the system.

## Authors
Thanks to the author of the original MS-DOS LIST program, Vernon Buerg, for providing me with an incentive to "roll my own" version of his program. Authors are of course, myself, and also Dion Bonner for debugging. Bob Stout and Paul Edwards provided some of the original routines in the SNIPPETS collection of public-domain source code, which has been seriously hacked on just to provide what I wanted. The rest is, of course, provided by me.

## The Future, or Things to add

* A filebrowser function, to show a list of files on the screen to choose from.

* Better handling in xterms etc. It can also be messy under a telnet session if the client hasn't been written well, to handle clearline and clearscreen or scrollscreen sequences.

* Handle multiple files specified on the commandline.

