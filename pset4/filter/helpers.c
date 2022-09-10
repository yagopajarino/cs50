#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float value = (float)(image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3;
            int rounded = round(value);
            image[i][j].rgbtBlue = rounded;
            image[i][j].rgbtGreen = rounded;
            image[i][j].rgbtRed = rounded;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE arr[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = width - 1, s = 0; j >= 0; j--, s++)
        {
            arr[i][j].rgbtBlue = image[i][s].rgbtBlue;
            arr[i][j].rgbtGreen = image[i][s].rgbtGreen;
            arr[i][j].rgbtRed = image[i][s].rgbtRed;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = arr[i][j].rgbtBlue;
            image[i][j].rgbtGreen = arr[i][j].rgbtGreen;
            image[i][j].rgbtRed = arr[i][j].rgbtRed;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE arr[height][width];
    // For pixels not in edges
    for (int i = 1; i < height - 1; i++)
    {
        for (int j = 1; j < width - 1; j++)
        {
            float blue = 0, green = 0, red = 0;
            for (int n = -1; n < 2; n++)
            {
                for (int m = -1; m < 2; m++)
                {
                    blue += image[i + n][j + m].rgbtBlue;
                    green += image[i + n][j + m].rgbtGreen;
                    red += image[i + n][j + m].rgbtRed;
                }
            }
            blue = round(blue / 9);
            green = round(green / 9);
            red = round(red / 9);

            arr[i][j].rgbtBlue = blue;
            arr[i][j].rgbtGreen = green;
            arr[i][j].rgbtRed = red;
        }
    }
    // Pixels in first row, but not in apex
    for (int i = 1; i < width - 1; i++)
    {
        float blue = 0, green = 0, red = 0;
        for (int n = 0; n < 2; n++)
        {
            for (int m = -1; m < 2; m++)
            {
                blue += image[n][i + m].rgbtBlue;
                green += image[n][i + m].rgbtGreen;
                red += image[n][i + m].rgbtRed;
            }
        }
        blue = round(blue / 6);
        green = round(green / 6);
        red = round(red / 6);


        arr[0][i].rgbtBlue = blue;
        arr[0][i].rgbtGreen = green;
        arr[0][i].rgbtRed = red;
    }

    // Pixels in last row, but not in apex
    for (int i = 1; i < width - 1; i++)
    {
        float blue = 0, green = 0, red = 0;
        for (int n = height - 2; n < height; n++)
        {
            for (int m = -1; m < 2; m++)
            {
                blue += image[n][i + m].rgbtBlue;
                green += image[n][i + m].rgbtGreen;
                red += image[n][i + m].rgbtRed;
            }
        }
        blue = round(blue / 6);
        green = round(green / 6);
        red = round(red / 6);

        arr[height - 1][i].rgbtBlue = blue;
        arr[height - 1][i].rgbtGreen = green;
        arr[height - 1][i].rgbtRed = red;
    }

    // Pixels in first column, but not in apex
    for (int i = 1; i < height - 1; i++)
    {
        float blue = 0, green = 0, red = 0;
        for (int n = 0; n < 2; n++)
        {
            for (int m = -1; m < 2; m++)
            {
                blue += image[i + m][n].rgbtBlue;
                green += image[i + m][n].rgbtGreen;
                red += image[i + m][n].rgbtRed;
            }
        }
        blue = round(blue / 6);
        green = round(green / 6);
        red = round(red / 6);

        arr[i][0].rgbtBlue = blue;
        arr[i][0].rgbtGreen = green;
        arr[i][0].rgbtRed = red;
    }

    // Pixels in last columns, but not in corners
    for (int i = 1; i < height - 1; i++)
    {
        float blue = 0, green = 0, red = 0;
        for (int n = width - 1; n > width - 3; n--)
        {
            for (int m = -1; m < 2; m++)
            {
                blue += image[i + m][n].rgbtBlue;
                green += image[i + m][n].rgbtGreen;
                red += image[i + m][n].rgbtRed;
            }
        }
        blue = round(blue / 6);
        green = round(green / 6);
        red = round(red / 6);

        arr[i][width - 1].rgbtBlue = blue;
        arr[i][width - 1].rgbtGreen = green;
        arr[i][width - 1].rgbtRed = red;
    }

    // Corners
    arr[0][0].rgbtBlue = round((float)(image[0][0].rgbtBlue + image[0][1].rgbtBlue + image[1][0].rgbtBlue + image[1][1].rgbtBlue) / 4);
    arr[0][0].rgbtGreen = round((float)(image[0][0].rgbtGreen + image[0][1].rgbtGreen + image[1][0].rgbtGreen + image[1][1].rgbtGreen) /
                                4);
    arr[0][0].rgbtRed = round((float)(image[0][0].rgbtRed + image[0][1].rgbtRed + image[1][0].rgbtRed + image[1][1].rgbtRed) / 4);

    arr[0][width - 1].rgbtBlue = round((float)(image[0][width - 1].rgbtBlue + image[0][width - 2].rgbtBlue + image[1][width -
                                       1].rgbtBlue + image[1][width - 2].rgbtBlue) / 4);
    arr[0][width - 1].rgbtGreen = round((float)(image[0][width - 1].rgbtGreen + image[0][width - 2].rgbtGreen + image[1][width -
                                        1].rgbtGreen + image[1][width - 2].rgbtGreen) / 4);
    arr[0][width - 1].rgbtRed = round((float)(image[0][width - 1].rgbtRed + image[0][width - 2].rgbtRed + image[1][width -
                                      1].rgbtRed + image[1][width - 2].rgbtRed) / 4);

    arr[height - 1][0].rgbtBlue = round((float)(image[height - 1][0].rgbtBlue + image[height - 1][1].rgbtBlue + image[height -
                                        2][0].rgbtBlue + image[height - 2][1].rgbtBlue) / 4);
    arr[height - 1][0].rgbtGreen = round((float)(image[height - 1][0].rgbtGreen + image[height - 1][1].rgbtGreen + image[height -
                                         2][0].rgbtGreen + image[height - 2][1].rgbtGreen) / 4);
    arr[height - 1][0].rgbtRed = round((float)(image[height - 1][0].rgbtRed + image[height - 1][1].rgbtRed + image[height -
                                       2][0].rgbtRed + image[height - 2][1].rgbtRed) / 4);

    arr[height - 1][width - 1].rgbtBlue = round((float)(image[height - 1][width - 1].rgbtBlue + image[height - 1][width -
                                          2].rgbtBlue + image[height - 2][width - 1].rgbtBlue + image[height - 2][width - 2].rgbtBlue) / 4);
    arr[height - 1][width - 1].rgbtGreen = round((float)(image[height - 1][width - 1].rgbtGreen + image[height - 1][width -
                                           2].rgbtGreen + image[height - 2][width - 1].rgbtGreen + image[height - 2][width - 2].rgbtGreen) / 4);
    arr[height - 1][width - 1].rgbtRed = round((float)(image[height - 1][width - 1].rgbtRed + image[height - 1][width -
                                         2].rgbtRed + image[height - 2][width - 1].rgbtRed + image[height - 2][width - 2].rgbtRed) / 4);

    // Writting arr into image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = arr[i][j].rgbtBlue;
            image[i][j].rgbtGreen = arr[i][j].rgbtGreen;
            image[i][j].rgbtRed = arr[i][j].rgbtRed;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    RGBTRIPLE image_ext[height + 2][width + 2];
    RGBTRIPLE arr[height][width];

    for (int i = 0; i < height + 2; i++)
    {
        for (int j = 0; j < width + 2; j++)
        {
            if (i == 0 || i == height + 1 || j == 0 || j == width + 1)
            {
                image_ext[i][j].rgbtBlue = 0;
                image_ext[i][j].rgbtGreen = 0;
                image_ext[i][j].rgbtRed = 0;
            }
            else
            {
                image_ext[i][j].rgbtBlue = image[i - 1][j - 1].rgbtBlue;
                image_ext[i][j].rgbtGreen = image[i - 1][j - 1].rgbtGreen;
                image_ext[i][j].rgbtRed = image[i - 1][j - 1].rgbtRed;
            }
        }
    }

    for (int i = 1; i < height + 1; i++)
    {
        for (int j = 1; j < width + 1; j++)
        {
            int blue_x = 0, green_x = 0, red_x = 0, blue_y = 0, green_y = 0, red_y = 0;
            int blue = 0, green = 0, red = 0;
            for (int n = -1; n < 2; n++)
            {
                for (int m = -1; m < 2; m++)
                {
                    blue_x += Gx[1 + n][1 + m] * image_ext[i + n][j + m].rgbtBlue;
                    green_x += Gx[1 + n][1 + m] * image_ext[i + n][j + m].rgbtGreen;
                    red_x += Gx[1 + n][1 + m] * image_ext[i + n][j + m].rgbtRed;

                    blue_y += Gy[1 + n][1 + m] * image_ext[i + n][j + m].rgbtBlue;
                    green_y += Gy[1 + n][1 + m] * image_ext[i + n][j + m].rgbtGreen;
                    red_y += Gy[1 + n][1 + m] * image_ext[i + n][j + m].rgbtRed;
                }
            }
            blue = round(sqrt((float)pow(blue_x, 2) + (float) pow(blue_y, 2)));
            green = round(sqrt((float)pow(green_x, 2) + (float) pow(green_y, 2)));
            red = round(sqrt((float)pow(red_x, 2) + (float) pow(red_y, 2)));

            if (blue > 255)
            {
                blue = 255;
            }
            if (green > 255)
            {
                green = 255;
            }
            if (red > 255)
            {
                red = 255;
            }

            arr[i - 1][j - 1].rgbtBlue = blue;
            arr[i - 1][j - 1].rgbtGreen = green;
            arr[i - 1][j - 1].rgbtRed = red;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = arr[i][j].rgbtBlue;
            image[i][j].rgbtGreen = arr[i][j].rgbtGreen;
            image[i][j].rgbtRed = arr[i][j].rgbtRed;
        }
    }
    return;
}
