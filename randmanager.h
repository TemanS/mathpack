#ifndef RANDMANAGER_H
#define RANDMANAGER_H

// inverseTerms table
//
// 0 - no inverse terms allowed, e.g. 6,5,4 4,5,6, and 5,6,4 are not allowed
// 1 - upto two inverse terms allowed 4,5,6 and 3,5,6 would be allowed, but
//     3,2, and 2,3 would not be allowed.
// 2 - allow any and all inverse terms
//
#define DEFAULT_INVERSE_TERMS 1

#include <QList>

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
    bool inverseTerms;      // Max number of stale terms allowed in a problem
    QList<int> minList;     // List of min values of terms for each dimension
    QList<int> maxList;     // List of max values of terms for each dimension
    QList<QList<int>*> vList;    // List of Lists of values

    QList<int>& getValues(int index, QList<int>& currentColumn);
};

#endif // RANDMANAGER_H
