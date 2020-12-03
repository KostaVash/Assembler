#include "header.h"

#ifndef MAIN_C
#define MAIN_C

int main(int argc, char **argv)
{
    int i;
    if (argc == 1)
    {
        fprintf(stdout, "\nERROR - not enough arguments\n");
        exit(0);
    }
    /*running all over the files from the user*/
    for (i = 1; i < argc; i++)
    {
        read_file(argv[i]);
        reset_all();
    }
    return 0;
}
#endif