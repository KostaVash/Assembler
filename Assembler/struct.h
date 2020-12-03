
#ifndef STRUCT_H
#define STRUCT_H
#define NUMBER_OF_OPRATIONS 16
#define LABEL_LENGTH 32
#define LINE_LENGTH 81
#define MEMORY_SIZE 4096

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

/*all the data about the structs */
typedef enum { GUIDE,
               EXTERN,
               ENTRY,
               OPCODE } symbolid;

/*struct of word , save the data in 16 bit and have pointer to the next node*/
struct word
{
    unsigned short word;
    struct word *next;
};
typedef struct word word;

typedef word *pword;
/*struct of operations , saves all the data about the operations*/
typedef struct
{
    char *Name;
    char *Op_Code;
    int Op_Number;
    int Source[4];
    int Destination[4];
} Operations;
/*struct of symbol , saves all the data about the label */
struct symbol
{
    char name[LABEL_LENGTH];
    int adress;
    int guide;
    int symbolid; /*0 for guide sentence ,1 for extern , 2 for entry,3 - opcode*/
    struct symbol *next;
};
typedef struct symbol symbol;

typedef symbol *psymbol;
/*struct of external , saves all the data about all the external labels */
struct external
{
    char name[LABEL_LENGTH];
    int adress;
    struct external *next;
};
typedef struct external external;

typedef external *pexternal;
#endif
