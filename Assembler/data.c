#ifndef DATA_C
#define DATA_C
#include "header.h"
extern int DC;
extern int IC;
extern int SymbolCount;
extern int EXT_COUNT;
extern int ENTERNAL_COUNT;
extern psymbol head;
/*all the funtions that extract data from the text file */
/*operations table - saves all the info about the opcodes*/
Operations myOperations[] =
    {
        {"mov", "0000", 2, {1, 1, 1, 1}, {0, 1, 1, 1}},  /*0*/
        {"cmp", "0001", 2, {1, 1, 1, 1}, {1, 1, 1, 1}},  /*1*/
        {"add", "0010", 2, {1, 1, 1, 1}, {0, 1, 1, 1}},  /*2*/
        {"sub", "0011", 2, {1, 1, 1, 1}, {0, 1, 1, 1}},  /*3*/
        {"lea", "0100", 2, {0, 1, 0, 0}, {0, 1, 1, 1}},  /*4*/
        {"clr", "0101", 1, {0, 0, 0, 0}, {0, 1, 1, 1}},  /*5*/
        {"not", "0110", 2, {0, 0, 0, 0}, {0, 1, 1, 1}},  /*6*/
        {"inc", "0111", 1, {0, 0, 0, 0}, {0, 1, 1, 1}},  /*7*/
        {"dec", "1000", 1, {0, 0, 0, 0}, {0, 1, 1, 1}},  /*8*/
        {"jmp", "1001", 1, {0, 0, 0, 0}, {0, 1, 1, 0}},  /*9*/
        {"bne", "1010", 1, {0, 0, 0, 0}, {0, 1, 1, 0}},  /*10*/
        {"red", "1011", 1, {0, 0, 0, 0}, {0, 1, 1, 1}},  /*11*/
        {"prn", "1100", 1, {0, 0, 0, 0}, {1, 1, 1, 1}},  /*12*/
        {"jsr", "1101", 1, {0, 0, 0, 0}, {0, 1, 1, 0}},  /*13*/
        {"rts", "1110", 0, {0, 0, 0, 0}, {0, 0, 0, 0}},  /*14*/
        {"stop", "1111", 0, {0, 0, 0, 0}, {0, 0, 0, 0}}, /*15*/
};
/*retrun the number of the opcode or -1 if not found , type 1 is for space , type 2 is for :, type 3 is without any char after the opcode*/
int is_opcode(char line[], int index, int type)
{

        char tmp[5]; /*saves the name of the opcode*/
        int i;       /*index of opcode*/

        memset(tmp, 0, sizeof(tmp));
        strncpy(tmp, &line[index], Stop); /*for stop */
        if (strcmp(tmp, myOperations[15].Name) == 0)
        {
                if (type == 1 && isspace(line[index + Stop]) != 0)
                        return 15;
                if (type == 2 && line[index + Stop] == ':')
                        return 15;
                if (type == 3)
                        return 15;
        }

        memset(tmp, '\0', sizeof(tmp));
        strncpy(tmp, &line[index], OpCode);
        /*for the reset of the cases*/
        for (i = 0; i < 15; i++)
                if (strcmp(tmp, myOperations[i].Name) == 0)
                {
                        if (type == 1 && isspace(line[index + OpCode]) != 0)
                                return i;
                        if (type == 2 && line[index + OpCode] == ':')
                                return i;
                        if (type == 3)
                                return i;
                }
        return -1;
}
/*return the index of the end of the LABEL , or -1 if not found or if its a error */
int get_label(char line[], int index)
{ /*if the label is register*/
        if (is_register(line, index) != -1)
                if (line[index + 2] == ':')
                        return -1;
        /*if the label is opcode */
        if (is_opcode(line, index, 2) != -1)
                return -1;

        /*if the label is extern*/
        if (strncmp("extern", &line[index], Extern) == 0)
                if (line[index + Extern] == ':')
                        return -1;

        /*if the label is entry*/
        if (strncmp("entry", &line[index], Entry) == 0)
                if (line[index + Entry] == ':')
                        return -1;
        /*if the label is string*/
        if (strncmp("string", &line[index], String) == 0)
                if (line[index + String] == ':')
                        return -1;
        /*if the label is data*/
        if (strncmp("data", &line[index], Data) == 0)
                if (line[index + Data] == ':')
                        return -1;
        /*running all over the string until : or if it the end of the line */
        while (line[index] != ':' && (line[index] != '\n' || line[index] != '\0'))
        {
                if ((isalpha(line[index]) == 0) && (isdigit(line[index]) == 0))
                        return -1;
                index++;
        }
        /*if it : it return the index of it */
        if (line[index] == ':')
                return index;
        return -1;
}

