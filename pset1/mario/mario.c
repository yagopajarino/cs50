#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int height;
    // Ask for user for integer, if non 0 < x < 9 repeats the question
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    // Loops that fill with " " or "#" depending the row number
    for (int i = 0; i < height; i++)
    {
        for (int n = height - i; n > 1; n--)
        {
            printf(" ");
        }
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }
        printf("  ");
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }
        printf("\n");
    }
}