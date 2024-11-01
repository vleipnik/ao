CC = gcc
CFLAGS = -Wall -O2 -fPIC
LDFLAGS = -shared

# Define the output library name
LIBRARY = didkit.so

# Define the source files
SRC = didkit.c

# Target to build the shared library
$(LIBRARY): $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(LIBRARY) $(SRC)

# Clean up the build files
clean:
	rm -f $(LIBRARY)