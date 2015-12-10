# Compiler is homebrew's gcc-5 (GCC 5.2.0)
CC	= clang
# Compilation flags
#  -g		debugging metadata added to executable
#  -Wall	most warnings are displayed
#  -Wpedantic	warn if style is bad
#  -std=c11	use the modern C11 standard
CFLAGS 	= -g -Wall -Wpedantic -Wextra -std=c11 -O2
# Directories containing header files other than /usr/include
INCLUDES=
# Library paths in addition to /usr/lib
LFLAGS	=
# Any libraries to include
LIBS 	=
# Source .c files
SRCS 	= mips_disasm.c mips_read.c
# Object files to compile
OBJS 	= $(SRCS:.c=.o)
# Main executable file
MAIN 	= mips_disasm

.PHONY: depend clean

all:	$(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) *.o *~ $(MAIN)

depend: $(SRCS)
	makedepend $^

#
# DO NOT DELETE
