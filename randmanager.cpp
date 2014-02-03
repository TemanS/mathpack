// This class manages the uniqueness of terms presented
// in math problems.
// Random number generators, with small max and min limits,
// often generate the same numbers. This class provides
// functionality to track the the terms required in an
// expression to guarantee their uniqueness.
//

#include <QtGlobal>
#include <climits>
#include "randmanager.h"

RandManager::RandManager()
{
}

RandManager::
RandManager(int terms, int probs, QList<int> &mins, QList<int> &maxs)
{
    init(terms, probs, mins, maxs);
}

void RandManager::
init(int terms, int probs, QList<int> &mins, QList<int> &maxs)
{
    minList.clear();
    maxList.clear();

    dimension = terms;
    problems = probs;
    minList = mins;
    maxList = maxs;

    // Create a new QList<int> for each term in the problem to be presented
    // These lists will be used to store values that have already been
    // presented in problems.
    //
    for(int i = 0; i < dimension; ++i)
        vList << new QList<int>;

    // If the caller only sent one min and one max value, then the
    // min/max for each dimension(term) is the same.
    //
    if(minList.size() == 1)
        for(int i = 0; i < dimension; ++i)
            minList[i] = minList[0];

    if(maxList.size() == 1)
        for(int i = 0; i < dimension; ++i)
            maxList[i] = maxList[0];

    // Let's start by setting inverseOK to false. This means that we will
    // consider terms 2,3 and 3,2 to be the same.
    //
    inverseTerms = DEFAULT_INVERSE_TERMS;
}

QList<int>& RandManager::getValues(int index, QList<int> &currentColumn)
{
    for(int i = 0; i < vList.size(); ++i)
        if(index < vList[i]->size())
            currentColumn << vList[i][index];

    return currentColumn;
}

bool RandManager::isStale(QList<int> vals)
{
    bool stale = false;
    QList<int> currCol;     // Current column of values

    //////////////////////////////////////////////////////////////////////
    //
    // It is possible to have fewer terms in some problems than in others.
    // Lowest common multiples and greatest common factors, for example.
    // Nevertheless, we will always add the terms of the problem to their
    // corresponding lists. If there is no term corresponding to a list,
    // we will add INT_MIN (see climits header). This assures that the
    // lists will always be the same length.
    //
    // We don't need to worry about the size of the lists getting too big,
    // because they are inherently limited by the number of problems.
    //
    //////////////////////////////////////////////////////////////////////

    // Assuming that all the lists in vList are the same size, because we
    // always add the vals passed to us to the end of the list.
    //
    for(int i = 0; i < vList[0]->size(); ++i) {
        currCol = getValues(i, currCol);

        // If we find the exact same terms in the list, return isStale true.
        //
        if(currCol == vals)
            return true;

        // Check for the maximum allowable number of inverse terms.
        // See the table in randmanager.h
        //
        switch(inverseTerms) {
        case 0:
            for(int j; j < vals.size(); ++j) {
                for(int k = 0; k < currCol.size(); ++k) {
                    if(vals[j] == currCol[k])
                        return true;
                }
            }
            return false;

        case 1: break;
        case 2: break;
        default: break;
        }
    }

    return stale;
}
