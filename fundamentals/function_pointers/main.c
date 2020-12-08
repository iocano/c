/*
	Program:
        Filter words that start with vowels or consonants
        Example of 3d array, function pointer
	Compile:
		gcc main.c -o filter
	-------------------------------------
	Run:
		./filter
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool start_with_vowel(char *s)
{
    return *s == 'A' || *s == 'E' || *s == 'I' || *s == 'O' || *s == 'U';
}

bool start_with_consonants(char *s)
{
    return !(*s == 'A' || *s == 'E' || *s == 'I' || *s == 'O' || *s == 'U');
}

void find(bool (*filter_function)(char*), void *lists, int lists_size,int row_size, int column_size)
{
    for (int i = 0; i < lists_size; i += row_size)
    {
        char *list = lists + i;
        for (int j = 0; j < row_size; j += column_size)
        {
            char *name = list + j;
            if(filter_function(name))
            {
                printf("%s, ", name);
            }
        }
    }
}

int main()
{
    // 3d array initialization first form
    char names_1[][5][20] =  //  [name_list_count][name_count][char_count]
    {
        {"Edgardo", "Shawn", "Rodrigo", "Ellis", "Emanuel"},
        {"Al", "Ellsworth", "Avery", "Stephan", "Lance"},
        {"Ismael", "Teddy", "Sebastian", "Avery", "Brendan"},
        {"Hai", "Richard", "Jae", "Olen", "Bernard"},
        {"Omer", "Teodoro", "Clinton", "Antione", "Kasey"}
    };

    // 3d array initialization second form
    char *names_2[5][5] =  //  *[name_list_count][name_count]
    {
        {"Edgardo", "Shawn", "Rodrigo", "Ellis", "Emanuel"},
        {"Al", "Ellsworth", "Avery", "Stephan", "Lance"},
        {"Ismael", "Teddy", "Sebastian", "Avery", "Brendan"},
        {"Hai", "Richard", "Jae", "Olen", "Bernard"},
        {"Omer", "Teodoro", "Clinton", "Antione", "Kasey"}
    };

    find(start_with_vowel, names_1, sizeof(names_1), sizeof(*names_1), sizeof(**names_1));
    find(start_with_consonants, names_1, sizeof(names_1), sizeof(*names_1), sizeof(**names_1));
    return 0;
}
