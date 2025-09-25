#include "table_string.h"
#include "str_cmp.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUCKETS
// DO NOT ADD ANY NEW FUNCTIONS BEFORE djb2
// Hash a character array to integer value between 0 and buckets-1.
// See here for more details: https://theartincode.stanis.me/008-djb2/
unsigned int djb2_word_to_bucket(char *word, int buckets) {
  if (!word) {
    printf("Invoked with null string");
    exit(EXIT_FAILURE);
  }
  unsigned long hash = 5381;
  uint32_t i;

  for (i = 0; i < strlen(word); i++)
    hash = ((hash << 5) + hash) + word[i];

  return hash & (buckets - 1);
}
// ADD YOUR HELPER FUNCTIONS AFTER djb2
/**
 * @brief Instantiate a table string
 *
 * @param buckets
 * @return table_string*
 */
table_string *table_string_allocate(unsigned int buckets) {
  // Instantiate a bucket number of heads
  table_string *hash = malloc(sizeof(table_string));
  if (!hash) {
    return NULL;
  }

  hash->buckets = buckets;
  hash->heads = calloc(buckets, sizeof(vs_entry_t*));

  if (!hash->heads) {
    free(hash); // clear memory for empty hash
    return NULL;
  }

  return hash;
}

/**
 * @brief Insert the string pointed to by word into the table string.
 *   If word is already found increment count.
 *   If word is not found insert at the end of the corresponding bucket
 * @param ts - Pointer to table_string datastructure
 * @param word - Pointer to string to search for
 * @return * Find*
 */

void table_string_insert_or_add(table_string *ts, char *word, int line) {
    // Find the bucket for word. djb2 will return a value between 0-buckets-1.
    // Rule: buckets can only be power of 2.
    int bucket = djb2_word_to_bucket(word, ts->buckets);

    vs_entry_t *current = ts->heads[bucket];

    int wordLength = 0;
    while (word[wordLength] != '\0') {
        wordLength++;
    }

    // Bucket hasn't been initialized yet
    if (ts->heads[bucket] == NULL) {
        vs_entry_t *newNode = malloc(sizeof(vs_entry_t));
        newNode->pos = malloc(sizeof(int));
        newNode->pos[0] = line;
        newNode->size_of_pos = 1;

        char *wordCpy = malloc(wordLength + 1);
        for (int i = 0; i <= wordLength; i++) {
            wordCpy[i] = word[i];
        }

        newNode->value = wordCpy;
        newNode->next = NULL;
        ts->heads[bucket] = newNode;
        return;
        
    } else { // bucket already has words
        while (current->next != NULL) {
          
            if (strcmp(word, current->value) == 0) { // already in list
                // Allocate space in line array
                current->pos = realloc(current->pos, (current->size_of_pos + 1) * sizeof(int));
                current->pos[current->size_of_pos] = line;
                current->size_of_pos += 1;
                return;
            }
            current = current->next;
        }

        // check last node, haven't checked when current->next == NULL
        if (strcmp(word, current->value) == 0) {
            current->pos = realloc(current->pos, (current->size_of_pos + 1) * sizeof(int));
            current->pos[current->size_of_pos] = line;
            current->size_of_pos += 1;
            return;
        }

        // new word found
        vs_entry_t *newNode = malloc(sizeof(vs_entry_t));
        newNode->pos = malloc(sizeof(int));
        newNode->pos[0] = line;
        newNode->size_of_pos = 1;

        char *wordCpy = malloc(wordLength + 1);
        for (int i = 0; i <= wordLength; i++) {
            wordCpy[i] = word[i];
        }

        newNode->value = wordCpy;
        newNode->next = NULL;
        current->next = newNode;
        return;
    }
}


void table_string_deallocate(table_string *ts) {
  // TODO:
  // Free the linked list of each bucket
  // Free the array of head pointers
  // Free the structure itself
  int numBuckets = ts->buckets;

  while (numBuckets > 0) { // Loop through buckets
    vs_entry_t *current = ts->heads[numBuckets-1];

    while (current != NULL) { // Loop through each node in each bucket
      vs_entry_t *currentNext = current->next;
      free(current);
      current = currentNext;
    }
    numBuckets -= 1;
    
  }
  free(ts->heads);
  free(ts);
  return;
}

void table_string_print(table_string *ts) {
  /** TODO:
   for each bucket
    for each entry in bucket
      print entry->word: line line line
  */
  for (int i = 0; i < ts->buckets; i++) {
    vs_entry_t *current = ts->heads[i];

    while (current != NULL) {
      printf("%s: ", current->value);

      for (int line = 0; line < current->size_of_pos; line++) {
        printf("%d ", current->pos[line]);
      }
      putchar('\n');
      current = current->next;
    }
  }
}
