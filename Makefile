# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Wextra -g -I$(SRCDIR)
LDFLAGS = -lsqlite3

# Directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Files
SOURCES = main.c $(SRCDIR)/combat.c $(SRCDIR)/lookup.c $(SRCDIR)/add.c 
OBJECTS = $(SOURCES:%.c=$(OBJDIR)/%.o)
EXECUTABLE = $(BINDIR)/clidnbatter

# Create the output directories
$(shell mkdir -p $(OBJDIR)/src $(BINDIR))

# Rules
all: $(EXECUTABLE)

# Link the executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Compile source files into object files
$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/src/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJDIR)/*.o $(OBJDIR)/src/*.o $(EXECUTABLE)
	rm -rf $(OBJDIR) $(BINDIR)

# old organization
# CC = gcc
# CFLAGS = -lpthread -ldl

# combatV2: main.c combat.c sqlite3.c lookup.c
# 	+$(MAKE) -C src

# clean:
# 	rm -f *.o combatV2