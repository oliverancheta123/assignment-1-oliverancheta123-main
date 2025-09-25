#include "dedup.h"
#include "table_string.h"
#include "vector_char.h"
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void dedup(table_string *ts1, char* file1, table_string *ts2, char* file2) {
  // Find common strings between ts1 and ts2
  // Store in array and print them
  // You can make use of dedup_entry_t (if you want; you can also ignore if you wish)
    // Print them.
    // printf("\n%s", word);
    // printf("\n%s ", file1);
    // Print all lines in file1 with word , separated
    // printf("%d,", entry[i].lines1[j]);
    // printf("\n%s", file2);
    // printf("%d,", entry[i].lines2[j]);
}

void unique(table_string *ts1, char *file1, table_string *ts2, char *file2) {
  // Find unique strings between ts1 and ts2
  // Store in array and print them
  // Print all strings in ts1 that are not in ts2 followed by
  // Print all strings in ts2 that are not in ts1
  // You can make use of dedup_entry_t (if you want; you can also ignore if you
  // wish) Print them. printf("\n%s", word); printf("\n%s ", file1); Print all
  // lines in file1 with word , separated printf("%d,", entry[i].lines1[j]);
  // printf("\n%s", file2);
  // printf("%d,", entry[i].lines2[j]);
  }

 void unify(table_string * ts1, char *file1, table_string *ts2, char *file2) {
    // Find union of strings between ts1 and ts2
    // Store in array and print them
    // Print all strings in ts1 (even those with entries in ts2) followed by
    // Print all strings in ts2 that are not in ts1
    // You can make use of dedup_entry_t (if you want; you can also ignore if
    // you wish) Print them. 
    // print all references for entry. If in ts1 and ts2 print both
    // If only in ts1 print only ts1
    // If only in ts2 print only ts2
    // printf("\n%s", word); printf("\n%s ", file1); Print
    // all lines in file1 with word , separated printf("%d,",
    // entry[i].lines1[j]); printf("\n%s", file2); printf("%d,",
    // entry[i].lines2[j]);}
 }