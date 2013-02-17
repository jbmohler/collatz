// CloseSets.cxx

#include "CollatzLib.h"
#include <iostream>

using namespace std;

int main()
{
	int power2 = 1;
	int power3 = 0;
	CFloating twos,
		threes,
		q,
		oldTwos;

	oldTwos.AssignInt( 1 );
	twos.AssignInt( 2 );
	threes.AssignInt( 1 );

	while( true ){
		threes *= 3;
		power3++;

		q *= 3;
		q += oldTwos;

		while( twos < threes ){
			power2++;
			twos *= 2;
			oldTwos *= 2;
		}

		CFloating diff;
		diff.Subtract( &twos, &threes );

//		PrintFloating( cout, twos, 10 );
		cout << 2 << "^" << power2 << ",";
//		PrintFloating( cout, threes, 10 );
		cout << 3 << "^" << power3 << ",";
		PrintFloating( cout, diff, 10 );
		cout << " , ";
		PrintFloating( cout, q, 10 );
		cout << flush;

		int bPrint = false;
		CFloating k;
//		k.AssignInt( 620000 );
		k.AssignInt( 1 );
		while( true ){
			CFloating diffByK;
			diffByK.Multiply( &k, &diff );
			if( diffByK > q )
				break;
			bPrint = true;
			int d,m;
//			cout << endl;
//			PrintFloating( cout, k, 10 );
			ComputeGlide( d, m, k );
			if( d >= power2 ){
				PrintFloating( cerr, k, 10 );
				cerr << " - " << d << endl;
			}
			k.AddAt( 0, 1 );
		}
		cout << ",";
//		if( bPrint )
			PrintFloating( cout, k, 10 );
//		else
//			cout << "620000";
		cout << endl;
	}
}
