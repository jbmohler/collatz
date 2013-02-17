#!/usr/bin/env python
import Collatz
import argparse

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

if base == 1:
    outFile.write(Collatz.GlideLogLine(0))
    outFile.flush()

class WorkHunk(object):
    def __init__(self, l, index):
        self.l = l

    def join(self):
        return Collatz.GlideLogLines(self.l)

while args.max is None or exponent < args.max:
    inputFile.seek(0)

    big_glides = []
    for line in inputFile:
        fields = line.split(',')
        if int(fields[0]) > exponent:
            big_glides.append(base+int(fields[1]))

    w = WorkHunk(big_glides)

    outFile.write(''.join(w.join()))
    outFile.flush()

    # Increment for the next loop
    exponent += 1
    base *= 2
