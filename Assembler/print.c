#ifndef PRINT_C
#define PRINT_C
#include "header.h"
/*functions that prints to output */
void print_ob(char *filename)
{
    FILE *fp;
    char *ob;
    char *createfile;

    ob = ".ob";

    /*allocates memory for the filename*/
    createfile = (char *)malloc(sizeof(char) * (strlen(filename) + strlen(ob) + 1));
    if (createfile != NULL)
    {
        append(filename, ob, createfile);

        fp = fopen(createfile, "w");
        if (fp != NULL)
        {
            fprintf(fp, "%d %d\n", IC, DC);

            {
                int num;
                pword tmp;
                tmp = listop;
                num = AddressBuffer;
                while (tmp != NULL)
                {

                    fprintf(fp, "%04d\t%05u\n", num++, convertToOctal(tmp->word));
                    tmp = tmp->next;
                }
            }
            fclose(fp);
        }
        free(createfile);
    }
}

void print_ent(char *filename)
{
    FILE *fp;
    char *ob;
    char *createfile;

    ob = ".ent";
    if (have_entry(head) == 1)
    {
        /*allocates memory for the filename*/
        createfile = (char *)malloc(sizeof(char) * (strlen(filename) + strlen(ob) + 1));
        if (createfile != NULL)
        {
            append(filename, ob, createfile);

            fp = fopen(createfile, "w");
            if (fp != NULL)
            {
                psymbol tmp;
                tmp = head;
                while (tmp != NULL)
                {
                    if (tmp->symbolid == ENTRY)
                        fprintf(fp, "%s %d\n", tmp->name, tmp->adress);
                    tmp = tmp->next;
                }
            }
            fclose(fp);
        }
        free(createfile);
    }
}
void print_ext(char *filename)
{
    FILE *fp;
    char *ob;
    char *createfile;

    ob = ".ext";
    if (start != NULL)
    {
        /*allocates memory for the filename*/
        createfile = (char *)malloc(sizeof(char) * (strlen(filename) + strlen(ob) + 1));
        if (createfile != NULL)
        {
            append(filename, ob, createfile);

            fp = fopen(createfile, "w");
            if (fp != NULL)
            {
                pexternal tmp;
                tmp = start;
                while (tmp != NULL)
                {
                    fprintf(fp, "%s %04d\n", tmp->name, tmp->adress);
                    tmp = tmp->next;
                }
            }
            fclose(fp);
        }
        free(createfile);
    }
}
#endif