// glide.cxx
//
//  Utility to compute the glide for a single number
//
// Author        : Joel B. Mohler
// Last Modified : 2003.05.24
//

#include <iostream>
#include "CollatzLib.h"

using namespace std;

int main( int argc, char **argv )
{
   if( argc < 2 ){
      cerr << "You must specify a Number" << endl;
      return 1;
   }

   CFloating in;
   String2Float( argv[1], 10, in );
   ComputeGlide( cout, in );
}
