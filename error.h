/*
 *  Error message.
 */

#ifndef ERROR_H
#define ERROR_H

#define SIZE  200

typedef struct position
{
  int line;
  int column;
  int begin;
  int offset;
} Position;

enum
{
  EK_NOTE, EK_WARN, EK_ERROR
};

extern char buffer[SIZE];
extern Position position;

/* Initialize the error system. */
void errorInitialize (char *file);

/* Update position informations.  */
void errorUpdatePosition ();

/* Print error messages. */
void errorPrint (int kind, char *message, Position *position);

#endif