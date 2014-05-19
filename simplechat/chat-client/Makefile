# chat-client makefile
# ====================
# Copyright 2014 Paul Griffiths
# Email: mail@paulgriffiths.net
#
# Distributed under the terms of the GNU General Public License.
# http://www.gnu.org/licenses/


# Variables section
# =================

# Executable names
OUT=chat-client
TEST_OUT=tests

# Compiler executable name
CC=gcc

# Compiler flags
CFLAGS=-std=c99 -pedantic -Wall -Wextra
C_DEBUG_FLAGS=-ggdb -DDEBUG -DDEBUG_ALL
C_RELEASE_FLAGS=-O3 -DNDEBUG

# Linker flags
LDFLAGS=-lcurses

# Object code files
OBJS=main.o mainwin.o signals.o input_buffer.o
OBJS+=sliwin.o msgwin.o string_functions.o messages.o logging.o mainloop.o

# Test object code files
TEST_OBJS=test_main.o test_logging.o test_string_functions.o
TEST_OBJS+=test_messages.o
TEST_OBJS+=string_functions.o messages.o logging.o

# Source and clean files and globs
SRCS=$(wildcard *.c *.h)

SRCGLOB=*.c

CLNGLOB=$(OUT) $(SAMPLEOUT)
CLNGLOB+=*~ *.o *.gcov *.out *.gcda *.gcno


# Build targets section
# =====================

default: debug

# debug - builds objects with debugging info
.PHONY: debug
debug: CFLAGS+=$(C_DEBUG_FLAGS)
debug: main

# release - builds with optimizations and without debugging info
.PHONY: release
release: CFLAGS+=$(C_RELEASE_FLAGS)
release: main

# clean - removes ancilliary files from working directory
.PHONY: clean
clean:
	-@rm $(CLNGLOB) 2>/dev/null

# docs - makes Doxygen documentation:
.PHONY: docs
docs:
	@doxygen
	-@cd latex; make; cd ..
	@if [ ! -d docs ]; then \
		mkdir docs; fi
	@echo "Copying reference manual to docs..."
	@cp latex/refman.pdf docs 
	@echo "Done."

# lint - runs splint with normal checking
.PHONY: lint
lint:
	@splint $(SRCGLOB)

# linteasy - runs splint with weak checking
.PHONY: linteasy
linteasy:
	@splint -weak $(SRCGLOB)

# tags - makes tags file
.PHONY: tags
tags:
	@ctags *.c *.g


# Executable targets section
# ==========================

# Main executable
main: $(OBJS)
	@echo "Building chat-client..."
	@$(CC) -o $(OUT) $(OBJS) $(LDFLAGS)
	@echo "Done."

# Unit tests
tests: $(TEST_OBJS)
	@echo "Building chat-client tests..."
	@$(CC) -o $(TEST_OUT) $(TEST_OBJS) $(LDFLAGS)
	@echo "Done."

# Object files targets section
# ============================

# Object files for executable

main.o: main.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c -o $@ $<

mainwin.o: mainwin.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c -o $@ $<

signals.o: signals.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c -o $@ $<

input_buffer.o: input_buffer.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c -o $@ $<

sliwin.o: sliwin.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c -o $@ $<

msgwin.o: msgwin.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c -o $@ $<

string_functions.o: string_functions.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c -o $@ $<

messages.o: messages.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c -o $@ $<

logging.o: logging.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c -o $@ $<

mainloop.o: mainloop.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c -o $@ $<

# Object files for tests

test_main.o: test_main.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c -o $@ $<

test_logging.o: test_logging.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c -o $@ $<

test_string_functions.o: test_string_functions.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c -o $@ $<

test_messages.o: test_messages.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c -o $@ $<

