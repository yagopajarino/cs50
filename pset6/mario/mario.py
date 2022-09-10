from cs50 import get_int

n = get_int("Height: ")
while (n < 1 or n > 8):
    n = get_int("Height: ")


for x in range(n):
    print(" " * (n - 1 - x) + "#" * (1 + x) + "  " + "#" * (1 + x))