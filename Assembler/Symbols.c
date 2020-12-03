
#ifndef SYMBOLS_C
#define SYMBOLS_C
#include "struct.h"
#include "header.h"
/*all the funtions of symbol table */
psymbol reset(psymbol head)
{
    memset(head->name, '\0', LABEL_LENGTH);
    head->adress = 0;
    head->guide = 0;
    head->symbolid = 0;
    head->next = NULL;

    return head;
}
/*adding the info to the symbol struct */
psymbol update(char *sname, int sadress, int guide, int symbolid, psymbol head)
{
    head = (psymbol)malloc(sizeof(symbol));
    if (head != NULL)
    {
        head = reset(head);
        /* head->name = (char *)malloc(sizeof(char) * (strlen(sname) + 1));*/

        strcpy(head->name, sname);
        /*head->name = sname;*/
        head->adress = sadress;
        head->guide = guide;
        head->symbolid = symbolid;
        head->next = NULL;
        return head;
    }
    printf("\nmemory problem ERROER SYMBOL UPDATE!\n");
    return NULL;
}

/*return 1 if found in the list ,0 if doesnt*/
int search(psymbol head, char sname[])
{
    psymbol tmp;
    tmp = head;
    while (tmp != NULL)
    {
        if (strcmp(tmp->name, sname) == 0)
            return 1;
        tmp = tmp->next;
    }
    return 0;
}
/*searching the symbol by name , if found return pointer to the symbol elsse return null*/
psymbol search_symbol(char *label, psymbol head)
{

    psymbol tmp;
    tmp = head;
    while (tmp != NULL)
    {
        if (strcmp(tmp->name, label) == 0)
            return tmp;
        tmp = tmp->next;
    }
    return NULL;
}
/*copy the node src to node dest and returning dest */
psymbol copy(psymbol dest, psymbol src)
{
    dest = update(src->name, src->adress, src->guide, src->symbolid, dest);
    return dest;
}
/*adding the node new to the end of the list head */
psymbol add_to_list(psymbol head, psymbol new)
{
    if (head == NULL) /*if the list is empty */
    {
        head = copy(head, new);
    }
    else /*if the list isnt empty*/
    {
        psymbol tmp;
        tmp = head;
        while (tmp->next != NULL)
            tmp = tmp->next;
        tmp->next = copy(tmp->next, new);
    }
    return head;
}
/*print the list from the begining */
void print_list(psymbol head)
{
    psymbol tmp;
    tmp = head;
    while (tmp != NULL)
    {
        printf("\nthe symbol name is %s,and the symbol adress is %d, is it guide sentense %d and its symbolid is %d", tmp->name, tmp->adress, tmp->guide, tmp->symbolid);
        tmp = tmp->next;
    }
}
/*return 1 if its in the symbol table and 0 if it doesnt*/
int search_in_symbol(char *name, psymbol head)
{
    psymbol tmp;
    if (head == NULL)
        return 0;

    tmp = head;
    /*searching in the symbol table*/
    while (tmp != NULL)
    {
        /*if found in the symbol table returns 1 */
        if (strcmp(tmp->name, name) == 0)
            return 1;
        /*going to the next node*/
        tmp = tmp->next;
    }
    return 0;
}
/*if have entry return 1 else 0*/
int have_entry(psymbol head)
{
    psymbol tmp;
    tmp = head;
    while (tmp != NULL)
    {
        if (tmp->symbolid == ENTRY)
            return 1;
        tmp = tmp->next;
    }
    return 0;
}
/*return 1 if found extern symbol in the list and 0 if not found */
int search_extern_symbol(char *label, psymbol head)
{
    psymbol tmp;
    if (head == NULL)
        return 0;

    tmp = head;
    /*searching in the symbol table*/
    while (tmp != NULL)
    {
        /*if found in the symbol table returns 1 and its extern */
        if (strcmp(tmp->name, label) == 0)
            if (tmp->symbolid == EXTERN)
                return 1;
        /*going to the next node*/
        tmp = tmp->next;
    }
    return 0;
}
/*free the allocated memoty in the list head*/
void free_symbol(psymbol head)
{
    psymbol tmp;

    while (head != NULL)
    {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

#endif
