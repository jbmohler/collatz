#!/usr/bin/env python

import pickle
import argparse

parser = argparse.ArgumentParser(description='collatz divide seed generator')
parser.add_argument('datafile', nargs=1, help='pickle divide file (typically divides-*.pickle)')
args = parser.parse_args()

divides=pickle.loads(open(args.datafile[0], 'r').read())

for div, n in divides:
    print '{0:x} -- {1}'.format(n,div)
