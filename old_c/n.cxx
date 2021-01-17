// n.cxx
//
//  Base conversion utility
//
// Author        : Joel B. Mohler
// Last Modified : 2003.06.26
//

#include "stdlib.h"
#include "stdio.h"
#include "iostream.h"
#include "floatfunc.h"

int main( int argc, char **argv )
{
   if( argc < 2 ){
      cerr << "You must specify a Number" << endl;
      return 1;
   }

   CFloating in;
   String2Float( argv[1], atoi( argv[2] ), in );
   string s;
   Float2String( in, atoi( argv[3] ), s );
   cout << s;// << endl;

   return 0;
}
