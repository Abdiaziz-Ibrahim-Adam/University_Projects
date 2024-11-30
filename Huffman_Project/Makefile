CC = gcc
ACTION1 = -encode
ACTION2 = -decode
FILE1 = löre.txt balans.txt out_fil.txt
FILE2 = löre.txt out_fil.txt rest.txt
FILE3 = abracadabra.txt abba.txt out_fil.txt
FILE4 = abracadabra.txt out_fil.txt rest.txt
#compiler flags
FLAGS = -g -std=c99 -Wall -o

main: huffman.c
	$(CC) $(FLAGS) huffman huffman.c frequency_table.c pqueue.c list.c Huff_Trie.c huff_table.c encode_decode.c bit_buffer.c 

run1: main
	./huffman $(ACTION1) $(FILE1)

run2: main
	./huffman $(ACTION2) $(FILE2)

run3: main
	./huffman $(ACTION1) $(FILE3)

run4: main
	./huffman $(ACTION2) $(FILE4)

val1: main
	valgrind --leak-check=full ./huffman $(ACTION1) $(FILE1)

val2: main
	valgrind --leak-check=full ./huffman $(ACTION2) $(FILE2)
