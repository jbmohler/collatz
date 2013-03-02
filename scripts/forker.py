#!/usr/bin/env python

import os
import sys
import collatz
import pickle
import multiprocessing

maxtwowidth = 8*3+4
processes = 4

xxx = 6
twowidth = xxx*4
multiplier = 0

divides=pickle.loads(open('data/divides-{0}.pickle'.format('x'*xxx), 'r').read())

sem = multiprocessing.Semaphore(processes)

prodigy = [multiplier]

while True:
    parent = True
    for i in range(maxtwowidth-twowidth):
        sem.acquire()
        if 0 == os.fork():
            parent = False
            break

    if parent:
        sys.exit(0)

    twowidth += i
    multiplier += 2**(twowidth-xxx*4)
    prodigy.append(multiplier)
    print ' -> '.join([bin(x) for x in prodigy])
    twoexp = 2**twowidth

    candidates = [twoexp+x[1] for x in divides if x[0]>twowidth]
    divides = collatz.GlideDividesList(candidates)
    candidates = None

    twowidth += 1

    open('data/divides-{0:x}{1}.pickle'.format(multiplier, 'x'*xxx), 'w').write(pickle.dumps(divides))

    sem.release()
