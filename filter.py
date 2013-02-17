#!/usr/bin/python
import Collatz
import argparse

parser = argparse.ArgumentParser(description='collatz sequence generator')
parser.add_argument('exponent', nargs=1, type=int, help='initial exponent (likely 0)')
parser.add_argument('datafile', nargs=1, help='text file to store glide results (likely new empty file)')
parser.add_argument('-m', '--max', default=None, type=int, help='max exponent')
args = parser.parse_args()

args.datafile = args.datafile[0]
args.exponent = args.exponent[0]

outFile = open( args.datafile, "a" )
inputFile = open( args.datafile, "r" )

base = 2**args.exponent
exponent = args.exponent

def CollatzAndLog( oFile, n ):
    t,d,m = Collatz.CollatzGlideCounts( n )

    if n == 0:
        outFile.write( "%i,%i,%i,%i\n" % (d, n, 0, t ) )
    else:
        outFile.write( "%i,%i,%i,%i\n" % (d, n, len(str(n)), t) )
    outFile.flush()

if base == 1:
    CollatzAndLog( outFile, 0 )

while args.max is None or exponent < args.max:
    inputFile.seek( 0 )

    for line in inputFile:
        fields = line.split( ',' )
        fields[0] = int( fields[0] )
        fields[1] = int( fields[1] )
        if fields[0] <= exponent:
            continue
        if fields[1] >= base:
            break
        CollatzAndLog( outFile, base + fields[1] )

    # Increment for the next loop
    exponent += 1
    base *= 2
