from cs50 import get_string

text = get_string("Text: ")
text = text.lower()

words = 1
letters = 0
sentences = 0
for x in text:
    if x in [".", "!", "?"]:
        sentences += 1
    elif x == " ":
        words += 1
    elif ord(x) > 96 and ord(x) < 123:
        letters += 1


L = letters / words * 100
S = sentences / words * 100
index = round(0.0588 * L - 0.296 * S - 15.8)

#print("w: {}, l: {}, s: {}, S: {}, L: {}, index: {}".format(words, letters, sentences, L, S, index))

if index > 16:
    print("Grade 16+")
elif index < 1:
    print("Before Grade 1")
else:
    print("Grade: {}".format(int(index)))
