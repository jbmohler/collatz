// GlideResidues.cxx
//
//

#include "CollatzLib.h"
#include <fstream>
#include <string>

int main( int argc, char **argv )
{
   CFloating *remainders;
   int exponent;

   ifstream x;
   x.open( argv[1] );
   int count = 0;
   string s;
	cerr << "Loading..." << endl;
   while( x >> s ){
      switch( *s.data() ){
      case 'E': // Exponent
//         cout << '>';
         exponent = atoi( s.data() + 1 );
         break;
      case 'C': // Count
//         cout << '<';'
         cerr << "Count:  " << atoi( s.data() + 1 ) << endl;
         remainders = new CFloating[atoi( s.data() + 1 )];
         break;
      default:
//         cout << '.';
         if( !(count % 1000) )
            cerr << count << endl;
         String2Float( s.c_str(), 10, remainders[count++] );
         break;
      }
   }

   CFloating base;
   base.AssignInt( 1 );
   for( int i = 0; i < exponent; i++ ){
      base.MultiplyByInt( 2 );
   }

   CFloating curr;
   curr.AssignInt( 0 );
   if( argc > 2 )
      String2Float( argv[2], 10, curr );
   while( true ){
      CFloating thisOne;
      for( int j = 0; j < count; j++ ){
         thisOne.Add( &curr, &remainders[j] );
         PrintFloating( cout, thisOne, 10 );
         cout << ',';
         ComputeGlide( cout, thisOne );
         cout << endl;
      }

      thisOne.Add( &curr, &base );
      curr = thisOne;
   }

   delete []remainders;

   return 0;
}
