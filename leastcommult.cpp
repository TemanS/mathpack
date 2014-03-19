#include <QtCore/qmath.h>
#include "leastcommult.h"

LeastComMult::LeastComMult()
{
}

///////////////////////////////////////////////////////////////////////////////
//
// getLeastCommonMultiple
//	Uses prime numbers to determine the least common multiple with
//	a table.
//
//	See http://en.wikipedia.org/wiki/Least_common_multiple about half
//	way down the page.
//
// Arguments
//	numArray - QVector of numbers to process
//
// Returns
//	lcm - Least Common Multiple
//
int LeastComMult::getLeastCommonMultiple(QVector<int>& numArray)
{
    int index;
    int count = numArray.count();
    int lcd = 1;
    int primeNumber = 2; 	// Start with lowest prime number

    QVector<int> lcdTable(numArray);

    // When the sum of the integers in the lcdTable is equal to their
    // count, then all the integers in the lcdTable have been reduced
    // to the number 1.
    //
    while(sumTable(lcdTable) > count) {
        bool match = false;

        for(index = 0; index < count; index++) {
            if(lcdTable[index] % primeNumber != 0)
                continue;
            else
                lcdTable[index] /= primeNumber;

            if(match == false) {
                lcd *= primeNumber;
                match = true;
            }
        }

        if(match == false)
            primeNumber = getNextPrime(primeNumber);
    }

    return lcd;
}

///////////////////////////////////////////////////////////////////////////////
//
// sumTable
//	Sum the integers in the table (QVector) passed by reference.
//
// Arguments
//	table	- reference to a QVector of integers
//
// Returns the sum of the integers in the array.
//
int LeastComMult::sumTable(QVector<int>& table)
{
    int index;
    int count = table.count();
    int sum = 0;

    for(index = 0; index < count; index++)
        sum += table[index];

    return sum;
}

///////////////////////////////////////////////////////////////////////////////
//
// getNextPrime
//
// Recursive function to Find the next prime number up from the one passed.
//
// Globals : none
// Arguments
//	prime - a prime number number
//
// Returns the next prime number
//
int LeastComMult::getNextPrime(int prime)
{
    int next = prime;
    int index;

    // If a number less than or equal to 2 is passed, then the next prime
    // number up from that is 3. Even though 2 is the smallest prime,
    // this function will tolerate numbers less than 2.
    //
    if(prime <= 2)
        return 3;

    next += 2;

    if(next %2 == 0)
        next = getNextPrime(next);

    for(index = 3; index <= qSqrt(next); index += 2)
        if(next % index == 0)
            next = getNextPrime(next);

    return next;
}
