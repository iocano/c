#include "account.h"

float account_calculate_net_price(float price, short tax_percent)
{
    //add '.0' to '100' to force float
    float tax_rate = 1 + tax_percent / 100.0;
    return price * tax_rate;
}



