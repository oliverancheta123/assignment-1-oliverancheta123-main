#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <table_string.h>
#include <vector_char.h>
int main(int argc, char **argv) {
  char *source = NULL;

  if (argc != 2) {
    printf("./wordcount.bin [FILE PATH]");
    exit(1);
  }
  /**
   * @brief Read file into source.
   * source is a character array with file contents
   * It is null terminated
   */
  FILE *fp = fopen(argv[1], "r");
  if (fp == NULL) { /* Error */
    printf("Error reading file");
    exit(1);
  }
  if (fp != NULL) {
    /* Go to the end of the file. */
    if (fseek(fp, 0L, SEEK_END) == 0) {
      /* Get the size of the file. */
      long bufsize = ftell(fp);
      if (bufsize == -1) { /* Error */
        printf("Error reading file");
      }
      /* Allocate our buffer to that size. */
      source = malloc(sizeof(char) * (bufsize + 1));
      /* Go back to the start of the file. */
      if (fseek(fp, 0L, SEEK_SET) != 0) { /* Error */
      }
      /* Read the entire file into memory. */
      size_t newLen = fread(source, sizeof(char), bufsize, fp);
      if (ferror(fp) != 0) {
        printf("Error reading file");
      } else {
        source[newLen++] = '\0'; /* Just to be safe. */
      }
    }
  }
  fclose(fp);

  /** Start processing file and separate into words */
  // TODO: Write linecount
  
  table_string *hash = table_string_allocate(8);
  if (!hash) {
    return 1;
  }

  vector_char_t *word = vector_char_allocate();
  if (!word) {
    return 1;
  }

  int lineCount = 0;

  char *copy = source;

  while (*copy != '\0') {

    if ((*copy >= 'a' && *copy <= 'z') ||
      (*copy >= 'A' && *copy <= 'Z')) {
      
      vector_char_add(word, *copy);
    } 
/*
    else if (*copy == '\n') {
      table_string_insert_or_add(hash, word, lineCount);
      lineCount++;
    }
*/
    else {
      if (word->len > 0) {

        char *wordCpy = malloc(word->len + 1);  // +1 for null terminator

        for (int j = 0; j < word->len; j++) {
          wordCpy[j] = vector_char_get(word, j); // get only valid chars
        }
        wordCpy[word->len] = '\0';  // add end of line char

        table_string_insert_or_add(hash, wordCpy, lineCount);
        
        lineCount++;
        word->len = 0;
      }
    }
    copy++;
  }

  free(source);
  table_string_print(hash);
  vector_char_delete(word);
  table_string_deallocate(hash);

  return 0;
}