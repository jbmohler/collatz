#!/usr/bin/perl
use IO::File;
use DBI;

#select * from families order by divides, family_len,family

$mysql = 1;

if( $mysql ){
   $dbh = DBI->connect( "DBI:mysql:database=collatz;host=localhost", 'root', '');
}

$index = 0;

$fh = new IO::File;
if( $fh->open( "<$ARGV[0]" ) ){
FILEREAD:   while( $line = <$fh> ){
      @x = split( '\n', $line );
      $line = $x[0];

#      if( $line > 10000 ){
#         last FILEREAD;
#      }

      $index++;

      $glide = ` ./glide $line `;
      $glide_len = length( $glide );
      $num_len = length( $line );
      $divCount = 0;
      for( $i = 0; $i < $glide_len; $i++ ){
         if( substr( $glide, $i, 1 ) eq "0" ){
            $divCount++;
         }
      }
      print "$index,$line,$num_len,$glide,$glide_len,$divCount\n";

      if( $mysql && $line > 10000 ){
         $sth = $dbh->prepare( "insert into families (inc,family,family_len,glide,glide_len,divides) values ($index,'$line',$num_len,'$glide',$glide_len,$divCount)" );
         $sth->execute();
      }
   }
   $fh->close();
}
