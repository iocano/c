/*
    Program:
        Calculate total amount with tax of n values
    Compile:
        gcc main.c account.c -o total_account_balance
    -------------------------------------
    Run:
        ./total_account_balance
    End:
        Ctrl-d
*/

#include <stdio.h>
#include "account.h"


int main()
{
    float total = 0;
    short count = 0;
    float input;
    printf("Enter item price: ");
    // if user press Ctrl-d SO send EOF signal to program
    while (scanf("%f", &input) == 1)
    {
        total += account_calculate_net_price(input, 6);
        count++;
        printf("Current total: %.2f\n", total);
        printf("Enter item price: ");
    }
    printf("\rNumber of items: %hi", count);
    printf("\nTotal: %.2f\n", total);
    return 0;
}
