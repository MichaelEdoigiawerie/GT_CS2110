# TL04 - CS2110: Spring 2023
# GCC flags from the syllabus (each flag described for the curious minds!)
# Flag info credit: https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html

CFLAGS = -std=c99					# Using the C99 standard
CFLAGS += -Wall						# This enables all the warnings about constructions that some users consider questionable, and that are easy to avoid (or modify to prevent the warning), even in conjunction with macros
CFLAGS += -pedantic					# Issue all the warnings demanded by strict ISO C and ISO C++; reject all programs that use forbidden extensions, and some other programs that do not follow ISO C and ISO C++
CFLAGS += -Wextra					# This enables some extra warning flags that are not enabled by -Wall
CFLAGS += -Werror					# Make all warnings into errors
CFLAGS += -O0						# Optimize even more. GCC performs nearly all supported optimizations that do not involve a space-speed tradeoff.
CFLAGS += -Wstrict-prototypes				# Warn if a function is declared or defined without specifying the argument types
CFLAGS += -Wold-style-definition			# Warn if an old-style function definition is used. A warning is given even if there is a previous prototype
CFLAGS += -g						# Generate debugging information
CFLAGS += -fcf-protection=none				# Work around a bug in current GDB version 
CFLAGS += -Werror=vla					# Disable variable-length arrays (considered bad practice in C)

ARCH = $(shell uname -m)

# Source files to be compiled together
CFILES = tl04.c main.c
HFILES = tl04.h suites/test_utils.h

# Executable name
OBJNAME = tl04

.PHONY: all
all: run-tests

tl04: $(CFILES)
	@ # Compile all source files with the given flags into the specified executable object!
	@ gcc -fno-asm $(CFLAGS) $(CFILES) -o $(OBJNAME)

.PHONY: clean
clean:
	@ rm -f $(OBJNAME) tests *.o *.out

##########################################################################################
# NOTE: AUTOGRADING BELOW

# Uses pkg-config to retrieve package information about check -- Unit Testing Framework
CHECK_LIBS = $(shell pkg-config --cflags --libs check)
TEST_FILES_VISIBLE = suites/test_entry.c suites/tl4_suite.c
TEST_FILES_HIDDEN = suites/$(ARCH)/test_utils.o
TEST_FILES = $(TEST_FILES_VISIBLE) $(TEST_FILES_HIDDEN)

AUTOGRADER_CFILES = tl04.c

# Testing with check library
.PHONY: tests
tests: $(AUTOGRADER_CFILES)
	@ gcc -fno-asm -include suites/fakemalloc.h $(CFLAGS) $(AUTOGRADER_CFILES) $(TEST_FILES) $(CHECK_LIBS) -o tests

# To run a specific test case (or all similar to tests)
.PHONY: run-tests
run-tests: tests
	@ ./tests $(TEST)

# To run gdb on testcase(s)
.PHONY: run-gdb
run-gdb: tests
	@ CK_FORK=no gdb --args ./tests $(TEST)

.PHONY: run-valgrind
run-valgrind: tests
	CK_FORK=no valgrind --quiet --leak-check=full --error-exitcode=1 --show-leak-kinds=all --errors-for-leak-kinds=all ./tests $(TEST)
