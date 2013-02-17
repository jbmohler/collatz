// CollatzLib.h
//
//  Utilities for the Collatz Sequence
//
// Author        : Joel B. Mohler
// Last Modified : 2003.05.24
//

#include "floatfunc.h"
#include <iostream>
#include <string>

void ComputeGlide( int *div, int *mult, std::string *str, std::ostream *stream, CFloating &in );
void ComputeGlide( int &div, int &mult, CFloating &in );
void ComputeGlide( std::string &s, CFloating &in );
void ComputeGlide( std::ostream &s, CFloating &in );
