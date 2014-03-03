#ifndef RANDMANAGER_H
#define RANDMANAGER_H


#include <QList>
#include <QVector>
#include <randomc.h>

#define DEFAULT_INVERSE_TERMS rm_one
#define SMALLEST_NUM 2  // Smallest number for problem terms
#define MAX_SMALLNUM 1  // Max quantity of numbers less than MIN_SMALLNUMBER

#define abs(x) (x < 0 ? (x * -1) : x)

class RandManager
{
public:
    RandManager(){}
    RandManager(int terms, int probs, QList<int>& mins, QList<int>& maxs);
    void init(int terms, int probs, QList<int>& mins, QList<int>& maxs);
    bool isStale(QList<int> vals, int terms);
    QList<int>& getTerms(QList<int>& vals);
    QList<int>& getTerms(QList<int> &vals, int terms);
    int& getTerms(int& term);

private:
    int dimension;          // The number of terms to track
    int problems;           // The number of problems being presented
    int smallcount;         // Count quantity of small number terms
    QList<int> minList;     // List of min values of terms for each dimension
    QList<int> maxList;     // List of max values of terms for each dimension
    CRandomMersenne *pRand; // Pointer to a random number gernerating class

    // QVector vVal is a container for the QLists of values for each term.
    // Consider a problem set that can require up to three terms in the
    // problem.
    //
    // vVal[0] is a QList of values that have been presented for the 1st term
    // vVal[1] is a QList of values that have been presented for the 2nd term
    // vVal[2] is a QList of values that have been presented for the 3rd term
    //
    QVector<QList<int> > vVal;  // Array of Lists of values that have already
                                // been presented.

    QList<int>& getValues(int index, int terms, QList<int>& column);
    bool checkInverseTerms(QList<int>& vals);
};

#endif // RANDMANAGER_H
