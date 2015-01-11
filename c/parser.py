#!/usr/bin/env python
import sys

def pair(line):                                                                      
    return int(line[:16], 16), int(line[18:])

def collatz_n():                                                                       
    for line in open('collatz_data.txt', 'r').readlines():
        yield pair(line)

def glide_divisions(divides):
    for n, divides_ in collatz_n():
        if divides_ == divides:
            yield n

def candidates(exponent):
    for c in (n for n, divides in collatz_n() if divides>exponent and n<2**exponent):
        yield c

def candidate_count(exponent):
    return sum(1 for _ in (n for n, divides in collatz_n() if divides>exponent and n<2**exponent))

if __name__ == '__main__':
    for line in sys.stdin.readlines():
        print pair(line)
