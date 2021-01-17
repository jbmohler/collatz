// bin_glide.cxx
//
//  Glide sequence viewer in binary
//
// Author        : Joel B. Mohler
// Last Modified : 2003.06.07
//

#include "CollatzLib.h"

using namespace std;

void PrintInvertedTruncated( CFloating &x, int t )
{
   for( int i = 0; i < t; i++ ){
	   cout << x.GetAt( i );
	}
	cout << endl;
}

int main( int argc, char **argv )
{
   if( argc < 2 ){
      cerr << "You must specify a Number" << endl;
      return 1;
   }

   CFloating in;
   String2Float( argv[1], 10, in );

	CFloating x( 2 );
	x.ConvertBase( &in );
	in = x;

	int truncate = 0;

   do
   {
      CFloating y;
      if( !(x % 2) )
      {
         y = x / 2;
         x = y;
			truncate++;
      }
      else
      {
         y = 3 * x;
         x = y+1;
      }
   }while( !in.GreatEqual( &x ) );

	x = in;

   do
   {
	   PrintInvertedTruncated( x, truncate );

      CFloating y;
      if( !(x % 2) )
      {
         y = x / 2;
         x = y;
			truncate--;
      }
      else
      {
         y = 3 * x;
         x = y+1;
      }
   }while( !in.GreatEqual( &x ) );

	CFloating one( 2 );
	one.AssignInt( 1 );

   if( x.Great( &one ) && x.Equal( &in ) ){
      cerr << "\n************* Collatz Loop Detected ***********************\n";
      PrintFloating( cerr, in, 10 );
      cerr << endl;
      PrintFloating( cerr, x, 10 );
      cerr << "\n***********************************************************\n";
   }
}
