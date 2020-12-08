/*
    Program:
        Initialize two product_t structs then displays its data
        Example of const, enum, struct, typedef, union
    Compile:
        gcc main.c -o products
    -------------------------------------
    Run:
        ./products
*/

#include <stdio.h>

typedef enum
{
    UNIT_KILOGRAM,
    UNIT_PIECES
} unit_t;

typedef union
{
    float kilograms;
    int pieces;
} quantity_t;

typedef struct
{
    const char *name;
    unit_t unit;
    quantity_t quantity;
} product_t;

void print_product(product_t product);

int main()
{
    product_t product_1 = {"salmon", UNIT_KILOGRAM, .quantity.kilograms = 2.5};
    // product_t product_1 = {"salmon", UNIT_KILOGRAM, .quantity = {.kilograms = 2.5}};
    product_t product_2 = {"eggs", UNIT_PIECES, .quantity.pieces = 12};
    // product_t product_2 = {"eggs", UNIT_PIECES, .quantity = {.pieces = 12}};

    print_product(product_1);
    print_product(product_2);

    return 0;
}

void print_product(product_t product)
{
    if (product.unit == UNIT_KILOGRAM )
    {
        printf("Product: %s, Quatity: %.2f\n", product.name, product.quantity.kilograms);
    }
    else
    {
        printf("Product: %s, Quatity: %i\n", product.name, product.quantity.pieces);
    }

}