/*return the number of the register or return -1 if its wrong*/
int is_register(char line[], int index)
{
        if (line[index] == 'r')
        {
                if (line[index + 1] == '0')
                        return 0;
                if (line[index + 1] == '1')
                        return 1;
                if (line[index + 1] == '2')
                        return 2;
                if (line[index + 1] == '3')
                        return 3;
                if (line[index + 1] == '4')
                        return 4;
                if (line[index + 1] == '5')
                        return 5;
                if (line[index + 1] == '6')
                        return 6;
                if (line[index + 1] == '7')
                        return 7;
        }
        return -1;
}

/*if its alabel in opcode return 1 it it or else 0 if doesnt */
int is_label(char line[], int index)
{ /*if its the end of the line */
        if (line[index] == '\n' || line[index] == '\0')
                return 0;
        /*its not register , not a # , not a* and not a opcode */

        if ((is_register(line, index) == -1) && (line[index] != '#') && (line[index] != '.') && (line[index] != '*') && is_opcode(line, index, 2) == -1)
        {
                return 1;
        }
        return 0;
}
/*check if its a label in the begining of the sentence*/
int is_label_start(char line[], int index)
{
        while (line[index] != '\n' && line[index] != '\0')
        {
                if (line[index] == ':')
                        return 1;
                index++;
        }
        return 0;
}

/*return the address code between 0 to 3 and -1 if there error*/
int address_code(char line[], int index)
{ /*check if it a didigt */
        if (line[index] == '#')
        {
                if (isdigit(line[index + 1]) != 0 || line[index + 1] == '-' || line[index + 1] == '+')
                        return 0;
                return -1;
        } /*check if its a label */
        if (get_label_in_sentence(line, index) != -1)
                return 1;
        /*check if its a *register*/
        if (line[index] == '*')
        {
                if (is_register(line, index + 1) != -1)
                        return 2;
                else
                        return -1;
        }
        /*check if its a register */
        if (is_register(line, index) != -1)
                return 3;
        return -1;
}

/*if its empty return 1 else 0*/
int is_empty_line(char line[])
{
        int count;
        count = space_index(line, 0);
        if (count == strlen(line))
                return 1;
        return 0;
}

/*return 1 if its note line else 0 */
int is_note(char line[])
{
        int index = 0;
        index += space_index(line, 0);
        if (line[index] == ';')
                return 1;
        return 0;
}

/*if its extern return 1 else 0*/
int is_extern(char line[], int index)
{
        if (line[index] == '.')
                if (strncmp("extern", &line[index + 1], Extern) == 0)
                        return 1;
        return 0;
}

/*if its entry return 1 else 0*/
int is_entry(char line[], int index)
{
        if (line[index] == '.')
                if (strncmp("entry", &line[index + 1], Entry) == 0)
                        return 1;
        return 0;
}

/*if its data return 1 else 0*/
int is_data(char line[], int index)
{
        if (line[index] == '.')
                if (strncmp("data", &line[index + 1], Data) == 0)
                        return 1;
        return 0;
}

