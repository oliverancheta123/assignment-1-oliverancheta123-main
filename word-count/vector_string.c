#include "vector_string.h"
#include "str_cmp.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/**
/////////////////////////////////////////

*/
/**
 * @brief Memory allocate a vector string so that we can start inserting entries
 * into it.
 *
 */
vector_string *vector_string_allocate() {
  
  vector_string *header = (vector_string*)malloc(sizeof(vector_string));
  if (!header)
    return NULL;
    
    header->head = NULL;
    header->tail = NULL;

  return header;
}

/**
 * @brief Search the vector string pointed to by vs and return true if the
 * vs_entry.value == key, else return false.
 * TODO:
 * @param vs - Pointer to vector_string datastructure
 * @param key - Pointer to string to search for
 * @return * Find*
 */
bool vector_string_find(vector_string *vs, char *key) {
  vs_entry_t *current = vs->head;

  while (current != NULL) {
    if (my_str_cmp(current->value, key) == 0) return true;

    current = current->next;
  }
  return false;
}

/**
 * @brief TODO: Insert the string pointed to by key into the vector string.
 *  char* is externally allocated. You do not have to allocate internally
 *
 * @param vs - Pointer to vector_string datastructure
 * @param key - Pointer to string to search for
 * @return * Find*
 */
void vector_string_insert(vector_string *vs, char *key) {
  // Insert the string into the tail of the list.
  vs_entry_t *newWord = (vs_entry_t*)malloc(sizeof(vs_entry_t));

  int keyLength = 0;
  char *tmp = key;

  while (*tmp != '\0') {
    keyLength++;
    tmp++;
  }

  newWord->value = malloc(keyLength + 1); // + 1 for '\0'

  for (int charNum = 0; charNum < keyLength; charNum++) {
    newWord->value[charNum] = key[charNum];
  }
  newWord->value[keyLength] = '\0'; // end the string

  newWord->next = NULL;

  if (!vs->head) { // check if empty list
    vs->head = newWord;
    vs->tail = newWord;
  }

  else { // non-empty list
    vs->tail->next = newWord; // add to end of list
    vs->tail = newWord; // set to tail
  }
}
/**
 * @brief Remove all entries and cleanup vector string
 * TODO: Remove all entries. Remember head and tail are pointers.
 * Remember to remove vs as well; not just entries. or you will have memory
 * leaks.
 * @param vs: Pointer to vector_string struct
 */
void vector_string_deallocate(vector_string *vs) {
  vs_entry_t *current = vs->head;

  while (current != NULL) {
    vs_entry_t *nextNode = current->next; // store next node
    free(current->value);
    free(current);
    current = nextNode;
  }
  free(vs);
}

/**
 * @brief Print all value in each entry of the vector string, in the following
 * format. In this case hello and world are the unique words in the file.
 * 1. hello
 * 2. world
 * @param vs
 */
void vector_string_print(vector_string *vs) {
  int count = 1;
  vs_entry_t *entry = vs->head;

  while (entry != NULL) {
    if (entry->value != NULL) {
      printf("%d. %s\n", count, entry->value);
    }
    count++;
    entry = entry->next;
  }
  
}