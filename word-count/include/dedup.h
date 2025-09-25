#ifndef DEDUP_H
#define DEDUP_H

#include <stdbool.h>
#include "table_string.h"

typedef struct dedup_entry {
  char *value; // Word
  char *file1; // Filename of the first file
  int* lines1; // Array for holding lines of file1
  char *file2; // Filename of the second file
  int* lines2; // Array for holding lines of file2
  int size_of_lines1; // Size of lines1
  int size_of_lines2; // Size of lines2
  int count; // Count of the word
} dedup_entry_t;


/**
 * @brief Find diff of two files.
 * Include all entries from file not found in file 2.
 *
 * @param ts1
 * @param file1
 * @param ts2
 * @param file2
 */
void subtract(table_string *ts1, char *file1, table_string *ts2);


/**
 * @brief Find union of two files.
 * Include all entries from file1 and then for
 * common entries, update fields for file2. Following which, add the entries
 * from file2.
 *
 * @param ts1
 * @param file1
 * @param ts2
 * @param file2
 */
void unify(table_string *ts1, char *file1, table_string *ts2, char *file2);

/**
 * @brief Find unique words between files
 *
 * @param t1 : Table string of file1
 * @param f1 : File name of file1
 * @param t2 : Table string of file2
 * @param f2 : File name of file2
 * @return int : Number of duplicate words
 */
void unique(table_string *t1, char *f1, table_string *t2, char *f2);

/**
 * @brief Find duplicate words between file 1 and file
 *
 * @param t1 : Table string of file1
 * @param f1 : File name of file1
 * @param t2 : Table string of file2
 * @param f2 : File name of file2
 * @return int : Number of duplicate words
 */
void dedup(table_string *t1, char *f1, table_string *t2, char *f2);



/** Create a dynamically sized array of dedup_entry_t and store counts of common words
/**
 * @brief Find duplicate words between file 1 and file
 *
 * @param t1 : Table string of file1
 * @param t2 : Table string of file2
 * @return void :
 */
void ccount(table_string *t1, table_string *t2);


#endif // DEDUP_H