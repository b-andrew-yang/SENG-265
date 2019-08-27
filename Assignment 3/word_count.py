# Andrew Yang
# SENG 265
# Daniela Damian
# V00878595
# 11/18/17

import sys
import operator
import string
from collections import Counter
from collections import OrderedDict


# flags to handle switches
infile_flag = 0
sort_flag = 0
printwords_flag = 0
input_file = ""
word_lengths = [0] * 26


def main():
    #sets our flags and initializes our word_list
    set_flags()
    word_list = {}

    # opens the file and counts the lengths
    infile_words = open(input_file).read()
    counts = Counter([len(word.strip('?!.,():; '))for word in infile_words.split()])

    # adds to our dictionary of lists which is {word_length, [words of that length]}
    exclude = ['.', ',', '!', '?', '(', ')', '\n', ' ', ';']
    for word in infile_words.split():
        tmp_word = word
        word = ''.join(ch for ch in word if ch not in exclude)
        if len(word) in word_list:
            if word.lower() not in word_list[len(word)]:
                word_list[len(word)].append(word.lower())
        else:
            word_list[len(word)] = [word.lower()]

    # sorts each list of words
    for tmp_list in word_list:
        word_list[tmp_list].sort()

    # checks our flag to see which method to go into
    if(sort_flag == 1):
        sort_mode(counts, word_list)
    elif(infile_flag == 1):
        infile_mode(counts, word_list)


# prints out the information for word length ascending order
def infile_mode(counts, word_list):
    if(printwords_flag == 0):
        for x in range(1, 27):
            if(counts[x] != 0):
                print("Count[" + '{0:02d}'.format(x) + "]=" + 
                     str('{0:02d}'.format((counts[x]))) + ";")
    elif(printwords_flag == 1):
        for x in range(1, 27):
            if(counts[x] != 0):
                if(len(word_list[x]) > 1):    
                    print("Count[" + '{0:02d}'.format(x) + "]=" + 
                        str('{0:02d}'.format((counts[x]))) + "; (words: \"" 
                            + '", "'.join(word_list[x][:-1]) + "\"" + " and " + 
                                "\"" + word_list[x][-1] + "\")")
                else:
                    print("Count[" + '{0:02d}'.format(x) + "]=" + 
                        str('{0:02d}'.format((counts[x]))) + "; (words: \"" 
                            + '", "'.join(word_list[x]) + "\")")


# prints out the information based on frequency descending order
def sort_mode(counts, word_list):
    if printwords_flag == 0:
        print_mode(sorted(counts.most_common(), key=lambda x: (-x[1], x[0])))
    else:
        print_words(sorted(counts.most_common(), key=lambda x: (-x[1], x[0])), word_list)

def print_mode(counts):
    for k,v in counts:
        if(v != 0):
            print("Count[" + str('{0:02d}'.format(k)) + "]=" + str('{0:02d}'.format(v)) + ";")


def print_words(counts, word_list):
    for x in range(0, len(counts)):
        length_list, freq_list = zip(*counts)
        if(len(word_list[length_list[x]]) > 1):
            print("Count[" + str('{0:02d}'.format(length_list[x])) + "]=" + 
                str('{0:02d}'.format(freq_list[x])) + "; (words: \"" 
                    + '", "'.join(word_list[length_list[x]][:-1]) + "\"" + " and " + 
                        "\"" + str(word_list[length_list[x]][-1]) + "\")")
        else:
            print("Count[" + str('{0:02d}'.format(length_list[x])) + "]=" + 
                str('{0:02d}'.format(freq_list[x])) + "; (words: \"" 
                    + '", "'.join(word_list[length_list[x]]) + "\")")

    # for k,v in counts:
    #     if(v != 0):
    #         x = int(k)
    #         if(len(word_list[k]) > 1):
    #             print("Count[" + str('{0:02d}'.format(x)) + "]=" + 
    #                 str('{0:02d}'.format((counts[x]))) + "; (words: \"" 
    #                     + '", "'.join(word_list[x][:-1]) + "\"" + " and " + 
    #                         "\"" + str(word_list[x][-1]) + "\")")
    #         else:
    #             print("Count[" + str('{0:02d}'.format(x)) + "]=" + 
    #                 str('{0:02d}'.format((counts[x]))) + "; (words: \"" 
    #                     + '", "'.join(word_list[x]) + "\")")


# sets our flags to see which commands were typed
def set_flags():
    for x in range(0, len(sys.argv)):
        if sys.argv[x] == "--infile":
            global infile_flag
            infile_flag = 1
            global input_file
            input_file = sys.argv[x + 1]
        if sys.argv[x] == "--sort":
            global sort_flag
            sort_flag = 1
        if sys.argv[x] == "--print-words":
            global printwords_flag
            printwords_flag = 1


if __name__ == '__main__':
    main()
