import collections
from . import _compute

def ofdividesabbr(twopower):
    preimage = [i for i in xrange(2**twopower) if _compute.GlideDivides(i) == twopower]

    groups = collections.defaultdict(list)
    for i in preimage:
        groups[_compute.GlideString(i).replace('10', '1')].append(i)
    return groups

def ofdivides(twopower):
    preimage = [i for i in xrange(2**twopower) if _compute.GlideDivides(i) == twopower]

    groups = collections.defaultdict(list)
    for i in preimage:
        groups[_compute.GlideString(i)].append(i)
    return groups

def possibleglides(divides):
    def onemult(glide, num, den):
        num *= 3
        glide += '1'
        while den < num and glide.count('0') < divides:
            den *= 2
            glide += '0'
            if den > num:
                yield glide
            if den < num:
                for g in onemult(glide, num, den):
                    yield g
    if divides > 0:
        yield '0'
    for g in onemult('', 1, 1):
        yield g

def summarize(maxtwopower, func=ofdividesabbr):
    summary = {}
    for i in range(maxtwopower+1):
        summary.update(func(i))
    return summary

def printdict(d):
    for k in sorted(d.keys()):
        print "{0} -> {1}".format(k, d[k])
