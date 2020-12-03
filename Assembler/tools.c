
#ifndef TOOLS_C
#define TOOLS_C

#include "header.h"
#include "struct.h"



/*adding the char add to the char target , and returning the result*/
void append(char *target, char *add, char *result)
{

	if (result != NULL)
	{

		result[0] = '\0';
		strcat(result, target);
		strcat(result, add);
	}
	else
	{
		printf("\nERROR- append - tools\n\n\n");
		result = NULL;
	}
}

/*count how many white spaces we have from index , and returning the amount ofwhite spaces we have */
int space_index(char str[], int index)
{
	int count;
	count = 0;

	while (isspace(str[index]))
	{
		count++;
		index++;
	}
	return count;
}

/*if it comma return 1 else return -1*/
int is_comma(char line[], int index)
{
	if (line[index] == ',')
		return 1;
	else
		return -1;
}
/*converting decimal number to ocatl*/
unsigned int convertToOctal(unsigned short num)
{
	unsigned int mask;
	unsigned int tmp, res, pow;
	int i;
	mask = 7;
	tmp = 0, res = 0, pow = 1;
	for (i = 0; i < 5; i++)
	{
		tmp = num & mask;
		res += tmp * pow;
		pow *= 10;
		num = num >> 3;
	}
	return res;
}
/*running until the end of the line */
void run_until_end(char *ch, FILE **fp)
{
	while (*ch != '\n' && *ch != '\0')
	{ /*running until the end of the line*/

		*ch = fgetc(*fp);
	}
}
/*return 1 if we have space , 0 if we dont */
int check_space(char line[], int index)
{
	int tmp;
	tmp = index;
	index += space_index(line, index);
	if (tmp == index)
	{
		fprintf(stdout, "\nERROR - we dont have space");
		return 0;
	}
	return 1;
}


#endif
