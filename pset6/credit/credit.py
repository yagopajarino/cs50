from cs50 import get_string

number = get_string("Number: ")

if len(number) < 13 or len(number) > 16:
    print("INVALID")
else:
    l = len(number)
    suma_d = 0
    suma_i = 0
    for x in range(0, l, 2):
        if l % 2 == 0:
            num = int(number[x]) * 2
            if num > 9:
                num = 1 + num % 10
            suma_d += num
            suma_i += int(number[x + 1])
        else:
            if x == 0:
                suma_i += int(number[x])
            else:
                num = int(number[x - 1]) * 2
                if num > 9:
                    num = 1 + num % 10
                suma_d += num
                suma_i += int(number[x])

    suma = suma_d + suma_i
    d_two = int(number[1])
    d = int(number[0])

    if (suma % 10 == 0):
        if (d == 3 and l == 15 and (d_two == 3 or d_two == 7)):
            print("AMEX")
        elif (d == 5 and l == 16 and (d_two >= 1 or d_two <= 5)):
            print("MASTERCARD")
        elif (d == 4 and (l == 13 or l == 16)):
            print("VISA")
        else:
            print("INVALID")
    else:
        print("INVALID")

    # print("len: {}, suma_d: {}, suma_i: {}, suma: {}, d: {}, d_two: {}".format(l, suma_d, suma_i, suma, d, d_two))