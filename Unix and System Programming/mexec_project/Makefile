CC = gcc
CFLAGS = -g -std=gnu11 -Werror -Wall -Wextra -Wpedantic \
         -Wmissing-declarations -Wmissing-prototypes -Wold-style-definition

OBJ = mexec.o

# Link the program
mexec: $(OBJ)
	$(CC) $(CFLAGS) -o mexec $(OBJ)

# Compile the object files
mexec.o: mexec.c mexec.h
	$(CC) $(CFLAGS) -c mexec.c

# Run the program
run: mexec
	./mexec

# Check for memory leaks
val: mexec
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./mexec

# Clean objects and executable
clean:
	rm -f mexec $(OBJ)
