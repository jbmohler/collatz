#!/usr/bin/env python
import Collatz
import argparse
import zmq

parser = argparse.ArgumentParser(description='collatz sequence generator')
parser.add_argument('exponent', nargs=1, type=int, help='initial exponent (likely 0)')
parser.add_argument('datafile', nargs=1, help='text file to store glide results (likely new empty file)')
parser.add_argument('-m', '--max', default=None, type=int, help='max exponent')
parser.add_argument('-t', '--threads', default=1, type=int, help='maximum threads')
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

context = zmq.Context()

class WorkHunk(object):
    def __init__(self, l, index):
        self.l = l
        self.socket = context.socket(zmq.REQ)
        self.socket.connect('tcp://127.0.0.1:{0}'.format(5555+index))
        self.socket.send_pyobj(('glides', self.l))

    def join(self):
        #return Collatz.GlideLogLines(self.l)
        return self.socket.recv_pyobj()

while args.max is None or exponent < args.max:
    inputFile.seek(0)

    big_glides = []
    for line in inputFile:
        fields = line.split(',')
        if int(fields[0]) > exponent:
            big_glides.append(base+int(fields[1]))

    blocksize = len(big_glides)//args.threads
    workers = []
    for i in range(args.threads):
        d = big_glides[i*blocksize:(i+1)*blocksize]
        if i == args.threads - 1:
            d = big_glides[i*blocksize:]
        workers.append(WorkHunk(d, i))

    for w in workers:
        outFile.write(''.join(w.join()))
        outFile.flush()

    #w = WorkHunk(big_glides)
    #outFile.write(''.join(w.join()))
    #outFile.flush()

    # Increment for the next loop
    exponent += 1
    base *= 2
