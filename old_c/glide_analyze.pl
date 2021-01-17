#!/usr/bin/perl
use IO::File;
use DBI;

my $dbh = DBI->connect( "DBI:mysql:database=collatz;host=localhost", 'root', '');

$fh = new IO::File;
if( $fh->open( "<$ARGV[0]" ) ){
   while( $line = <$fh> ){
      my @data = split( /,/, $line );

      my $sth = $dbh->prepare( "select * from families where glide='$data[1]';" );
		$sth->execute();

		if( !defined $sth->fetchrow ){
		   $sth = $dbh->prepare( "insert into families set number='$data[0]', glide='$data[1]'" );
			$sth->execute;
		}

      #if( !defined $big_list{$data[1]} ){
      #   $big_list{$data[1]} = $data[0];
      #   print $line;
      #}
   }
   $fh->close();
}
