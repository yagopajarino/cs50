#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <math.h>

int main(void)
{
    string s = get_string("Text: ");
    int let = 0, words = 0, sent = 0;
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        char c = s[i];
        if (c == ' ')
        {
            words += 1;
        }
        else if (c == '.' || c == '!' || c == '?')
        {
            sent += 1;
        }
        else if ((int) c >= 'A' && (int) c <= 'z')
        {
            let += 1;
        }
    }
    words += 1;
    float L = (float) let / words * 100;
    float S = (float) sent / words * 100;
    double index = 0.0588 * L - 0.296 * S - 15.8;
    int grade = round(index);
    //printf("letters: %i\nwords: %i\nsentences: %i\nL: %f, S: %f\n", let, words, sent, L, S);
    if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}