# Makefile for ARP Request Tool - Norma v4 Compliant
# Author: rdelicado
# Platform: Linux

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -Iinclude
DEBUG_FLAGS = -g -DDEBUG -O0
LDFLAGS =

# Project settings
NAME = arp_request
SRCDIR = src
INCDIR = include
OBJDIR = obj

# Source files
SOURCES = $(SRCDIR)/arp_request.c \
          $(SRCDIR)/network.c \
          $(SRCDIR)/packet.c \
          $(SRCDIR)/utils.c

OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Installation paths
PREFIX = /usr/local
BINDIR = $(PREFIX)/bin

# Default target
all: $(NAME)

# Create object directory
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Build the main target
$(NAME): $(OBJDIR) $(OBJECTS)
	$(CC) $(OBJECTS) -o $(NAME) $(LDFLAGS)

# Compile source files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Debug build
debug: CFLAGS += $(DEBUG_FLAGS)
debug: fclean $(NAME)

# Clean build artifacts
clean:
	rm -f $(OBJECTS)
	rm -rf $(OBJDIR)

# Full clean (remove all generated files including backups)
fclean: clean
	rm -f $(NAME)
	rm -f *~ *.bak *.tmp core vgcore.*
	rm -f $(NAME).debug
	@echo "Full clean completed"

# Rebuild everything
re: fclean all

# Install the binary (requires root privileges)
install: $(NAME)
	install -d $(BINDIR)
	install -m 755 $(NAME) $(BINDIR)
	@echo "$(NAME) installed to $(BINDIR)"
	@echo "Note: Run with sudo privileges for raw socket access"

# Uninstall the binary
uninstall:
	rm -f $(BINDIR)/$(NAME)
	@echo "$(NAME) removed from $(BINDIR)"

# Run the program (example usage)
run: $(NAME)
	@echo "Usage: sudo ./$(NAME) <target_ip>"
	@echo "Example: sudo ./$(NAME) 192.168.1.1"

# Check code style and potential issues
check: $(SOURCES)
	cppcheck --enable=all --suppress=missingIncludeSystem $(SOURCES)

# Show help
help:
	@echo "Available targets:"
	@echo "  all      - Build the program (default)"
	@echo "  debug    - Build with debug symbols"
	@echo "  clean    - Remove build artifacts"
	@echo "  fclean   - Full clean (remove all generated files)"
	@echo "  re       - Rebuild everything (fclean + all)"
	@echo "  install  - Install to $(BINDIR) (requires sudo)"
	@echo "  uninstall- Remove from $(BINDIR) (requires sudo)"
	@echo "  run      - Show usage example"
	@echo "  check    - Run static code analysis (requires cppcheck)"
	@echo "  help     - Show this help message"
	@echo ""
	@echo "Note: This program requires root privileges to create raw sockets"

# Declare phony targets
.PHONY: all debug clean fclean re install uninstall run check help

# Default target when no arguments are given
.DEFAULT_GOAL := all
