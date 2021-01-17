
LINKFLAGS=-lfloatLib -L../../../mathcode/floatLib/trunk/
CPPFLAGS=-g -I../../../mathcode/floatLib/trunk/

# This is the primary program to generate the collatz families
filter:  ../../../mathcode/floatLib/trunk/libfloatLib.a filter.o CollatzLib.o
	libtool --mode=link g++ $(CPPFLAGS) $(LINKFLAGS) filter.o CollatzLib.o -I/usr/local/include/mysql -L/usr/local/lib/mysql -lmysqlclient -o filter

# A utility program
glide:  ../../../mathcode/floatLib/trunk/libfloatLib.a CollatzLib.o glide.cxx
	libtool --mode=link g++ $(CPPFLAGS) $(LINKFLAGS) CollatzLib.o glide.cxx -o glide

n:  floating.o buffer.o floatfunc.o n.cxx
	g++ floating.o floatfunc.o buffer.o  n.cxx -o n

CloseSets:  ../../../mathcode/floatLib/trunk/libfloatLib.a CollatzLib.o CloseSets.cxx
	libtool --mode=link g++ $(CPPFLAGS) $(LINKFLAGS) CollatzLib.o CloseSets.cxx -o CloseSets

group_by:  floating.o buffer.o floatfunc.o group_by.cxx
	g++ floating.o floatfunc.o buffer.o  group_by.cxx -o group_by

# A utility program
bin_glide:  ../../../mathcode/floatLib/trunk/libfloatLib.a CollatzLib.o bin_glide.cxx
	libtool --mode=link g++ $(CPPFLAGS) $(LINKFLAGS) CollatzLib.o bin_glide.cxx -o bin_glide

# A utility program
hex_glide:  ../../../mathcode/floatLib/trunk/libfloatLib.a hex_glide.cxx
	libtool --mode=link g++ $(CPPFLAGS) $(LINKFLAGS) hex_glide.cxx -o hex_glide

# a program generating many glides from a residue file
GlideResidues:  floating.o buffer.o floatfunc.o CollatzLib.o GlideResidues.cxx
	g++ floating.o floatfunc.o buffer.o CollatzLib.o GlideResidues.cxx -o GlideResidues

CollatzLib.o:  CollatzLib.cxx CollatzLib.h
	g++ $(CPPFLAGS) CollatzLib.cxx -c

filter.o:  filter.cxx
	g++ $(CPPFLAGS) filter.cxx -c

../../../mathcode/floatLib/trunk/libfloatLib.a:
	make -C ../../../mathcode/floatLib/trunk/ libfloatLib.a

clean:
	rm *.o; true
	rm glide; true
	rm filter; true
	rm CloseSets; true
	rm bin_glide; true
	rm -rf .libs; true

# Legacy stuff
Collatz:  Collatz.cxx
	g++ Collatz.cxx -o Collatz

sig.txt:  Collatz
	./Collatz > sig.txt

sig_sort.txt:  sig.txt
	awk '-F,' '{printf( "%s:%s,%s\n",$$2,$$1,$$3 )}' sig.txt > sig_temp.txt
	sort -k 1 -t , sig_temp.txt > sig_sort_temp.txt
	awk '-F:|,' '{printf( "%s,%s,%s\n", $$2,$$1,$$3 )}' sig_sort_temp.txt > sig_sort.txt
	rm sig_temp.txt
	rm sig_sort_temp.txt
