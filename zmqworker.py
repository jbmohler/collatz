#!/usr/bin/env python
import zmq
import argparse
import Collatz

parser = argparse.ArgumentParser('worker thread for collatz things')
parser.add_argument('addr', nargs=1, type=str, help='0mq address')
args = parser.parse_args()

context = zmq.Context()

s1 = context.socket(zmq.REP)

s1.bind(args.addr[0])

while True:
    obj = s1.recv_pyobj()

    if obj == 'quit':
        break

    if obj[0] == 'glides':
        #print "glides for:  ", len(obj[1])
        s1.send_pyobj(Collatz.GlideLogLines(obj[1]))
    else:
        s1.send_pyobj(eval(obj))

