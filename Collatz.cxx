// Collatz.cxx
//

#include "stdio.h"

typedef int BIG_INT;

BIG_INT optArray[1000];

//:Collatz
// Arguments:  starting point BIG_INT, maximum amount of terms to check
// Return:  -1, is maxTerms is reached, else a positive BIG_INT
//    If maxTerms == -1 then the function will not return if the number never gets to one
BIG_INT Collatz( BIG_INT start, BIG_INT maxTerms )
{
  BIG_INT result( -1 );
  BIG_INT i( start );

  if( start > 0 && start < 1000 )
    result = optArray[start - 1];

  if( result < 0 )
  {
    result = 1;

    while( i != 1 )
    {
      if( i % 2 )
        i = 3 * i + 1;
      else
        i = i / 2;

      if( i > 0 && i < 1000 )
      {
        result += Collatz( (BIG_INT)i, (maxTerms > 0) ? (maxTerms - result) : -1 );
        break;
      }
      else
      {
        result++;
        if( result == maxTerms )
        {
          result = -1;
          break;
        }
      }
    }

    if( start > 0 && start < 1000 )
      optArray[start - 1] = result;
  }

  return result;
}

//:VerboseCollatz
// Arguments:  starting point BIG_INT
// Return:  -1, is maxTerms is reached, else a positive BIG_INT
//    If maxTerms == -1 then the function will not return if the number never gets to one
void VerboseCollatz( BIG_INT start, int &divs, int &mults )
{
   BIG_INT i( start );
   bool bComma = false;

   while( i != 1 )
   {
      if( i % 2 )
      {
         if( !bComma )
         {
            mults++;
            printf( "1" );
         }
         i = 3 * i + 1;
      }
      else
      {
         if( !bComma )
         {
            divs++;
            printf( "0" );
         }
         i = i / 2;
      }

      if( i < start && !bComma )
      {
         bComma = true;
//         printf( "," );
      }
   }
}

class CRemainder
{
public:
   CFloating m_remainder;
   int m_exp2;
   CRemainder *m_next;
};

class CRemainders
{
public:
   CRemainders();
   ~CRemainders();

   void Double();
   bool RemoveNumber( BIG_INT iRemove, int iPower );

   bool IsCandidate( BIG_INT x );
   
   BIG_INT GetExponent() { return m_iPowerTwo; };
   
   void Show();

protected:
   int FindIndex( BIG_INT x );

   int m_iPowerTwo;
   BIG_INT m_iMod;
   BIG_INT *m_array;
   int m_arraySize;
};

CRemainders::CRemainders()
{
   m_iPowerTwo = 2;
   m_iMod = 4;
   m_arraySize = 1;
   m_array = new BIG_INT[m_arraySize];
   m_array[0] = 3;
}

CRemainders::~CRemainders()
{
   if( m_array )
      delete []m_array;
}

void CRemainders::Double()
{
   BIG_INT *newArray = new BIG_INT[m_arraySize * 2];

   for( BIG_INT i = 0; i < m_arraySize; i++ )
   {
      newArray[i] = m_array[i];
      newArray[i+m_arraySize] = m_array[i] + m_iMod;
   }

   m_arraySize *= 2;
   m_array = newArray;

   m_iMod *= 2;
   m_iPowerTwo++;
}

bool CRemainders::RemoveNumber( BIG_INT iRemove, int iPower )
{
  int diff = 1,
    mod = 1;
  mod = mod << iPower;
  diff = diff << (m_iPowerTwo - iPower);
  for( int j = 0; j < diff; j++ )
    {
   int index = FindIndex( mod * j + iRemove );

   if( index >= 0 )
   {
      BIG_INT *newArray = new BIG_INT[m_arraySize - 1];

      int i;
      for( i = 0; i < index; i++ )
      {
         newArray[i] = m_array[i];
      }

      for( i = index; i < m_arraySize - 1; i++ )
      {
         newArray[i] = m_array[i+1];
      }

      m_arraySize -= 1;
      m_array = newArray;
   }
    }

   return true;
}

bool CRemainders::IsCandidate( BIG_INT x )
{
   return FindIndex( x % m_iMod ) != -1;
}

void CRemainders::Show()
{
   printf( "Mod %i, Size %i:  ", m_iMod, m_arraySize );

   if( m_arraySize < 40 )
     {
   for( int i = 0; i < m_arraySize; i++ )
   {
      printf( " %i", m_array[i] );
   }
     }
   printf( "\n" );
}

int CRemainders::FindIndex( BIG_INT x )
{
   int index = -1;

   int mod = x % m_iMod;

   for( int i = 0; i < m_arraySize && index == -1; i++ )
   {
      if( mod == m_array[i] )
         index = i;
   }

   return index;
}


BIG_INT main()
{
   {
      for( BIG_INT i = 0; i < 1000; i++ )
         optArray[i] = -1;
   }

/*
  for( BIG_INT i = 1; i < 100000; i++ )
  {
    printf( "%i\t%i\n", (int)i, Collatz( i, -1 ) );
  }
*/
/*
   int mod = 256;
   int validremainders[] = { 27, 31, 47, 63, 71, 91, 103, 111, 127, 155, 159, 167, 63+128, 79+128, 95+128, 231, 239, 251, 255 };
   int validcount = 19;*/
   
   CRemainders rm;
   rm.Show();

   for( BIG_INT i = 2; i < 10000; i++ ){
//      bool bUse = false;
//
     //   rm.Show();
      bool bUse = rm.IsCandidate( i );
      //for( int x = 0; x < validcount; x++ ){
      //   if( validremainders[x] == i % mod )
      //      bUse = true;
      //}

      if( bUse )
      {
         printf( "%.8i,", i );
         int divs = 0,
            mults = 0;
         VerboseCollatz( i, divs, mults );
         printf( "\n" );

         if( divs < 30 )
         {
            while( rm.GetExponent() < divs )
            {
               rm.Double();
            }
            rm.RemoveNumber( i, divs );
            rm.Show();
         }
         else
         {
            printf( "Skipping Recalibrate\n" );
         }
      }
   }
   return 0;
}
