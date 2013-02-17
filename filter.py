#!/usr/bin/env python
import Collatz
import argparse
import threading

parser = argparse.ArgumentParser(description='collatz sequence generator')
parser.add_argument('exponent', nargs=1, type=int, help='initial exponent (likely 0)')
parser.add_argument('datafile', nargs=1, help='text file to store glide results (likely new empty file)')
parser.add_argument('-m', '--max', default=None, type=int, help='max exponent')
parser.add_argument('-t', '--threads', default=None, type=int, help='maximum threads')
args = parser.parse_args()

args.datafile = args.datafile[0]
args.exponent = args.exponent[0]

outFile = open(args.datafile, "a")
inputFile = open(args.datafile, "r")

base = 2**args.exponent
exponent = args.exponent

def GlideLogLine(n):
    t,d,m = Collatz.CollatzGlideCounts(n)

    if n == 0:
        return "%i,%i,%i,%i\n" % (d, n, 0, t)
    else:
        return "%i,%i,%i,%i\n" % (d, n, len(str(n)), t)

def GlideLogLines(l):
    return [GlideLogLine(n) for n in l]

if base == 1:
    outFile.write(GlideLogLine(0))
    outFile.flush()

while args.max is None or exponent < args.max:
    inputFile.seek(0)

    big_glides = []
    for line in inputFile:
        fields = line.split(',')
        if int(fields[0]) > exponent:
            big_glides.append(base+int(fields[1]))

    outFile.write(''.join(GlideLogLines(big_glides)))
    outFile.flush()

    # Increment for the next loop
    exponent += 1
    base *= 2
