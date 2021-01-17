// hex_glide.cxx
//
//  Glide sequence viewer in base 6
//
// Author        : Joel B. Mohler
//

#define BASE  2

#include "integer.h"

using namespace std;

int main( int argc, char **argv )
{
   if( argc < 2 ){
      cerr << "You must specify a Number" << endl;
      return 1;
   }

   CInteger in;
   in.AssignString( argv[1], -1, BASE );

	CInteger x( BASE );
	x.ConvertBase( &in );
	in = x;

   do
   {
      cout << CNumberStream( x ).SetBase( BASE ) << endl;

      CInteger y;
      if( !(x % 2) )
      {
         y = x / 2;
         x = y;
      }
      else
      {
         y = 3 * x;
         x = y+1;
      }
   }while( !in.GreatEqual( &x ) );

   cout << CNumberStream( x ).SetBase( BASE ) << endl;

	CInteger one( BASE );
	one.AssignInt( 1 );

   if( x.Great( &one ) && x.Equal( &in ) ){
      cerr << "\n************* Collatz Loop Detected ***********************\n";
      cerr << in << endl << x;
      cerr << "\n***********************************************************\n";
   }
}
