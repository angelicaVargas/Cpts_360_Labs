PROGRAM = raw_copy
# The "-W" warnings are the bare minimum I recommend. There are lots
# of others: see "info gcc".
CFLAGS = -g -Wall -Wstrict-prototypes

# "-g" says to link in support for the debugger (gdb).
LDFLAGS = -g

# Having defined macros, we start on the actual targets.

# ".PHONY" declares a target that is not a real file. The first target
# is the default target produced by entering "make" alone on the
# command line, hence the name "default". Apart from this, I generally
# put targets in alphabetical order for readability.
.PHONY: default
default: $(PROGRAM)

# This says how to compile any ".c" into a "*.o"
# "$<" means "the first file (or, here, pattern) to the right of the
# ':'" above.
raw_copy.o: raw_copy.c
	$(CC) $(CFLAGS) -c $<

# This target gets rid of all the "*.o" files that otherwise clutter
# the directory, but leaves the executable intact.
.PHONY: clean
clean:
	rm -f core* *.o *~

# This says how to link the "*.o" files into the executable.
# "$^" means "all of the files to the right of the ':'".
# "$@" means the "target" (what's on the left of the ':'").
# Note that $(CC) is used both for compiling and loading.
raw_copy: raw_copy.o
	$(CC) $(LDFLAGS) $^ -o $@

# This will run various tests of your code so that you can easily check
# the compile and test of the program in various ways.
# add new tests to evaluate the program

test: raw_copy
	./$^ 4 example.txt testfile.txt	
	        
# The "immaculate" target is a more extreme version of "clean",
# deleting all of the files that can be reconstructed automatically
# from source.
.PHONY: immaculate
immaculate: clean
	rm -f $(PROGRAM)
