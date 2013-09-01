# echoserver makefile
# ===================
# Copyright 2013 Paul Griffiths
# Email: mail@paulgriffiths.net
#
# Distributed under the terms of the GNU General Public License.
# http://www.gnu.org/licenses/


# Variables section
# =================

# Executable names
OUT=echoserver

# Compiler executable name
CC=gcc

# Compiler flags
CFLAGS=-std=c99 -pedantic -Wall -Wextra -D_POSIX_C_SOURCE=200112L
C_DEBUG_FLAGS=-ggdb -DDEBUG -DDEBUG_ALL
C_RELEASE_FLAGS=-O3 -DNDEBUG

# Linker flags
LDFLAGS=-lpthread -lchelpers -lsockethelpers

# Object code files
OBJS=main.o echo_server.o socket_helpers.o debug_thread_counter.o

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
debug: C_ISO_FLAGS+=$(C_DEBUG_FLAGS)
debug: C_UNIX_FLAGS+=$(C_DEBUG_FLAGS)
debug: main

# release - builds with optimizations and without debugging info
.PHONY: release
release: CFLAGS+=$(C_RELEASE_FLAGS)
release: C_ISO_FLAGS+=$(C_RELEASE_FLAGS)
release: C_UNIX_FLAGS+=$(C_RELEASE_FLAGS)
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


# Executable targets section
# ==========================

# Main executable
main: $(OBJS)
	@echo "Building echoserver..."
	@$(CC) -o $(OUT) $(OBJS) $(LDFLAGS)
	@echo "Done."


# Object files targets section
# ============================

# Object files for executable

main.o: main.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c -o $@ $<

debug_thread_counter.o: debug_thread_counter.c debug_thread_counter.h
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c -o $@ $<

echo_server.o: echo_server.c echo_server.h debug_thread_counter.h \
	socket_helpers.h
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c -o $@ $<

socket_helpers.o: socket_helpers.c socket_helpers.h
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c -o $@ $<

