// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 1;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    long n = strlen(word);
    char *str = malloc(sizeof(char) * n);

    for (int i = 0; i < n; i++)
    {
        str[i] = tolower(word[i]);
    }

    int index = hash(str);
    node *temp = table[index];

    while (temp -> next != NULL)
    {
        if (strcasecmp(word, temp -> word) == 0)
        {
            free(str);
            return true;
        }
        temp = temp -> next;

    }
    if (strcasecmp(word, temp -> word) == 0)
    {
        free(str);
        return true;
    }
    free(str);
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned long hash = 5381;
    long c = *word++;

    hash = ((hash << 5) + hash); /* hash * 33 + c */
    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    char word[LENGTH + 1];

    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        printf("Cant open dict");
        return false;
    }

    while (fscanf(dict, "%s", word) != EOF)
    {
        for (int w = 0, n = strlen(word); w < n; w++)
        {
            word[w] = tolower(word[w]);
        }

        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            printf("Memory error at malloc dict");
            return false;
        }

        strcpy(n -> word, word);
        n -> next = NULL;

        int index = hash(word);

        if (table[index] == 0)
        {
            table[index] = n;
        }
        else
        {
            n -> next = table[index];
            table[index] = n;
        }
    }
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    node *n;
    int count = 0;
    for (int i = 0; i < N; i++)
    {
        n = table[i];
        while (n -> next != NULL)
        {
            n = n -> next;
            count++;
        }
        count++;
    }
    return count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    int count = 0;
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        node *tmp = cursor;

        while (cursor -> next != NULL)
        {
            cursor = cursor -> next;
            free(tmp);
            tmp = cursor;
        }
        free(cursor);
    }
    return true;
}