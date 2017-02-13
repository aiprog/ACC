/* 
 *  Hash table.
 */

#ifndef TABLE_H
#define TABLE_H

typedef struct data
{
  char *key;
  void *value;
  struct data *next;
} Data;

typedef struct hashTable
{
  int capacity;
  int size;
  Data **slot;
} HashTable;

/* Create a hash table with 'capacity' slots. */
HashTable *hashTableNew (int capacity);

/* Get the capacity of a hash table. */
int hashTableCapacity (HashTable *table);

/* Get the size of a hash table. */
int hashTableSize (HashTable *table);

/* Insert a key to the hash table. */
void hashTableInsert (HashTable *table, char *key, void *value);

/* Search for a key in the hash table. */
void *hashTableSearch (HashTable *table, char *key);

#endif