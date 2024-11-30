# Compiler and Flags
CC = gcc
CFLAGS = -g -std=gnu11 -Werror -Wall -Wextra -Wpedantic \
         -Wmissing-declarations -Wmissing-prototypes -Wold-style-definition
LDFLAGS = -lpthread

# Object Files
OBJ = mdu.o paths.o threads.o directory_usage.o stack.o

# Define the 'all' target
all: mdu

# Link the program
mdu: $(OBJ)
	$(CC) $(OBJ) -o mdu $(LDFLAGS)

# Compile the object files
mdu.o: mdu.c mdu.h paths.h threads.h directory_usage.h stack.h
	$(CC) $(CFLAGS) -c mdu.c -o mdu.o

paths.o: paths.c paths.h mdu.h
	$(CC) $(CFLAGS) -c paths.c -o paths.o

threads.o: threads.c threads.h mdu.h
	$(CC) $(CFLAGS) -c threads.c -o threads.o

directory_usage.o: directory_usage.c directory_usage.h mdu.h
	$(CC) $(CFLAGS) -c directory_usage.c -o directory_usage.o

stack.o: stack.c stack.h
	$(CC) $(CFLAGS) -c stack.c -o stack.o

# Run the program
run: mdu
	./mdu

# Check for memory leaks
val: mdu
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./mdu 

# Check for race conditions
val_t: mdu
	valgrind --tool=helgrind ./mdu  

# Clean objects and executable
clean:
	rm -f mdu $(OBJ)
