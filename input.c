#include <stdio.h>
#define MAX 100

int main(void)
{
    // Data types and identifiers
    int a = 10;
    float b = 5.5;
    char ch = 'A';
    double value = 20.5;

    // If-else condition
    if (a > 5)
    {
        printf("Greater\n");
    }
    else
    {
        printf("Smaller\n");
    }

    // For loop
    for (int i = 0; i < 5; i++)
    {
        printf("%d\n", i);
    }

    // While loop
    int j = 0;
    while (j < 3)
    {
        j++;
    }

    // Switch-case
    switch (a)
    {
        case 10:
            break;
        default:
            return 0;
    }

    /* Multi-line comment */
    return 0;
}