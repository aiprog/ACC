#include <stdlib.h>
#include <string.h>

#include "table.h"

/* Create a hash table with 'capacity' slot. */
HashTable *hashTableNew (int capacity)
{
  HashTable *table;

  table = malloc (sizeof (HashTable));
  table->capacity = capacity;
  table->size = 0;
  table->slot = malloc (capacity * sizeof (Data *));
  memset (table->slot, 0, capacity * sizeof (Data *));

  return table;
}

/* Get the capacity of a hash table. */
int hashTableCapacity (HashTable *table)
{
  return table->capacity;
}

/* Get the size of a hash table. */
int hashTableSize (HashTable *table)
{
  return table->size;
}

/* Hash function. */
static int hashTableHash (HashTable *table, char *key)
{
  int hashValue, i;

  for (hashValue = 0, i = 0; key[i] != '\0'; ++i)
    hashValue += key[i];

  return (hashValue & ~(1 << 31)) % table->capacity;
}

/* Insert a key to the hash table. */
void hashTableInsert (HashTable *table, char *key, void *value)
{
  Data *data, *ite, *temp, **slot;
  int hashValue, i;

  if (table->size < table->capacity)
  {
    data = malloc (sizeof (Data));
    data->key = key;
    data->value = value;

    hashValue = hashTableHash (table, key);
    data->next = table->slot[hashValue];
    table->slot[hashValue] = data;

    ++table->size;
  }
  else
  {
    table->capacity *= 2;
    slot = malloc (table->capacity * sizeof (Data *));
    memset (slot, 0, table->capacity * sizeof (Data *));

    for (i = 0; i < table->capacity / 2; ++i)
    {
      if ((ite = table->slot[i]) != NULL)
      {
        while (ite != NULL)
        {
          temp = ite;
          ite = ite->next;

          hashValue = hashTableHash (table, temp->key);
          temp->next = slot[hashValue];
          slot[hashValue] = temp;
        }
      }
    }

    free (table->slot);
    table->slot = slot;

    hashTableInsert (table, key, value);
  }
}

/* Search for a key in the hash table. */
void *hashTableSearch (HashTable *table, char *key)
{
  Data *ite;
  int hashValue;

  hashValue = hashTableHash (table, key);
  for (ite = table->slot[hashValue]; ite != NULL; ite = ite->next)
  {
    if (strcmp (key, ite->key) == 0)
      return ite->value;
  }

  return NULL;
}