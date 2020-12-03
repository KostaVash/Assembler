#ifndef EXTERNAL_C
#define EXTERNAL_C
#include "struct.h"

pexternal reset_ext(pexternal start)
{
    memset(start->name, '\0', LABEL_LENGTH);
    start->adress = 0;

    return start;
}
/*adding the info to the symbol struct */
pexternal update_ext(char *sname, int sadress, pexternal start)
{
    start = (pexternal)malloc(sizeof(symbol));
    if (start != NULL)
    {
        start = reset_ext(start);
        /* start->name = (char *)malloc(sizeof(char) * (strlen(sname) + 1));*/

        strcpy(start->name, sname);
        /*start->name = sname;*/
        start->adress = sadress;
        start->next = NULL;
        return start;
    }
    printf("\nmemory problem ERROER SYMBOL UPDATE!\n");
    return NULL;
}

/*copy the node src to node dest and returning node dest */
pexternal copy_ext(pexternal dest, pexternal src)
{
    dest = update_ext(src->name, src->adress, dest);
    return dest;
}
/*adding the node new to the list start */
pexternal add_to_list_ext(pexternal start, pexternal new)
{
    if (start == NULL)
    {
        start = copy_ext(start, new);
    }
    else
    {
        pexternal tmp;
        tmp = start;
        while (tmp->next != NULL)
            tmp = tmp->next;
        tmp->next = copy_ext(tmp->next, new);
    }
    return start;
}
/*print the list start from the beginning */
void print_list_ext(pexternal start)
{
    pexternal tmp;
    tmp = start;
    while (tmp != NULL)
    {
        printf("\nthe symbol name is %s,and the symbol adress is %d", tmp->name, tmp->adress);
        tmp = tmp->next;
    }
}
/*free the allocated memoty of the list start */
void free_external(pexternal start)
{
    pexternal tmp;

    while (start != NULL)
    {
        tmp = start;
        start = start->next;
        free(tmp);
    }
}
#endif