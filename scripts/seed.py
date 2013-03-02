#!/usr/bin/env python
import argparse
import collatz
import pickle

parser = argparse.ArgumentParser(description='collatz divide seed generator')
#parser.add_argument('exponent', nargs=1, type=int, help='initial exponent (likely 0)')
#parser.add_argument('datafile', nargs=1, help='text file to store glide results (likely new empty file)')
parser.add_argument('-m', '--max', default=None, type=int, help='max exponent')
parser.add_argument('-t', '--threads', default=1, type=int, help='maximum threads')
args = parser.parse_args()

#args.datafile = args.datafile[0]
#args.exponent = args.exponent[0]

args.exponent = 0

assert args.max % 4 == 0, "This should seed with an even hex digit for file name convenience"

#outFile = open(args.datafile, "a")
#inputFile = open(args.datafile, "r")

base = 2**args.exponent
exponent = args.exponent

divides = []

if base == 1:
    divides.append((collatz.GlideDivides(0), 0))

while args.max is None or exponent < args.max:
    big_glides = [base+n for div, n in divides if div > exponent]

    divides += collatz.GlideDividesList(big_glides)

    # Increment for the next loop
    exponent += 1
    base *= 2

fname = 'data/divides-{0}.pickle'.format('x'*(args.max//4))
open(fname, 'w').write(pickle.dumps(divides))
