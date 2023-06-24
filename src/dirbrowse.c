#include <stdio.h>
#include <stdlib.h>
#include <stat.h>
/* Do I need sys/stat.h here? */
#include "list.h"

/* Should display a sorted directory */
char *ReadSortedDirectory(DIR *) {  /* Once we have type of returned list, change this */
  /* Things I'll need to know how to do:
   * extract a list of files from a provided directory
     fail if we can't access/read the directory itself
   * open directory; fail if eperm
     for number_of_entries
     do { read entry
       add entry.name to list
     } while (--entries > 0)

   * sort a list of files
   * return a pointer to the sorted list of files 
  */
  return 0; /* STUB */
}

/* Print sorted directory into area on screen
   return -1 if error, 0 otherwise
 */
int PrintDirectory(void) {
  return 0; /* STUB */
}