/*if its string return 1 else 0*/
int is_string(char line[], int index)
{
        if (line[index] == '.')
                if (strncmp("string", &line[index + 1], Extern) == 0)
                        return 1;
        return 0;
}
/*return the number of digits in the line.return -100 if it letter , -200 if its no a integer*/
int data_len(char line[], int index)
{
        int count, digiflag;
        count = 0;
        digiflag = 0;
        while (line[index] != '\0' || line[index] != '\n')
        {

                index += space_index(line, index);
                /*if it have a digit - digiflag will turn on */
                if ((line[index] == '+' || line[index] == '-') && isdigit(line[index + 1]) != 0)
                {
                        index += 2;
                        digiflag = 1;
                }
                while ((isdigit(line[index]) != 0) && (line[index] != '\0' || line[index] != '\n') && line[index] != ',')
                { /*if its a number*/
                        digiflag = 1;
                        index++;
                }
                index += space_index(line, index);
                if (line[index] == '.') /*if its a double*/
                        return -100;
                else if (isalpha(line[index]) != 0) /*if its letter*/
                        return -200;
                else if (is_comma(line, index) == 1 && digiflag == 0) /*if it have a comma but not a digit*/
                        return -500;
                else if (is_comma(line, index) == 1) /*count++ becuse we have a number */
                        count++;
                else if (digiflag == 0 && count != 0) /*not terminated */
                        return -400;
                else if (digiflag == 0 && count == 0) /*empty data*/
                        return -300;
                else if (line[index] == '\0' || line[index] == '\n')
                {
                        count++;
                        return count;
                }
                digiflag = 0;
                index++;
        }

        return count;
}

/*return the number of chars in the line.return -100 if it doesnt start with " , -200 if its not terminated correctly, -300 if its empty string  and -400 is its not valied char */
int string_len(char line[], int index)
{
        int count;
        count = 0;
        index += space_index(line, index);
        if (line[index] != '\"')
                return -100;
        else /*check if its empty string */
        {
                if (line[index + 1] == '\"')
                        return -300;
        }
        index++;
        while (line[index] != '\0' || line[index] != '\n' || line[index] != '\t')
        {
                /*check if the char is valid */
                if (StringCheck)
                        count++;
                else
                        return -400;

                index++;
                if (line[index] == '\"')
                {
                        index++;
                        index += space_index(line, index);
                        if (((line[index] == '\0' || line[index] == '\n'))) /*if its empty after the end of the string return the amount of char */
                                return count;
                        else
                                return -200;
                }

                else if (line[index] == '\0' || line[index] == '\n')
                {

                        return -200;
                }
        }

        return count;
}
/*retrun the last index of the label or -1 if there no label*/
int get_label_in_sentence(char line[], int index)
{
        int count; /*count how many chars in label*/
        count = 0;
        if (line[index] == '\n' || line[index] == '\0')
                return -1;
        /*its not register , not a # and not a* and not a opcode*/

        if (!((is_register(line, index) == -1) && (line[index] != '#') && (line[index] != '.') && (line[index] != '*') && is_opcode(line, index, 1) == -1))
        {
                return -1;
        }

        if (isalpha(line[index]) == 0)
                return -1;

        while (line[index] != ',' && (line[index] != '\n' || line[index] != '\0') && ((isalpha(line[index]) != 0) || (isdigit(line[index]) != 0)))
        {

                index++;
                count++;
        }
        /*if its bigger than 30 retrun -1 */
        if (count < LABEL_LENGTH - 1)
                return count;
        return -1;
}
/*skipping chars in the line from index until the case and return the number of the index
1 - numbers 
2- chars 
3- comma 
4- end of the line */
int skip_char(char line[], int index, int cases)
{
        int count;
        count = 0;
        switch (cases)
        {
        case 1:
        {
                if (line[index] == '-' || line[index] == '+')
                {
                        index++;
                        count++;
                }
                while (isdigit(line[index]) != 0 && (line[index] != '\0' || line[index] != '\n'))
                {
                        index++;
                        count++;
                }
                return count;
        }
        case 2:
        {
                while (isalpha(line[index]) != 0 && (line[index] != '\0' || line[index] != '\n'))
                {
                        index++;
                        count++;
                }
                return count;
        }
        case 3:
        {
                while (line[index] != ',' && (line[index] != '\0' || line[index] != '\n'))
                {
                        index++;
                        count++;
                }
                return count;
        }
        case 4:
        {
                while (isspace(line[index]) == 0)
                {
                        index++;
                        count++;
                }
                return count;
        }
        }
        return -1; /*if error retrun -1*/
}
/*retrun number between 2047 and -2048 if size is 1 
and between -16384 and 16383 if size is 2 , 16384 for error
for size if its 1 its for 12 bits and if its 2 its for 15 bits */
short get_num(char line[], int index, int size)
{
        int num;
        num = 0;
        if (line[index] == '\0' || line[index] == '\n')
        {
                return NumError;
        }
        sscanf(&line[index], "%d", &num);
        if (size == 1)
                if (num >= NegSmallRangeNum && num <= SmallRangeNum)
                        return num;
        if (size == 2)
                if (num >= NegBigRangeNum && num <= BigRangeNum)
                        return num;
        return NumError;
}
/*return the label name, getting the line and the beggining of the line -index- and the array to save to there the label name */
char *get_label_name(char line[], int index, char *label)
{
        int amount;
        amount = get_label_in_sentence(line, index);

        if (amount != -1)
        {
                label = (char *)malloc(sizeof(char) * (amount + 1));
                if (label != NULL)
                {
                        memset(label, '\0', amount + 1);
                        strncpy(label, &line[index], amount);
                        return label;
                }
        }
        return NULL;
}

