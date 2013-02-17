#!/usr/bin/perl

# print $ARGV[0];
#open( INF, $ARGV[0] ) or die "problem";
while( $line = <STDIN> ){
	my @stuff = split( /,/, $line );
	$stuff[1] = ` ./n $stuff[1] 10 16 `;
	$stuff[2] = length( $stuff[1] );
	printf( "%s,%s,%s,%s", $stuff[0], $stuff[1], $stuff[2], $stuff[3] );
}
