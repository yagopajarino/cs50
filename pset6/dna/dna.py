from sys import argv
import csv


def main():
    if len(argv) != 3:
        print("Incorrect use of dna.py")
    else:
        with open(argv[1], "r") as source:
            reader = csv.DictReader(source)
            data = []
            for x in reader:
                data.append(x)

        with open(argv[2], "r") as source:
            txt = source.read()
            # print(txt)

        # print(data)

        seq = {}
        for dicts in data:
            for keys in dicts:
                try:
                    dicts[keys] = int(dicts[keys])
                    seq[keys] = 0
                except:
                    continue

        keys = list(seq.keys())
        count = 0
        for x in keys:
            rep = []
            l = len(x)
            for n in range(l):
                for y in range(0, len(txt), l):
                    if txt[y + n:(y + n + l)] == x:
                        count += 1
                    else:
                        rep.append(count)
                        count = 0
            rep = sorted(rep)
            seq[x] = rep[-1]
            count = 0
        # print (seq)

        check = 0
        for x in data:
            for y in keys:
                if x[y] == seq[y]:
                    check += 1
            if check == len(keys):
                print(x["name"])
                return
            else:
                check = 0
        print("No match")


main()

