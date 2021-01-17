// group_by.cxx
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
#include <fstream>

int main( int argc, char **argv )
{
   if( argc < 2 ){
      cerr << "You must specify a Number" << endl;
      return 1;
   }

   ifstream inputFile;
	inputFile.open( argv[1], fstream::in );
	ofstream outputFile;
	outputFile.open( argv[2], fstream::out );

	char lineArray[50];
	string line;
	while( inputFile >> line ){
		strcpy( lineArray, line.c_str() );
      char *x = strtok( lineArray, "," ); // divides
		outputFile << x << ",";
		x = strtok( NULL, "," );

		CFloating in;
		String2Float( x, 10, in );
		string s;
		Float2String( in, 16, s );
      outputFile << s << "," << s.length() << ",";

		x = strtok( NULL, "," );
		x = strtok( NULL, "," );
		outputFile << x << endl;
	}

   return 0;
}
