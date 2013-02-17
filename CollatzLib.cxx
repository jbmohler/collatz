// CollatzLib.cxx
//
//  Utilities for the Collatz Sequence
//
// Author        : Joel B. Mohler
// Last Modified : 2003.05.24
//

#include "CollatzLib.h"

using namespace std;

// A helper function
void ComputeGlide( int *div, int *mult, string *str, ostream *stream, CFloating &in )
{
    if( div )
	    *div = 0;
	if( mult )
	    *mult = 0;
   CFloating one;
   one.AssignInt( 1 );

   CFloating x;
   x = in;
   do
   {
      CFloating y;
      if( !(x % 2) )
      {
		   if( stream ){
            (*stream) << "0";
			}
			if( str ){
			   *str += "0";
			}
			if( div )
			    (*div)++;
         y = x / 2;
         x = y;
      }
      else
      {
		   if( stream ){
            (*stream) << "1";
			}
			if( str ){
			   *str += "1";
			}
			if( mult )
			    (*mult)++;
         y = 3 * x;
         x = y+1;
      }
   }while( !in.GreatEqual( &x ) );

   if( x.Great( &one ) && x.Equal( &in ) ){
      cerr << "\n************* Collatz Loop Detected ***********************\n";
      PrintFloating( cerr, in, 10 );
      cerr << endl;
      PrintFloating( cerr, x, 10 );
      cerr << "\n***********************************************************\n";
   }
}

void ComputeGlide( int &div, int &mult, CFloating &in )
{
   ComputeGlide( &div, &mult, NULL, NULL, in );
}

void ComputeGlide( string &s, CFloating &in )
{
   ComputeGlide( NULL, NULL, &s, NULL, in );
}

void ComputeGlide( ostream &s, CFloating &in )
{
   ComputeGlide( NULL, NULL, NULL, &s, in );
}
