#!/usr/bin/env python
import zmq
import argparse

parser = argparse.ArgumentParser('worker thread for collatz things')
parser.add_argument('addr', nargs=1, type=str, help='0mq address')
args = parser.parse_args()

context = zmq.Context()

s1 = context.socket(zmq.REQ)

s1.connect(args.addr[0])


s1.send_pyobj(('glides', []))
print s1.recv_pyobj()

s1.send_pyobj(('glides', [1]))
print s1.recv_pyobj()

s1.send_pyobj('1+2')
print s1.recv_pyobj()

s1.send_pyobj(('glides', [2,3,4]))
print s1.recv_pyobj()

s1.send_pyobj('quit')
#print s1.recv_pyobj()
