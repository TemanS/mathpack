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

/**********************************
 ** PUBLIC ROUTINES
 *********************************/

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

    // Create an array of QList<int> for each term (dimension) in the
    // problem to be presented. This should be the maximum number of
    // terms that can be expected for a given problem type. Some problems
    // can only have two terms, while others can have two or more.
    // These lists will be used to store values that have already been
    // presented in problems.
    //
    vVal.resize(dimension);

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

// bool RandManager::isStale - determines whether the values passed are stale
//
// QList vals - a list of values, which can be thought of as a "column"
//              These are the values that are being proposed for presentation
//              to the user. This function will determine if these values
//              are "stale" based on whether they are present anywhere else
//              in the array (QVector vVal) and depending on the "staleness"
//              policy.
//
//              It is possible that the vals.size() can be less than the
//              dimension originaly passed. For example, this may be a
//              problem set that can have up to four terms, but this
//              particular problem in that set is only presenting two
//              terms, e.g. 2+5 instead of 2+5+7.
//
//              rm_none - no inverse terms allowed (4+5 and 5+4 not allowed)
//              rm_one  - only one of the terms needs to be unique
//              rm_all  - allow any and all inverse terms
//
// Returns true or false, based upon whether the terms are considered stale
// according to the "staleness" policy.
//
bool RandManager::isStale(QList<int> vals)
{
    bool stale = false;
    int terms = vals.size();    // The number of terms in this problem
    int sofar = vVal[0].size(); // The number of unique terms so far
    QList<int> aCol;            // A column of values

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

    for(int i = 0; i < sofar; ++i) {
        aCol = getValues(i, terms, aCol);

        // If we find the exact same terms in the list, return isStale true.
        //
        if(aCol == vals)
            return true;

        // Check for the maximum allowable number of inverse terms.
        // See the table in randmanager.h
        //
        switch(inverseTerms) {
        case rm_none: stale = checkInverseTerms(vals, aCol); break;
        case rm_some: stale = !checkUniqueTerms(vals, aCol); break;
        case rm_all: break;
        } // end switch

        if(stale)
            return stale;
    }

    // If we got here, it means we do not have stale terms, as defined by
    // the "staleness" policy, so let's add the terms to the lists.
    //
    for(int i = 0; i < vVal.size(); ++i)
        vVal[i] << (i < terms ? vals[i] : INT_MIN);

    return false;
}

/**********************************
 ** PRIVATE ROUTINES
 *********************************/

QList<int>& RandManager::
getValues(int index, int terms, QList<int> &column)
{
    for(int i = 0; i < terms; ++i)
        column << vVal[i][index];
    return column;
}

bool RandManager::
checkInverseTerms(QList<int>& vals, QList<int>& col)
{
    int terms = vals.size();
    for(int j; j < terms; ++j)
        for(int k = 0; k < terms; ++k)
            if(vals[j] == col[k])
                return true;
    return false;
}

bool RandManager::
checkUniqueTerms(QList<int>& vals, QList<int>& col)
{
    int terms = vals.size();
    for(int i = 0; i < terms; ++i)
        if(vals[0] == col[i])
            return false;
    return true;
}
