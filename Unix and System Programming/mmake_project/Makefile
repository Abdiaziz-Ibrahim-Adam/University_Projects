CC = gcc
CFLAGS = -g -std=gnu11 -Werror -Wall -Wextra -Wpedantic \
         -Wmissing-declarations -Wmissing-prototypes -Wold-style-definition

OBJ = mmake.o parser.o

# Define the 'all' target
all: mmake

# Link the program
mmake: $(OBJ)
	$(CC) $(CFLAGS) -o mmake $(OBJ)

# Compile the object files
mmake.o: mmake.c parser.h
	$(CC) $(CFLAGS) -c mmake.c

parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c

# Run the program
run: mmake
	./mmake 

# Check for memory leaks
val: mmake
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./mmake

# Clean objects and executable
clean:
	rm -f mmake $(OBJ)
