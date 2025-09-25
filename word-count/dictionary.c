#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector_char.h>
#include <vector_string.h>
int main(int argc, char **argv) {
  char *source = NULL;

  if (argc != 2) {
    printf("./grade_tokenize.bin [FILE PATH]");
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

  
  // TODO: Process source[] and count the number of words
  // Print the number of words in the end.
  vector_string *dict = vector_string_allocate();
  if (!dict) {
    return 1;
  }
  vector_char_t *word = vector_char_allocate();
  if (!word) { // if allocation fails, terminate
    return 1;
  }

  char*copy = source;
        
  while(*copy != '\0') {

    if ((*copy >= 'a' && *copy <= 'z') ||
      (*copy >= 'A' && *copy <= 'Z')) {
      
      vector_char_add(word, *copy);
    } 
    
    else {
      if (word->len > 0) {
        char *dictKey = malloc(word->len + 1);  // +1 for null terminator

        for (int j = 0; j < word->len; j++) {
          dictKey[j] = vector_char_get(word, j); // get only valid chars
        }

        dictKey[word->len] = '\0';  // add end of line char

        if (!vector_string_find(dict, dictKey)) {
          vector_string_insert(dict, dictKey);
        }
        free(dictKey);
        word->len = 0;
      }
    }
    copy++;
  }
  
  if (word->len > 0) { // in case last char is alphabetical
    char *dictKey = malloc(word->len + 1);  // +1 for null terminator
    memcpy(dictKey, vector_char_get_array(word), word->len);
    dictKey[word->len] = '\0';  // add end of line char

    if (!vector_string_find(dict, dictKey)) {
      vector_string_insert(dict, dictKey);
    } 
    free(dictKey);
  }

  free(source);
  vector_string_print(dict);
  vector_char_delete(word);
  vector_string_deallocate(dict);
  return 0;
}