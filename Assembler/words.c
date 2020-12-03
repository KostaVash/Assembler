
#ifndef WORDS_C
#define WORDS_C

#include "header.h"
#define A 2 /*turn bit A in the word */ 

/*resets the data of the node*/
pword reset_word(pword node)
{
    node->word = 0;
    node->next = NULL;
    return node;
}
/*creating the node , whit the data of data, returning NULL if didnt succeed*/
pword create_word(pword node, short data)
{
    node = (pword)malloc(sizeof(word));
    if (node != NULL)
    {
        node = reset_word(node);
        node->word = data;
        node->next = NULL;
        return node;
    }
    return NULL;
}
/*copying node src into dest */
pword copy_word(pword dest, pword src)
{
    dest = create_word(dest, src->word);
    return dest;
}
/*adding the node new to the end of the list head*/
pword add_to_list_word(pword head, pword new)
{
    if (head == NULL)/*if the head of the list is empty */
    {
        head = copy_word(head, new);
    }
    else/*if the list isnt empty*/
    {
        pword tmp = head;
        while (tmp->next != NULL)
            tmp = tmp->next;
        tmp->next = copy_word(tmp->next, new);
    }
    return head;
}
/*calculates the word */
short calculate_data(int src, int trg, int srcop, int trgop, char *label1, char *label2)
{
    short data;
    data = 0;
    /*if they both registers */
    if ((src == 2 || src == 3) && (trg == 2 || trg == 3))
    {
        data = (srcop << 6) | (trgop << 3);
        SetBit(data, A);
        return data;
    }
    /*if the source is register*/
    if ((src == 2 || src == 3))
    {
        data = (srcop << 6);
        SetBit(data, A);
        return data;
    }
    /*if the target is register*/
    if (trg == 2 || trg == 3)
    {
        data = (trgop << 3);
        SetBit(data, A);
        return data;
    } /*if its a number */
    if (trg == 0 && src == -1)
    {
        data = trgop << 3;
        SetBit(data, A);
        return data;
    } /*if its a number */
    if (src == 0 && trg == -1)
    {
        data = srcop << 3;
        SetBit(data, A);
        return data;
    }
    return data;
}
/*creating the first word */
short first_word(int opcode, int src, int dest)
{
    int data;
    data = 0;
    data = opcode << 11;
    if (src != -1 && dest != -1)
    {
        SetBit(data, (src + 7));
        SetBit(data, (dest + 3));
    }
    if (dest != -1 && src == -1)
    {
        SetBit(data, (dest + 3));
    }
    SetBit(data, A);
    return data;
}
/*returning the num in short */
short get_short_num(int num)
{
    short data;
    data = num;
    return data;
}/*returning the char c in short */
short get_short_letter(char c)
{
    short data;
    data = c;
    return data;
}/*print the list from the begining in octal s */
void print_list_word(pword list)
{
    int num;
    pword tmp;
    tmp = list;
    num = AddressBuffer;
    while (tmp != NULL)
    {
        printf(" %04d\t", num++);
        convertToOctal(tmp->word);
        putchar('\n');
        tmp = tmp->next;
    }
}
/*returning the node or NULL if not found */
pword search_word(int index, pword list)
{
    pword tmp;
    int count;
    tmp = list;
    count = 1;
    while (count < index && tmp != NULL)
    {
        tmp = tmp->next;
        count++;
    }
    if (tmp != NULL)
        return tmp;
    else
        return NULL;
}
/*adding the words tmp to the end of the list */
pword update_word(psymbol head, pword tmp, char *label, int index)
{
    psymbol tmps;
    short data;
    if (tmp != NULL && tmp->word == 0)
    {

        tmps = search_symbol(label, head);
        /*if its extern*/
        if (tmps == NULL)
        {
            fprintf(stdout, "\nERROR the label %s did not found", label);
            return NULL;
        }
        if (tmps->symbolid == EXTERN)
        {
            tmp->word = 1;
        } /*entry or opcode and guide sentence */
        if (tmps->symbolid == ENTRY || tmps->symbolid == OPCODE || (tmps->guide == 1 && tmps->symbolid == GUIDE))
        {

            data = tmps->adress;
            data = data << 3;
            SetBit(data, 1);
            tmp->word = data;
        }
    }
    return tmp;
}
/*free every node in the list */
void free_word(pword list)
{
    pword tmp;

    while ((tmp = list) != NULL)
    {
        list = list->next;
        free(tmp);
    }
}
/*merging 2 list adding the list add to the end of the list dest*/
pword merge_list(pword dest, pword add)
{
    pword tmp;
    tmp = dest;
    while (tmp->next != NULL)
        tmp = tmp->next;
    tmp->next = add;
    return dest;
}

#endif