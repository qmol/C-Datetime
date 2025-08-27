# Makefile for C-DaTime project
# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS = 

# Project name and executable
TARGET = datetime_unique
SRCDIR = .
OBJDIR = obj

# Source files
SOURCES = main.c datetime_util.c hash_set.c
OBJECTS = $(SOURCES:%.c=$(OBJDIR)/%.o)
HEADERS = datetime_util.h hash_set.h func_status.h

# Default target
all: $(TARGET)

# Create object directory if it doesn't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Build the main executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compile source files to object files
$(OBJDIR)/%.o: %.c $(HEADERS) | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(OBJDIR) $(TARGET)

# Clean and rebuild everything
rebuild: clean all

# Run the program with test input
test: $(TARGET)
	./$(TARGET) test_input.txt output.txt

# Run with custom input file
run: $(TARGET)
	@echo "Usage: make run INPUT=<input_file> OUTPUT=<output_file>"
	@echo "Example: make run INPUT=test_input.txt OUTPUT=results.txt"
ifdef INPUT
ifdef OUTPUT
	./$(TARGET) $(INPUT) $(OUTPUT)
else
	@echo "Error: Both INPUT and OUTPUT must be specified"
endif
else
	@echo "Error: Both INPUT and OUTPUT must be specified"
endif

# Debug build with additional flags
debug: CFLAGS += -DDEBUG -O0
debug: $(TARGET)

# Release build with optimization
release: CFLAGS += -O2 -DNDEBUG
release: clean $(TARGET)

# Install the executable (optional)
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

# Uninstall the executable
uninstall:
	rm -f /usr/local/bin/$(TARGET)

# Show help
help:
	@echo "Available targets:"
	@echo "  all      - Build the project (default)"
	@echo "  clean    - Remove build artifacts"
	@echo "  rebuild  - Clean and build"
	@echo "  test     - Run with test_input.txt"
	@echo "  run      - Run with custom input (requires INPUT and OUTPUT variables)"
	@echo "  debug    - Build with debug flags"
	@echo "  release  - Build optimized release version"
	@echo "  install  - Install executable to /usr/local/bin"
	@echo "  uninstall- Remove executable from /usr/local/bin"
	@echo "  help     - Show this help message"
	@echo ""
	@echo "Examples:"
	@echo "  make"
	@echo "  make test"
	@echo "  make run INPUT=data.txt OUTPUT=results.txt"
	@echo "  make debug"
	@echo "  make release"

# Declare phony targets
.PHONY: all clean rebuild test run debug release install uninstall help

# Dependencies (automatically generated)
$(OBJDIR)/main.o: main.c hash_set.h datetime_util.h func_status.h
$(OBJDIR)/datetime_util.o: datetime_util.c datetime_util.h func_status.h
$(OBJDIR)/hash_set.o: hash_set.c hash_set.h func_status.h
