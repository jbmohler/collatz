#!/usr/bin/perl
use IO::File;
use DBI;

# This query does what this file does
#set option SQL_SELECT_LIMIT=Default;
#select family into outfile '/tmp/residue_24.txt' lines terminated by '\n' from families where ((family<'16777216' and family_len=8) or family_len < 8) and divides > 24 order by inc

$dbh = DBI->connect( "DBI:mysql:database=collatz;host=localhost", 'root', '');

$upperBoundLog2 = 8;

$limit = 1;
for( $i = $upperBoundLog2; $i > 0; $i-- ){
   $limit *= 2;
}

#@possibilities = (

for( $i = 1; $i < $limit; $i += 1 ){
   # We want to check for this number in the database
	if( CheckNumber( $i, $upperBoundLog2 ) == 1 ){
	   print "$i\n";
	}
}

sub CheckNumber {
   my ($n, $m2exp) = @_;
	$result = 1;

	$two = 1;
LP:for( $x = 1; $x <= $m2exp; $x++ ){
	   $two *= 2;
		if( $two > $n ){
		   last LP;
		}

		$base = $n % $two;
      $result = Query( $base, $x, $x );
		if( $result == 0 ){
		   last LP;
		}
	}

	if( $result == 1 ){
	   $result = Query( $n, $x, $m2exp );
	}

	return $result;
}

sub Query {
   my ($familyBase, $minDiv, $maxDiv) = @_;

	$sql = "select * from families where family='$familyBase' and divides >= $minDiv and divides <= $maxDiv";
#	print "$sql\n";
	$sth = $dbh->prepare( "select * from families where family='$familyBase' and divides >= $minDiv and divides <= $maxDiv" );
	$sth->execute();

	return defined $sth->fetchrow ? 0 : 1;
}