/*updating IC counter - opaopaddress1 - is for the source and opaddress2 is for destination same for op1 and op2 */
int update_ic(int opaddress1, int opaddress2, int op1, int op2)
{
        /*when they both registers */
        if ((opaddress1 == 2 || opaddress1 == 3) && (opaddress2 == 2 || opaddress2 == 3))
        {
                if (op1 != -1 && op2 != -1)
                        IC += 1;
        }
        else if (opaddress1 != -1 && opaddress2 != -1)
        {
                IC += 2;
        }
        else if (opaddress1 == -1 && opaddress2 != -1)
        {
                IC += 1;
        }
        if (IC + DC > MEMORY_SIZE)
        {
                printf("\nERROR - over the memory of the machine");
                return 0;
        }
        return 1;
}
/*update the adress of the symbol table to guide sentence*/
void update_symbol_dc(psymbol head, int ic)
{
        psymbol tmp;
        tmp = head;
        while (tmp != NULL)
        {
                if (tmp->guide == 1 && tmp->symbolid == GUIDE)
                        tmp->adress += ic + AddressBuffer;
                tmp = tmp->next;
        }
}
/*adding the label to the symbol table return 1 if succeed and 0 if doesnt */
int update_label(char *label, psymbol tmp, int dc, int guide, int symbolid)
{

        if (label != NULL)
        {

                tmp = update(label, dc, guide, symbolid, tmp);
                if (tmp != NULL)
                {
                        if ((search_in_symbol(label, head)) == 0)
                        {
                                head = add_to_list(head, tmp);
                                free(label);
                        }
                        else
                        {
                                printf("\nERROR - symbol in the symbol table");
                                free(label);
                                return 0;
                        }
                }
        }
        return 1;
}
/*returning 0 if there error and 1 if its ok 
getting the addres type between 0-3 getting the opperand or the label 
the line and the index begin from 
and if its first or second run */
int get_operand(int type1, int *op1, char **label1, char line[], int *index, int run)
{
        int count;
        switch (type1)
        {
        case 0:
        {

                count = 0;
                (*index)++; /*for the # */

                *op1 = get_num(line, *index, 2);
                if (*op1 == NumError)
                {
                        printf("\nERROR - Invalid integer");
                        return 0;
                }
                (*index) += skip_char(line, *index, 1);

                break;
        }
        case 1:
        {

                count = 0;
                count = get_label_in_sentence(line, *index);
                if (count != -1)
                {
                        *label1 = get_label_name(line, *index, *label1);
                        (*index) += count;
                }
                else
                {
                        if (run == 1)
                                printf("\nERROR - wrong label");
                        return 0;
                }
                break;
        }
        case 2:
        {
                (*index)++; /*skipping 1 char beacuse "*" */
                if (is_register(line, *index) != -1)
                {
                        *op1 = is_register(line, *index);
                        (*index) += 2; /*becuse r1 its 2 chars */
                }
                else
                {
                        if (run == 1)
                                printf("\nERROR - not valied register");
                        return 0;
                }
                break;
        }
        case 3:
        {
                if (is_register(line, *index) != -1)
                {
                        *op1 = is_register(line, *index);
                        (*index) += 2; /*becuse r1 its 2 chars */
                }
                else
                {
                        if (run == 1)
                                printf("\nERROR - not valied register");
                        return 0;
                }

                break;
        }
        } /*end of checking operand */

        return 1;
}
#endif
