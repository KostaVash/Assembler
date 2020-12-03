
#ifndef FILEREADER_C
#define FILEREADER_C

#include "header.h"

/*creating all the data types and printing errors */
extern int DC;
extern int IC;
extern psymbol head;
extern Operations myOperations[NUMBER_OF_OPRATIONS];
extern pword listop;
extern pword listdata;
extern pexternal start;

void read_file(char *name)
{
        int lineindex; /*line counter in the file */
        char *as;
        int index;      /*index of the char in the line */
        char *filename; /*file name with the ending */
        FILE *fp;       /*pointer to the file */

        int error;     /*if have error 1 if doesnt 0 */
        int flagerror; /*if we had error in the file it will turn 1 and stay */
        int lineerror; /*if we had a error in amount of chars in line will turn 1 */
        int spaceflag; /*if we have space stays at 1 else changes to 0 */
                       /*reset the pointers*/
        head = NULL;
        listop = NULL;
        listdata = NULL;
        start = NULL;

        as = ".as";
        /*allocates memory for the filename*/
        filename = (char *)malloc(sizeof(char) * (strlen(name) + strlen(as) + 1));
        if (filename != NULL)
                append(name, as, filename);
        else
        {
                fprintf(stdout, "ERROR - cant allocate memory in file %s", name);
                return;
        }
        /*if the file exists so start to word */
        fp = fopen(filename, "r");
        if (fp != NULL)
        {
                /*ch will get the chat and will be assigned in string line in place index*/
                char ch;
                char line[LINE_LENGTH];
                fprintf(stdout, "\nopened file %s\n", name);
                ch = fgetc(fp);
                lineindex = 1; /*reseting all the counters and error flags */
                IC = 0;
                DC = 0;
                error = 0;
                flagerror = 0;
                lineerror = 0;
                spaceflag = 1;

                while ((ch != EOF))
                { /*running all over the file*/
                        index = 0;
                        memset(line, '\0', LINE_LENGTH); /*reseting the string line*/
                        spaceflag = 1;
                        while (ch != '\n')
                        {
                                /*coping every line to the string line*/
                                if (isspace(ch) == 0 && spaceflag == 1) /*ignoring white-space chars*/
                                        spaceflag = 0;
                                if (spaceflag == 0) /*adding only if we skiped the white-space chars in the begining*/
                                {
                                        line[index++] = ch;
                                        /*checking if the line is bigger than LINE_LENGTH*/
                                        if (index > LINE_LENGTH - 1)
                                        {
                                                lineerror = 1;
                                                fprintf(stdout, "\nERROR - line %d too long in file %s \n", lineindex, name);
                                                run_until_end(&ch, &fp);
                                                break;
                                        }
                                }

                                ch = fgetc(fp);
                        }

                        /*if its empty line or note line skip this line */
                        if (is_empty_line(line) || is_note(line))
                        {
                        }
                        else
                        { /*if the line not bigger LINE_LENGTH*/
                                if (lineerror == 0)
                                { /*do the first run*/
                                        error = first_run(line, lineindex, name);
                                        if (error == 0)
                                                flagerror = 1;
                                }
                                else
                                        flagerror = 1;
                        }
                        lineerror = 0;
                        lineindex++;

                        ch = fgetc(fp);
                } /*if we didnt found any errors in first run start the second run*/
                if (flagerror != 1)
                { /*update the symbol table and reseting the file pointer and counters */
                        update_symbol_dc(head, IC);
                        /*reset lineindex and flagerror and start reading the file from the begining*/
                        flagerror = 0;
                        lineindex = 1;
                        spaceflag = 1;
                        IC = 0;
                        fseek(fp, 0, SEEK_SET);

                        ch = fgetc(fp);

                        while (ch != EOF)
                        {
                                /*going all over the file*/
                                index = 0;
                                memset(line, '\0', LINE_LENGTH);
                                spaceflag = 1;

                                while (ch != '\n')
                                {                                               /*coping every line to the string line*/
                                        if (isspace(ch) == 0 && spaceflag == 1) /*ignoring white-space chars*/
                                                spaceflag = 0;
                                        if (spaceflag == 0)
                                        {
                                                line[index++] = ch;

                                                if (index > LINE_LENGTH)
                                                {
                                                        fprintf(stdout, "\nERROR line %d too long in file %s  \n", lineindex, name);
                                                        break;
                                                }
                                        }
                                        ch = fgetc(fp);
                                }
                                /*if the line empty or note line skip it */
                                if (is_empty_line(line) || is_note(line))
                                {
                                }
                                else
                                { /*do the second run*/
                                        error = second_run(line, lineindex, name);
                                        if (error == 0)
                                                flagerror = 1;
                                        lineindex++;
                                }

                                ch = fgetc(fp);
                        }
                        if (flagerror == 0)
                        { /*printing the final input*/
                                listop = merge_list(listop, listdata);
                                print_ob(name);
                                print_ent(name);
                                print_ext(name);
                        }
                }
                fprintf(stdout, "\nclosed file %s\n", name);
                fclose(fp);
        }
        else
        {
                fprintf(stdout, "\nERROR - cant open the file %s\n", name);
        }
        if (filename != NULL)
                free(filename);
}

