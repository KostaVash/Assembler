#ifndef HEADER_H
#define HEADER_H

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include <ctype.h>

#include "struct.h"
#define SetBit(A, k) (A |= (1 << (k)))
#define ClearBit(A, k) (A &= ~(1 << (k)))
#define PrintBit(A, k) (((A & (1 << (k))) != 0) ? 1 : 0)
/*define of numbers*/
#define BigRangeNum 16383
#define NegBigRangeNum -16384
#define SmallRangeNum 2047
#define NegSmallRangeNum -2048
#define NumError 16384
#define AddressBuffer 100
#define OpCode 3
#define Stop 4
#define Data 4
#define Entry 5
#define Extern 6
#define String 6
#define StringCheck ((line[index] >= 32 && line[index] <= 126) || (line[index] == 9))/*checks if the char is valid */
/*bit operations*/
#define SetBit(A, k) (A |= (1 << (k)))
#define ClearBit(A, k) (A &= ~(1 << (k)))
#define PrintBit(A, k) (((A & (1 << (k))) != 0) ? 1 : 0)

/*global prameters*/
int DC;                                       /*data counter */
int IC;                                       /*Imstructiom counter*/
psymbol head;                                 /*pointer to the symbol table */
Operations myOperations[NUMBER_OF_OPRATIONS]; /*operation table*/
pword listop;                                 /*pointer to instruction words */
pword listdata;                               /*pointer to data words*/
pexternal start;                              /*pointer to external labels */

/*fileReader.c*/
void read_file(char *);
int first_run(char[], int, char *);
int second_run(char[], int, char *);
void reset_all(void);

/*data.c*/
int is_opcode(char[], int, int);
int get_label(char[], int);
int is_label(char[], int);
int is_label_start(char[], int);
int is_register(char[], int);
int is_empty_line(char[]);
int address_code(char[], int);
int is_note(char[]);
int is_extern(char[], int);
int is_entry(char[], int);
int is_data(char[], int);
int is_string(char[], int);
int data_len(char[], int);
int string_len(char[], int);
int get_label_in_sentence(char[], int);
int skip_char(char[], int, int);
char *get_label_name(char[], int, char *);
short get_num(char[], int, int);
int update_ic(int, int, int, int);
void update_symbol_dc(psymbol, int);
int update_label(char *, psymbol, int, int, int);
int get_operand(int, int *, char **, char[], int *, int);

/*Symbols.c*/
psymbol reset(psymbol);
psymbol update(char[], int, int, int, psymbol);
psymbol copy(psymbol, psymbol);
int search(psymbol, char *);
psymbol add_to_list(psymbol, psymbol);
void print_list(psymbol);
psymbol search_symbol(char *, psymbol);
int search_in_symbol(char *, psymbol);
int have_entry(psymbol);
int search_extern_symbol(char *, psymbol);
void free_symbol(psymbol);

/*words.c*/
pword reset_word(pword);
pword create_word(pword, short);
pword copy_word(pword, pword);
pword add_to_list_word(pword, pword);
short calculate_data(int, int, int, int, char *, char *);
short first_word(int, int, int);
short get_short_num(int);
short get_short_letter(char);
void print_list_word(pword);
pword search_word(int, pword);
pword update_word(psymbol, pword, char *, int);
void free_word(pword);
pword merge_list(pword, pword ad);

/*external.c*/
pexternal reset_ext(pexternal);
pexternal update_ext(char *, int, pexternal);
pexternal copy_ext(pexternal, pexternal);
pexternal add_to_list_ext(pexternal, pexternal);
void print_list_ext(pexternal);
void free_external(pexternal);

/*tools.c*/
void append(char *, char *, char *);
int space_index(char[], int);
int is_comma(char[], int);
unsigned int convertToOctal(unsigned short);
void run_until_end(char *, FILE **);
int check_space(char[], int);


/*print.c*/
void print_ob(char *);
void print_ent(char *);
void print_ext(char *);

#endif
