#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    char filename[100];                         //String for filename
    int count = 0;                              //Count for sprintf
    int loop = 0;

    FILE *image = malloc(sizeof(int) * 512);    //FILE pointer to write
    unsigned char img[512];                     //Buffer for reading card

    FILE *card = fopen(argv[1], "r");
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    float m = 1;
    while (m == 1)
    {
        m = fread(img, 512, 1, card);
        if (m == 0)
        {
            break;
        }
        if (img[0] == 0xff && img[1] == 0xd8 && img[2] == 0xff && (img[3] & 0xf0) == 0xe0)
        {
            if (count == 0)
            {
                sprintf(filename, "%03i.jpg", count);
                image = fopen(filename, "w");
                fwrite(img, 512, 1, image);
                count ++;
            }
            else
            {
                fclose(image);
                sprintf(filename, "%03i.jpg", count);
                image = fopen(filename, "w");
                fwrite(img, 512, 1, image);
                count++;

            }
        }
        else
        {
            if (count != 0)
            {
                fwrite(img, 512, 1, image);
            }
        }
        //loop++;
        //int last_byte = ftell(card);
        //printf("m: %f, loop: %i, lastbyte: %i\n", m, loop, last_byte);
    }
    fclose(image);
    fclose(card);
}