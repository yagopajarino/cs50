#include <stdio.h>
#include <cs50.h>

int main(void)
{
    string name = get_string("What's your name? "); // Prompt asking for user's name
    printf("hello, %s\n", name); // Printing hello followed by user's name
}