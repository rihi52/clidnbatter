# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Wextra -g -I$(SRCDIR)
LDFLAGS = -lsqlite3

# Directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin
PROJECT_DIR = $(shell pwd)
INSTALL_DIR = /usr/local/bin

# Files
SOURCES = main.c $(SRCDIR)/combat.c $(SRCDIR)/lookup.c $(SRCDIR)/modify.c $(SRCDIR)/global.c 
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

# Install the binary
install: $(EXECUTABLE)
	install -m 0755 $(EXECUTABLE) $(INSTALL_DIR)

# Uninstall the binary
uninstall:
	rm -f $(INSTALL_DIR)/clidnbatter

# Clean up generated files
clean:
	rm -f $(OBJDIR)/*.o $(OBJDIR)/src/*.o $(EXECUTABLE)
	rm -rf $(OBJDIR) $(BINDIR)


# # Compiler and Flags
# CC = gcc
# CFLAGS = -Wall -Wextra -g -I$(SRCDIR)
# LDFLAGS = -lsqlite3

# # Directories
# SRCDIR = src
# OBJDIR = obj
# BINDIR = bin
# PROJECT_DIR = $(shell pwd)

# # Files
# SOURCES = main.c $(SRCDIR)/combat.c $(SRCDIR)/lookup.c $(SRCDIR)/modify.c $(SRCDIR)/global.c 
# OBJECTS = $(SOURCES:%.c=$(OBJDIR)/%.o)
# EXECUTABLE = $(BINDIR)/clidnbatter

# # Create the output directories
# $(shell mkdir -p $(OBJDIR)/src $(BINDIR))

# # Rules
# all: $(EXECUTABLE)

# # Link the executable
# $(EXECUTABLE): $(OBJECTS)
# 	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# # Compile source files into object files
# $(OBJDIR)/%.o: %.c
# 	$(CC) $(CFLAGS) -c $< -o $@

# $(OBJDIR)/src/%.o: $(SRCDIR)/%.c
# 	$(CC) $(CFLAGS) -c $< -o $@

# # Clean up generated files
# clean:
# 	rm -f $(OBJDIR)/*.o $(OBJDIR)/src/*.o $(EXECUTABLE)
# 	rm -rf $(OBJDIR) $(BINDIR)