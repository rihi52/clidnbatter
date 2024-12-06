# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lsqlite3  # Link with sqlite3 library

# Directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Files
SOURCES = main.c $(SRCDIR)/combat.c $(SRCDIR)/lookup.c
OBJECTS = $(patsubst %.c, $(OBJDIR)/%.o, $(notdir $(SOURCES)))
EXECUTABLE = $(BINDIR)/clidnbatter

# Create the output directories
$(shell mkdir -p $(OBJDIR) $(BINDIR))

# Rules
all: $(EXECUTABLE)

# Link the executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Compile source files into object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/main.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJDIR)/*.o $(EXECUTABLE)
	rm -rf $(OBJDIR) $(BINDIR)

# Phony targets
.PHONY: all clean

# old organization
# CC = gcc
# CFLAGS = -lpthread -ldl

# combatV2: main.c combat.c sqlite3.c lookup.c
# 	+$(MAKE) -C src

# clean:
# 	rm -f *.o combatV2