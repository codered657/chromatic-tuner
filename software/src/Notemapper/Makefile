
# The first build rule is used if no arguments are specfiied to make.
# Alternatively, one can type "make clean" to run the clean rule, or
# "make clean all" to invoke multiple build targets.

all: notemapper

# This rule specifies how to generate the notemapper program, if we also have
# notemapper.o.  If onebits.o doesn't exist, make will use the rule for notemapper.o
# to generate it from notemapper.c.

notemapper: notemapper.o
	gcc -lm -o notemapper notemapper.o

notemapper.o: notemapper.c
	gcc -lm -c notemapper.c
	
clean:
	rm -f notemapper *.o *~
