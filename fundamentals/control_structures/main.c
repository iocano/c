/*
	Program:
		Control structures examples
        Example of do while, else, for, if, switch, while
	Compile:
		gcc main.c -o control_structures
	-------------------------------------
	Run:
		./control_structures
*/

#include <stdio.h>

void if_example();
void if_else_example();
void switch_example();

void while_example();
void do_while_example();
void for_example();

int main()
{
	//control structures:

	//sequential:	default control structure, statements are executed
	//				line by line in the order in which they appear
    int x = 0;
    int y = 0;
    int total = x + y;
    printf("Total: %i\n", total);

	//selection:	structure is used to test a condition
	//				sequence of statements is executed depending on
	//				whether or not the condition it true or false
	if_example();
	if_else_example();
	switch_example();

	//iteration:	repeatedly executes a series of statements as long
	//				as the condition is true
	while_example();
	do_while_example();
	for_example();
}

void if_example()
{
	int count = 0;

	if (count > 100) // conditions
    {
        // instructions..
    }
}

void if_else_example()
{
	int count = 0;
	if (count < 100)
    {
		// instructions
    }
	else
    {
        // alternative instructions
    }
}

void switch_example()
{
	char suit = 'h';
	switch (suit)
    {
        case 'c':
            // instructions for c
            break;
        case 'd':
            // instructions for d
            break;
        case 'h':
            // instructions for h
            break;
        case 's':
            // instructions for s
            break;
        default:
            // default instrucctions if previous cases are not satisfied
            ;
	}
}

void while_example()
{
	int count = 3;
	while (count > 0)
    {
        // instructions
		count--;
	}
}

void do_while_example()
{
	int x = 0;
	do
    {
        // instructions that runs at least one time
	} while (x++ < 2);
}

void for_example()
{
	int count;
	for (count = 0; count < 3; count++)
    {
        // instructions
	}
}
