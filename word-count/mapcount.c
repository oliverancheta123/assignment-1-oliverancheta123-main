#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <table_string.h>
#include <vector_char.h>

// Function to read file content into a string
char* read_file_content(char* filename) {
  char *source = NULL;
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Error reading file: %s\n", filename);
    exit(1);
  }
  
  if (fseek(fp, 0L, SEEK_END) == 0) {
    long bufsize = ftell(fp);
    if (bufsize == -1) {
      printf("Error reading file: %s\n", filename);
      exit(1);
    }
    source = malloc(sizeof(char) * (bufsize + 1));
    if (fseek(fp, 0L, SEEK_SET) != 0) {
      printf("Error reading file: %s\n", filename);
      exit(1);
    }
    size_t newLen = fread(source, sizeof(char), bufsize, fp);
    if (ferror(fp) != 0) {
      printf("Error reading file: %s\n", filename);
      exit(1);
    } else {
      source[newLen++] = '\0';
    }
  }
  fclose(fp);
  return source;
}

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("Usage: ./mapcount.bin [FIRST_FILE] [SECOND_FILE]\n");
    exit(1);
  }
  
  // Read both files
  char *first_file_content = read_file_content(argv[1]);
  char *second_file_content = read_file_content(argv[2]);

  /** Process first file and build table_string with word positions */

  /** Process second file and search for consecutive word pairs */
  return 0;
}