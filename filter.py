#!/usr/bin/python
import Collatz
import sys
import argparse

def CollatzAndLog( oFile, n ):
	t,d,m = Collatz.CollatzGlideCounts( n )

#	print( "%i,%i,%i,%i\n" % (d, n, len( str( n ) ), t ) )
	unpad = str(n).lstrip( '0' )
	outFile.write( "%i,%i,%i,%i\n" % (d, n, len( unpad ), t ) )
	outFile.flush()
	

if len( sys.argv ) < 3:
	sys.stderr.write( "You must supply a file name\n" )
	sys.stderr.flush()
	sys.exit( -1 )

outFile = open( sys.argv[2], "a" )
inputFile = open( sys.argv[2], "r" )

if inputFile == None or outFile == None:
	sys.stderr.write( "Failed to open text file:  %s\n" % (sys.argv[2]) )
	sys.stderr.flush()
	sys.exit()

base = 1
exponent = 0
startAt = 0
if len( sys.argv ) > 0:
	startAt = int( sys.argv[1] )
	# current starting point
	while exponent < startAt:
		exponent = exponent + 1
		base *= 2

if startAt == 0:
	CollatzAndLog( outFile, 0 )

while True:
	inputFile.seek( 0 )

	for line in inputFile:
		fields = line.split( ',' )
		fields[0] = int( fields[0] )
		fields[1] = int( fields[1] )
		if fields[0] <= exponent:
			continue
		if fields[1] >= base:
			break
		CollatzAndLog( outFile, base + fields[1] )

	# Increment for the next loop
	exponent += 1
	base *= 2
