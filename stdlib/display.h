/*
 * Robert Durst 2019
 * Sailfish Programming Language
 * C Standard Library including useful wrapper functions for print methods.
 */
#include <stdio.h>

void
print_flt(float f)
{
    printf("%f", f);
}

void
print_int(int i)
{
    printf("%d", i);
}

void
print_str(char* c)
{
    printf("%s", c);
}