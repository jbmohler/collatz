// filter.cxx
//
// A collatz family filter
// Author		  : Joel B. Mohler
//

#define BASE		10

#define _LARGEFILE64_SOURCE

//#include "mysql.h"
//#include "stdio.h"
#include <iostream>
#include <fstream>
#include "CollatzLib.h"

using namespace std;

//#define MYSQL_FILE
#define TEXT_FILE

int main( int argc, char **argv ){
#ifdef MYSQL_FILE
	MYSQL mysql;

	mysql_init(&mysql);
	mysql_options(&mysql,MYSQL_READ_DEFAULT_GROUP,"collatz_filter");
	if (!mysql_real_connect(&mysql,"localhost","root","","collatz",0,NULL,0)){
		fprintf(stderr, "Failed to connect to database: Error: %s\n", mysql_error(&mysql));
	}
#endif

#ifdef TEXT_FILE
	if( argc < 2 ){
		cerr << "You must supply a file name" << endl;
	}

	ifstream inputFile;
	ofstream outFile;

	outFile.open( argv[2], fstream::out | fstream::app | fstream::ate );
	inputFile.open( argv[2], fstream::in );

	if( inputFile.fail() || outFile.fail() ){
//	if( inputFile.fail() ){
//	if( outFile.fail() ){
		cerr << "Failed to open text files" << endl;
		return 1;
	}
#endif

	CFloating base;
	base.AssignInt( 1 );
	int exponent = 0;

	int startAt = 0;
	if( argc > 0 ){
		startAt = atoi( argv[1] );
		// current starting point
		while( exponent < startAt ){
			exponent++;
			base.MultiplyByInt( 2 );
		}
	}

	char mysqlQuery[4000];
	// an initial case

	if( startAt == 0 ){
#ifdef MYSQL_FILE
		sprintf( mysqlQuery, "insert into families set family='0', family_len=0, glide='0', glide_len=1, divides=1" );
		mysql_real_query( &mysql, mysqlQuery, strlen( mysqlQuery ) );
#endif
#ifdef TEXT_FILE
		sprintf( mysqlQuery, "%i,%s,%i,%i\n", 1, "0", 0, 1 );
		outFile << mysqlQuery;
		outFile.flush();
//		outFile.write( mysqlQuery, strlen( mysqlQuery ) );
//		outFile.flush();
#endif
	}

	while( true ){
		// Write the query to dig out the unfamilied
#ifdef MYSQL_FILE
		string stringRep;
		Float2String( base, BASE, stringRep );

		sprintf( mysqlQuery, "select family into outfile '/mnt/collatz/residues/residue_%i.txt' lines terminated by '\\n' from families where ((family<'%s' and family_len=%i) or family_len < %i) and divides > %i order by family_len, family", exponent, stringRep.c_str(), stringRep.length(), stringRep.length(), exponent );

		cout << mysqlQuery << endl;

		// Execute the Query
		if( mysql_real_query( &mysql, mysqlQuery, strlen( mysqlQuery ) ) ){
			fprintf(stderr, "Exponent Query Failed:  Error: %s\n", mysql_error(&mysql));
			return 1;
		}

		// Read the dump-file
		sprintf( mysqlQuery, "/mnt/collatz/residues/residue_%i.txt", exponent );
#endif

#ifdef TEXT_FILE
		inputFile.seekg( 0, ios::beg );
#endif

		string number;
#ifdef MYSQL_FILE
		ifstream dumpFile;
		dumpFile.open( mysqlQuery );
		while( dumpFile >> number ){
#endif

#ifdef TEXT_FILE
		string line;
		while( inputFile >> line ){
			strcpy( mysqlQuery, line.c_str() );
			// String (divides,family,family_len,glide_len)
			strtok( mysqlQuery, "," ); // divides
			if( atoi( mysqlQuery ) <= exponent )
				continue;
			number = strtok( NULL, "," ); // family
#endif
			CFloating family;
			String2Float( number.c_str(), BASE, family );

#ifdef TEXT_FILE
			if( family.GreatEqual( &base ) )
				break;
#endif

			CFloating thisOne;
			thisOne.Add( &base, &family );

			// We know from theory & the above query that every single number tested should go into the database
//			string s;
			int divides, multiplies;
			ComputeGlide( &divides, &multiplies, NULL, NULL, thisOne );

			string familyString;
			Float2String( thisOne, BASE, familyString );
#ifdef MYSQL_FILE
			sprintf( mysqlQuery, "insert into families set family='%s', family_len=%i, glide='%s', glide_len=%i, divides=%i", familyString.c_str(), familyString.length(), s.c_str(), divides + multiplies, divides );
			cout << mysqlQuery << endl;
			if( mysql_real_query( &mysql, mysqlQuery, strlen( mysqlQuery ) ) ){
				fprintf(stderr, "Insertion Query Failed:	Error: %s\n", mysql_error(&mysql));
				return 1;
			}
#endif
#ifdef TEXT_FILE
			sprintf( mysqlQuery, "%i,%s,%i,%i\n", divides, familyString.c_str(), familyString.length(), divides + multiplies );
//			sprintf( mysqlQuery, "%i,%s,%i,%i,%s\n", divides, familyString.c_str(), familyString.length(), divides + multiplies, s.c_str() );
			outFile << mysqlQuery;
			outFile.flush();
//			outFile.write( mysqlQuery, strlen( mysqlQuery ) );
//			outFile.flush();
#endif
		}

		// Increment for the next loop
		exponent++;
		base.MultiplyByInt( 2 );
	}

	return 0;
}
