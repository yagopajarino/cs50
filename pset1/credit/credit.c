#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Ask user for credit card number
    long number;
    int r = 0;
    number = get_long("Number: ");
    long nro = number;
    for (; nro > 0; nro /= 10)
    {
        r++;
    }


    if (r > 16 || r < 13)
    {
        printf("INVALID\n");
    }
    else
    {
        int d;
        int n = 0;
        int suma_p = 0;
        int suma_i = 0;
        int d_two = 0;
        for (nro = number; nro > 10; nro /= 10)
        {
            d_two = nro % 10;
        }
        for (; number > 0; number /= 10)
        {
            d = number % 10;
            if (n % 2 != 0)
            {
                int num = d * 2;
                if (num >= 10)
                {
                    num = 1 + num % 10;
                }
                suma_p += num;
            }
            else
            {
                suma_i += d;
            }
            n++;
        }
        int sum = suma_p + suma_i;
        if (sum % 10 == 0)
        {
            if (d == 3 && r == 15 && (d_two == 3 || d_two == 7))
            {
                printf("AMEX\n");
            }
            else if (d == 5 && r == 16 && d_two >= 1 && d_two <= 5)
            {
                printf("MASTERCARD\n");
            }
            else if (d == 4 && (r == 13 || r == 16))
            {
                printf("VISA\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else
        {
            printf("INVALID\n");
        }
        //printf("r = %d d = %d, sum = %d sum_p = %d sum_i = %d\n",r,d,sum,suma_p,suma_i);
    }

}