/*first run return 1 if its ok and 0 if we have error  */
int first_run(char line[], int lineindex, char *filename)
{
        int index; /*index in the line */
        int error; /*if we have error */

        char *label; /*pointer to the label */

        psymbol tmp; /*temporary node for symbols */
        pword tmpw;  /*temporary node for words */

        label = NULL;
        tmp = NULL;
        tmpw = NULL;
        error = 1;
        index = 0;

        index += space_index(line, index);

        /*if it`s label */

        if (is_label_start(line, index) == 1)
        {

                /*getting the label name*/

                int end_label;
                end_label = 0;
                end_label = get_label(line, index);
                /*if the label is right end_label will be bigger than 0 */
                if (end_label != -1)
                {
                        int amount;
                        amount = 0;
                        amount = end_label - index;
                        /*checking the length of label */
                        if (amount > LABEL_LENGTH)
                        {
                                fprintf(stdout, "\nERROR - label too long in line %d in file %s\n", lineindex, filename);
                                return 0;
                        }
                        /*allocate memory for the label */
                        label = (char *)malloc(sizeof(char) * (amount + 1));
                        if (label != NULL)
                        { /*copying the label name from line to label*/
                                memset(label, '\0', amount + 1);
                                strncpy(label, &line[index], amount);
                        }
                        else
                                fprintf(stdout, "\nmemory problem");

                        /*skipping spaces*/
                        index = end_label;
                        index++;                                        /*for the :*/
                        if (line[index] == '\n' || line[index] == '\0') /*if we dont have anything after the label*/
                        {
                                fprintf(stdout, "\nERROR - unused label in line %d in file %s\n", lineindex, filename);
                                if (tmp != NULL)
                                        free(tmp);
                                return 0;
                        }
                        if (check_space(line, index) == 0) /*if we dont have space after the definiton of the label (:) */
                        {
                                fprintf(stdout, " in line %d in file %s\n", lineindex, filename);
                                if (tmp != NULL)
                                        free(tmp);
                                return 0;
                        }
                        index += space_index(line, index);
                }
                else
                {
                        fprintf(stdout, "\nERROR - wrong label definition in line %d in file %s\n", lineindex, filename);
                        if (tmp != NULL)
                                free(tmp);
                        return 0;
                }
        }

        /*if its .data*/

        if (is_data(line, index) == 1)
        {
                int res; /*saves the length of data*/

                /*if its have label so add to the symbol list*/

                error = update_label(label, tmp, DC, 1, 0);

                if (error == 0)
                {
                        fprintf(stdout, " in line %d in file %s\n", lineindex, filename);
                        return error;
                }
                index += (Data + 1); /*.data length is 5 chars*/
                if (check_space(line, index) == 0)
                {
                        fprintf(stdout, " in line %d in file %s\n", lineindex, filename);
                        return 0;
                }
                index += space_index(line, index); /*skipping spaces */
                res = data_len(line, index);
                if (res > 0)
                {

                        short data; /*save the number from .data */

                        /*getting the numbers data */
                        while (line[index] != '\0' || line[index] != '\n')
                        {
                                data = 0;
                                data = get_num(line, index, 1);
                                if (data != NumError)
                                { /*creating the word to every number in data and increasing DC */
                                        DC++;
                                        tmpw = create_word(tmpw, data);
                                        listdata = add_to_list_word(listdata, tmpw);
                                        index += skip_char(line, index, 1); /*skipping digits*/
                                        index += space_index(line, index);  /*skipping spaces */
                                        if (line[index] == '\0' || line[index] == '\n')
                                        {
                                                free(tmpw);
                                                return 1;
                                        }
                                        index++;
                                        index += space_index(line, index); /*skipping spaces */
                                        if (tmpw != NULL)
                                        {
                                                free(tmpw);
                                        }
                                }
                                else
                                {
                                        fprintf(stdout, "\nERROR - the number is out of the range in line %d in file %s\n", lineindex, filename);
                                        return 0;
                                }
                        }
                }
                else
                {
                        if (res == -100 || res == -200)
                                fprintf(stdout, "\nERROR - not integer type in data in line %d in file %s\n", lineindex, filename);
                        else if (res == -300)
                                fprintf(stdout, "\nERROR - empty data input in line %d in file %s\n", lineindex, filename);
                        else if (res == -400)
                                fprintf(stdout, "\nERROR - data not terminated correctly in line %d in file %s\n", lineindex, filename);

                        else
                                fprintf(stdout, "\nERROR - illegal comma in line %d in file %s\n", lineindex, filename);
                        return 0;
                }
        }

        /*if its .string*/

        if (is_string(line, index) == 1)
        {
                int res;

                /*if its have label so add to the symbol list*/

                error = update_label(label, tmp, DC, 1, 0);
                if (error == 0)
                {
                        fprintf(stdout, " in line %d in file %s\n", lineindex, filename);
                        return error;
                }
                index += (String + 1); /*.string length is 7 chars*/
                if (check_space(line, index) == 0)
                {
                        fprintf(stdout, " in line %d in file %s\n", lineindex, filename);
                        return 0;
                }
                index += space_index(line, index); /*skipping spaces */
                res = string_len(line, index);
                if (res > 0)
                {
                        short data; /*saves the asci code of the letter */

                        DC += res;
                        while (line[index] != '\0' && line[index] != '\n')
                        {
                                data = 0;
                                /*adding the string chars to the listdata */
                                if ((line[index] != 34) && StringCheck)
                                { /*creating the words for every char in the string and increasing Dc */
                                        data = get_short_letter(line[index]);

                                        tmpw = create_word(tmpw, data);
                                        listdata = add_to_list_word(listdata, tmpw);
                                        if (tmpw != NULL)
                                        {

                                                free(tmpw);
                                        }
                                }
                                index++;
                        } /*adding the zero to the end of the string */
                        tmpw = create_word(tmpw, 0);
                        DC++; /*for the ending 0 in the string*/
                        listdata = add_to_list_word(listdata, tmpw);
                        if (tmpw != NULL)
                        {

                                free(tmpw);
                        }
                }
                else
                {
                        if (res == -100)
                                fprintf(stdout, "\nERROR - no string definition in line %d in file %s\n", lineindex, filename);
                        else if (res == -200)
                                fprintf(stdout, "\nERROR - the string is not terminated correctly in line %d in file %s\n", lineindex, filename);
                        else if (res == -300)
                                fprintf(stdout, "\nERROR - the string is empty in line %d in file %s\n", lineindex, filename);
                        else
                                fprintf(stdout, "\nERROR - not valied char in the string in line %d in file %s\n", lineindex, filename);

                        return 0;
                }
                return 1;
        }
        /*if its entry */
        if (is_entry(line, index) == 1)
        {

                return 1;
        }
        /*if its extern*/
        if (is_extern(line, index) == 1)
        {
                int end_label;
                end_label = 0;
                index += (Extern + 1);
                if (check_space(line, index) == 0)
                {
                        fprintf(stdout, " in line %d in file %s\n", lineindex, filename);
                        return 0;
                }                                               /*.extern is 7 chars*/
                index += space_index(line, index);              /*skipping spaces */
                end_label = get_label_in_sentence(line, index); /*getting the label name*/

                if (end_label != -1)
                {

                        label = (char *)malloc(sizeof(char) * (end_label + 1));
                        if (label != NULL)
                        { /*saving the label from line to label*/
                                memset(label, '\0', end_label + 1);
                                strncpy(label, &line[index], end_label);
                        }
                        else
                                fprintf(stdout, "\nmemory problem");

                        index += end_label;
                        index += space_index(line, index); /*skipping spaces*/
                        index++;
                        /*addind to the symbol table*/

                        error = update_label(label, tmp, 0, 0, 01);
                        if (error == 0)
                        {
                                fprintf(stdout, " in line %d in file %s\n", lineindex, filename);
                                return error;
                        }
                }
                return 1;
        }
        /*is opcode*/
        if (is_opcode(line, index, 3) != -1)
        {
                short test; /*the coding of the word */
                int opcode; /*the opcode of the sentence*/

                /*if its have label so add to the symbol list*/

                error = update_label(label, tmp, IC + AddressBuffer, 0, 3);
                if (error == 0)
                {
                        fprintf(stdout, " in line %d in file %s\n", lineindex, filename);
                        return error;
                }
                IC++;                               /*increasing IC */
                opcode = is_opcode(line, index, 3); /*getting the op code */
                                                    /*cases by the number of operands*/
                switch (myOperations[opcode].Op_Number)
                {
                case 2:
                {
                        int op1, op2, type1, type2; /*for the 2 oprands after the opcode*/

                        char *label1, *label2;
                        label1 = NULL;
                        label2 = NULL;
                        op1 = 0;
                        op2 = 0;
                        type1 = 0;
                        type2 = 0;
                        test = 0;
                        index += OpCode;
                        if (check_space(line, index) == 0)
                        {
                                fprintf(stdout, " in line %d in file %s\n", lineindex, filename);
                                return 0;
                        }                                  /*3 letter for the op name */
                        index += space_index(line, index); /*skipping spaces*/

                        type1 = address_code(line, index); /*getting the address code of the first operand*/

                        if (type1 == -1)
                        {
                                fprintf(stdout, "\nERROR - illegal operand in line %d in file %s\n", lineindex, filename);
                                return 0;
                        }

                        if (myOperations[opcode].Source[type1] == 0)
                        {
                                fprintf(stdout, "\nERROR - wrong source address in line %d in file %s\n", lineindex, filename);
                                return 0;
                        }
                        /*skipping chars and checkinng the first operand */
                        error = get_operand(type1, &op1, &label1, line, &index, 1); /*getting the operand info */
                        if (error == 0)
                        {
                                fprintf(stdout, " in line %d in file %s\n", lineindex, filename);
                                return error;
                        }

                        /*start of checking commas*/
                        index += space_index(line, index); /*skipping spaces*/
                        if (is_comma(line, index) == 1)
                                index++;
                        else
                        {
                                fprintf(stdout, "\nERROR - Missing commas in line %d in file %s\n", lineindex, filename);
                                return 0;
                        }

                        index += space_index(line, index); /*skipping spaces*/
                        if (is_comma(line, index) == 1)
                        {
                                fprintf(stdout, "\nERROR - Multuple consecutive comma in line %d in file %s\n", lineindex, filename);
                                return 0;
                        }
                        /*end of checking commas*/
                        /*second operand */
                        /*TODO : make this check as function*/
                        /*start of checking operand 2*/
                        type2 = address_code(line, index); /*getting the second opperand address code */

                        if (type2 == -1)
                        {
                                fprintf(stdout, "\nERROR - illegal operand in line %d in file %s\n", lineindex, filename);
                                return 0;
                        }

                        if (myOperations[opcode].Destination[type2] == 0)
                        {
                                fprintf(stdout, "\nERROR - wrong destination address in line %d in file %s\n", lineindex, filename);
                                return 0;
                        }
                        /*getting onfo about the second operand */
                        error = get_operand(type2, &op2, &label2, line, &index, 1);
                        if (error == 0)
                        {
                                fprintf(stdout, " in line %d in file %s\n", lineindex, filename);
                                return error;
                        }
                        /*end of checking operand 2*/
                        /*skipping spaces */
                        index += space_index(line, index); /*skipping spaces*/
                        if (label1 != NULL)
                                free(label1);
                        if (label2 != NULL)
                                free(label2);
                        /*updating the IC*/
                        error = update_ic(type1, type2, op1, op2);
                        if (error == 0)
                        {
                                fprintf(stdout, " in line %d in file %s\n", lineindex, filename);
                                return error;
                        }
                        /*creating the first word and adding to the list */
                        test = first_word(opcode, type1, type2);
                        tmpw = create_word(tmpw, test);

                        if (tmpw != NULL)
                        {

                                listop = add_to_list_word(listop, tmpw);
                                free(tmpw);
                        } /*if both operand are registers */
                        if ((type1 == 2 || type1 == 3) && (type2 == 2 || type2 == 3))
                        {
                                test = calculate_data(type1, type2, op1, op2, label1, label2);
                                tmpw = create_word(tmpw, test);
                                if (tmpw != NULL)
                                {

                                        listop = add_to_list_word(listop, tmpw);
                                        free(tmpw);
                                }
                        }
                        else
                        { /*creating and adding the words in other cases */
                                if (type1 != -1)
                                {
                                        test = calculate_data(type1, -1, op1, -1, label1, label2);
                                        tmpw = create_word(tmpw, test);
                                        if (tmpw != NULL)
                                        {

                                                listop = add_to_list_word(listop, tmpw);
                                                free(tmpw);
                                        }
                                        if (type2 != -1)
                                        {
                                                test = calculate_data(-1, type2, -1, op2, label1, label2);
                                                tmpw = create_word(tmpw, test);
                                                if (tmpw != NULL)
                                                {

                                                        listop = add_to_list_word(listop, tmpw);
                                                        free(tmpw);
                                                }
                                        }
                                }
                        }
                        /*check if the end of the line is empty */
                        if (line[index] == '\n' || line[index] == '\0')
                                return 1;
                        else
                        {
                                fprintf(stdout, "\nERROR - Extraneous text after end of command in line %d in file %s\n", lineindex, filename);
                                return 0;
                        }
                }
                /*1 operand*/
                case 1:
                {
                        int op1, type1; /*saving the data for the oprands after the opcode*/
                        op1 = 0;
                        type1 = 0;
                        index += OpCode;
                        if (check_space(line, index) == 0)
                        {
                                fprintf(stdout, " in line %d in file %s\n", lineindex, filename);
                                return 0;
                        }                                  /*3 letter for the op name */
                        index += space_index(line, index); /*skipping spaces*/

                        type1 = address_code(line, index);

                        if (type1 == -1)
                        {
                                fprintf(stdout, "\nERROR - illegal operand in line %d in file %s\n", lineindex, filename);
                                return 0;
                        }

                        if (myOperations[opcode].Destination[type1] == 0)
                        {
                                fprintf(stdout, "\nERROR - wrong destination address in line %d in file %s\n", lineindex, filename);
                                return 0;
                        }
                        /*skipping chars and checking the operand */
                        /* checking operand 1*/
                        error = get_operand(type1, &op1, &label, line, &index, 1);
                        if (error == 0)
                        {
                                fprintf(stdout, " in line %d in file %s\n", lineindex, filename);
                                return error;
                        }
                        index += space_index(line, index); /*skipping spaces*/
                        update_ic(-1, type1, op1, -1);     /*updating ic */

                        test = first_word(opcode, -1, type1); /*creating the first word */

                        tmpw = create_word(tmpw, test);

                        if (tmpw != NULL)
                        {
                                listop = add_to_list_word(listop, tmpw); /*adding to the op list */
                                free(tmpw);
                        }
                        if (type1 != -1)
                        {
                                test = calculate_data(-1, type1, -1, op1, NULL, NULL); /*reating the word and adding to the list */
                                tmpw = create_word(tmpw, test);

                                if (tmpw != NULL)
                                {

                                        listop = add_to_list_word(listop, tmpw);
                                        free(tmpw);
                                }
                                /*check if the end of the line is empty */
                                if (line[index] == '\n' || line[index] == '\0')
                                        return 1;
                                else
                                {
                                        fprintf(stdout, "\nERROR - Extraneous text after end of command in line %d in file %s\n", lineindex, filename);
                                        return 0;
                                }
                        }
                }
                /*no operands*/
                case 0:
                {
                        if (opcode == 14)
                                index += OpCode;
                        /*rts is 3 chars */
                        if (opcode == 15)
                                index += Stop;             /*stop is 4 chars */
                        index += space_index(line, index); /*skipping spaces*/
                        test = first_word(opcode, -1, -1);

                        tmpw = create_word(tmpw, test);

                        if (tmpw != NULL)
                        {

                                listop = add_to_list_word(listop, tmpw);
                                free(tmpw);
                        }

                        if (line[index] == '\n' || line[index] == '\0')
                        {

                                return 1;
                        }
                        else
                        {
                                fprintf(stdout, "\nERROR - Extraneous text after end of command in line %d in file %s\n", lineindex, filename);
                                return 0;
                        }
                }
                }
        }

        if (tmp != NULL)
                free(tmp);
        fprintf(stdout, "\nERROR - Wrong sentence in line %d in file %s\n", lineindex, filename);
        return 0;
}
/*retrun 1 if there no errror and 0 if there error  */
int second_run(char line[], int lineindex, char *filename)
{
        /*index - for the index in the line , label - save the label , ext if we have extern , tmp - for the symbol , tmpw - for the codding of the word */
        int index;
        char *label;
        pexternal ext;
        psymbol tmp;
        pword tmpw;
        /*reseting */
        label = NULL;
        tmp = NULL;
        tmpw = NULL;
        ext = NULL;

        index = 0;
        index += space_index(line, index);

        if (is_label(line, index) == 1)
        {

                /*skipping the label name*/

                int end_label;
                end_label = 0;
                end_label = get_label(line, index);

                if (end_label != -1)
                {

                        /*skipping label and spaces*/
                        index = end_label;
                        index++;
                        index += space_index(line, index);
                }
        }

        /*if its extern or data or string*/
        if (is_extern(line, index) == 1 || is_data(line, index) == 1 || is_string(line, index) == 1)
                return 1;

        /*if its entry*/
        if (is_entry(line, index) == 1)
        {
                int end_label;
                end_label = 0;
                index += (Entry + 1);
                if (check_space(line, index) == 0)
                {
                        fprintf(stdout, " in line %d in file %s\n", lineindex, filename);
                        return 0;
                }                                               /*.entry is 6 chars*/
                index += space_index(line, index);              /*skipping spaces */
                end_label = get_label_in_sentence(line, index); /*getting the label name*/

                if (end_label != -1) /*if the label is ok */
                {

                        label = (char *)malloc(sizeof(char) * (end_label + 1));
                        if (label != NULL)
                        {
                                memset(label, '\0', end_label + 1);
                                strncpy(label, &line[index], end_label);
                        }
                        else
                                fprintf(stdout, "\nmemory problem");

                        index += end_label;
                        index += space_index(line, index); /*skipping spaces*/
                        index++;
                } /*updaing entry labels */
                tmp = search_symbol(label, head);
                if (tmp != NULL)
                {
                        tmp->symbolid = ENTRY;
                        return 1;
                }
                else
                {
                        fprintf(stdout, "\nERROR - did not found the label in symbol table in line %d in file %s\n", lineindex, filename);
                        return 0;
                }
        } /*end of entry*/

        if (is_opcode(line, index, 3) != -1)
        { /*start of op code*/

                int opcode;

                /*if its have label so add to the symbol list*/

                IC++;
                opcode = is_opcode(line, index, 3);

                switch (myOperations[opcode].Op_Number)
                {
                case 2:
                {
                        int op1, op2, type1, type2; /*for the 2 oprands after the opcode*/

                        char *label1, *label2;
                        label1 = NULL;
                        label2 = NULL;
                        op1 = 0;
                        op2 = 0;
                        type1 = 0;
                        type2 = 0;

                        index += OpCode;                   /*3 letter for the op name */
                        index += space_index(line, index); /*skipping spaces*/

                        type1 = address_code(line, index);

                        /*skipping chars and checkinng the first operand */
                        get_operand(type1, &op1, &label1, line, &index, 2);
                        /*saving the external info*/
                        if (label1 != NULL)
                                if (search_extern_symbol(label1, head) == 1)
                                {
                                        ext = update_ext(label1, IC + AddressBuffer, ext);
                                        start = add_to_list_ext(start, ext);
                                        if (ext != NULL)
                                                free(ext);
                                        ext = NULL;
                                }
                        IC++;
                        if (type1 == 1)
                        {
                                tmpw = search_word(IC, listop);
                                tmpw = update_word(head, tmpw, label1, IC);
                                if (tmpw == NULL)
                                {
                                        if (label1 != NULL)
                                                free(label1);
                                        fprintf(stdout, " in line %d in file %s\n", lineindex, filename);
                                        return 0;
                                }
                        }

                        /*start of checking commas*/
                        index += space_index(line, index); /*skipping spaces*/
                        if (is_comma(line, index) == 1)
                                index++;

                        index += space_index(line, index); /*skipping spaces*/

                        /*end of checking commas*/
                        /*second operand */
                        /*TODO : make this check as function*/
                        /*start of checking operand 2*/
                        type2 = address_code(line, index);

                        /*skipping chars and checkinng the second operand */
                        get_operand(type2, &op2, &label2, line, &index, 2);

                        if ((type1 != 2 && type1 != 3) || (type2 != 2 && type2 != 3))

                                /*saving the external info*/
                                if (label2 != NULL)
                                        if (search_extern_symbol(label2, head) == 1)
                                        {
                                                ext = update_ext(label2, IC + AddressBuffer, ext);
                                                start = add_to_list_ext(start, ext);
                                                if (ext != NULL)
                                                        free(ext);
                                                ext = NULL;
                                        }
                        if ((type1 != 2 && type1 != 3) || (type2 != 2 && type2 != 3))
                                IC++;
                        if (type2 == 1)
                        {
                                tmpw = search_word(IC, listop);
                                tmpw = update_word(head, tmpw, label2, IC);
                                if (tmpw == NULL)
                                {
                                        if (label2 != NULL)
                                                free(label2);
                                        fprintf(stdout, " in line %d in file %s\n", lineindex, filename);
                                        return 0;
                                }
                        }

                        index += space_index(line, index); /*skipping spaces*/
                        if (label1 != NULL)
                                free(label1);
                        if (label2 != NULL)
                                free(label2);
                        return 1;
                        break;
                }
                /*1 operand*/
                case 1:
                {
                        int op1, type1; /*for the 2 oprands after the opcode*/
                        op1 = 0;
                        type1 = 0;
                        index += OpCode;                   /*3 letter for the op name */
                        index += space_index(line, index); /*skipping spaces*/

                        type1 = address_code(line, index);

                        /*skipping chars and checkinng the first operand */
                        /* checking operand 1*/

                        get_operand(type1, &op1, &label, line, &index, 2);

                        /*saving the external info*/
                        if (label != NULL)
                                if (search_extern_symbol(label, head) == 1)
                                {
                                        ext = update_ext(label, IC + AddressBuffer, ext);
                                        start = add_to_list_ext(start, ext);
                                        if (ext != NULL)
                                                free(ext);
                                        ext = NULL;
                                }
                        IC++;
                        if (type1 == 1)
                        {
                                tmpw = search_word(IC, listop);

                                tmpw = update_word(head, tmpw, label, IC);
                                if (tmpw == NULL)
                                {
                                        fprintf(stdout, " in line %d in file %s\n", lineindex, filename);
                                        if (label != NULL)
                                                free(label);
                                        return 0;
                                }
                        }
                        if (label != NULL)
                                free(label);
                        return 1;
                        break;
                }
                /*no operands*/
                case 0:
                {
                        if (opcode == 14)
                                index += OpCode;
                        /*rts is 3 chars */
                        if (opcode == 15)
                                index += Stop;             /*stop is 4 chars */
                        index += space_index(line, index); /*skipping spaces*/

                        return 1;
                        break;
                }
                }

        } /*end of opcode*/
        return 0;
}

/*reseting the counter and free allocate memory */
void reset_all(void)
{
        DC = 0;
        IC = 0;
        free_symbol(head);
        free_word(listop);
        free_external(start);
}

#endif
