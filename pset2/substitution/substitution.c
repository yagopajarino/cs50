#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

bool double_char(string key);
string cipher(string key, string plain);

int main(int argc, string argv[])
{
    string key;
    if (argc == 2)
    {
        key = argv[1];
    }
    else
    {
        printf("Usage: ./substitution KEY\n");
        return 1;
    }
    int n = strlen(key);
    for (int i = 0; i < n; i++) // Look for non alphabetic characters in the string
    {
        if (key[i] < 'A' || key[i] > 'z')
        {
            printf("Error: KEY must contain only alphabetic characters\n");
            return 1;
        }
    }
    if (n != 26) // Cheking KEY lenght
    {
        printf("Error: KEY must contain 26 characters\n");
        return 1;
    }
    if (double_char(key))
    {
        printf("Error: KEY must not repite letters\n");
        return 1;
    }
    string plain = get_string("plaintext: \n");
    string ct = cipher(key, plain);
    printf("ciphertext: %s\n", ct);
}

bool double_char(string key)
{
    string abc = "abcdefghijklmnopqrstuvwxyz";
    for (int i = 0, n = 26; i < n; i++)
    {
        char c = abc[i];
        int reps = 0;
        for (int j = 0, m = 26; j < m; j++)
        {
            char d = tolower(key[j]);
            if (c == d)
            {
                reps += 1;
            }
        }
        if (reps == 0)
        {
            return true;
        }
    }
    return false;
}

string cipher(string key, string plain)
{
    char in, out;
    for (int i = 0, n = strlen(key); i < n; i++)
    {
        key[i] = tolower(key[i]);
    }
    for (int i = 0, n = strlen(plain); i < n; i++)
    {
        in = plain[i];
        if (in < 'A' || in > 'z')
        {
            out = in;
        }
        else if (islower(in))
        {
            out = key[(int) in - 97];
        }
        else
        {
            in = tolower(in);
            out = toupper(key[(int) in - 97]);
        }
        plain[i] = out;
    }
    return plain;
}