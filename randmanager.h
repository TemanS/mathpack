#ifndef RANDMANAGER_H
#define RANDMANAGER_H

// inverseTerms table
//
// Inverse Terms are problems that are presented as 4 + 6 and 6 + 4.
// We want to minimize those to present the user with unique terms
// for each problem.
//
// rm_none - no inverse terms allowed, e.g. 6,5,4 4,5,6, and 5,6,4
//           are not allowed
// rm_one  - Only one of the terms must be unique.
// rm_all  - allow any and all inverse terms
//
enum rm_inverse {rm_none, rm_some, rm_all};

#define DEFAULT_INVERSE_TERMS rm_some
#define DEFAULT_DUPLICATES 2
#define DEFAULT_ZEROS 1
#define DEFAULT_ONES 1

#include <QList>
#include <QVector>

class RandManager
{
public:
    RandManager();
    RandManager(int terms, int probs, QList<int>& mins, QList<int>& maxs);
    void init(int terms, int probs, QList<int>& mins, QList<int>& maxs);
    bool isStale(QList<int> vals);
    void setInverseTerms(int state) {inverseTerms = state;}
    int getInverseTerms() {return inverseTerms;}

private:
    int dimension;          // The number of terms to track
    int problems;           // The number of problems being presented
    int inverseTerms;       // Max number of stale terms allowed in a problem
    QList<int> minList;     // List of min values of terms for each dimension
    QList<int> maxList;     // List of max values of terms for each dimension

    // QVector vVal is a container for the QLists of values for each term.
    // Consider a problem set that can require up to three terms in the problem.
    //
    // vVal[0] is a QList of values that have been presented for the 1st term
    // vVal[1] is a QList of values that have been presented for the 2nd term
    // vVal[2] is a QList of values that have been presented for the 3rd term
    //
    QVector<QList<int> > vVal;    // Array of Lists of values that have already
                                 // been presented.

    QList<int>& getValues(int index, int terms, QList<int>& column);
    bool checkInverseTerms(QList<int>& vals, QList<int>& col);
    bool checkUniqueTerms(QList<int>& vals, QList<int>& col);
};

#endif // RANDMANAGER_H
