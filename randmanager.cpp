// This class manages the uniqueness of terms presented
// in math problems.
// Random number generators, with small max and min limits,
// often generate the same numbers. This class provides
// functionality to track the the terms required in an
// expression to guarantee their uniqueness.
//

#include <QtGlobal>
#include <climits>
#include <ctime>
//#include <QDebug>
#include "randmanager.h"

/**********************************
 ** PUBLIC ROUTINES
 *********************************/

//////////////////////////////////////////////////////////////////////////////
//
// RandManager constructor
//
// terms    - the number of terms each problem can have
// probs    - the number of problems to be presented
// mins     - a list of minimum values for each term in the problem
// maxs     - a list of maximum values for each term in the problem
//
RandManager::
RandManager(int terms, int probs, QList<int> &mins, QList<int> &maxs)
{
    init(terms, probs, mins, maxs);
}

//////////////////////////////////////////////////////////////////////////////
//
// init - initializes the RandManager
//
// terms    - the number of terms each problem can have
// probs    - the number of problems to be presented
// mins     - a list of minimum values for each term in the problem
// maxs     - a list of maximum values for each term in the problem
//
void RandManager::init(int terms, int probs, QList<int> &mins, QList<int> &maxs)
{
    dimension = terms;
    problems = probs;

    // Create an array of QList<int> for each term (dimension) in the
    // problem to be presented. This should be the maximum number of
    // terms that can be expected for a given problem type. Some problems
    // can only have two terms, while others can have two or more.
    // These lists will be used to store values that have already been
    // presented in problems.
    //
    vVal.clear();
    vVal.resize(dimension);

    // If the caller only sent one min and one max value, then the
    // min/max for each dimension(term) is the same.
    //
    minList.clear();
    maxList.clear();

    for(int i = 0; i < dimension; ++i) {
        if(mins.size() == 1)
            minList << mins[0];
        else
            minList << mins[i];

        if(maxs.size() == 1)
            maxList << maxs[0];
        else
            maxList << maxs[i];
    }

    // Initialize the random number generator.
    //
    int seed = (int)time(0);
    pRand = new CRandomMersenne(seed);

    smallcount = 0;
}

//////////////////////////////////////////////////////////////////////////////
//
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
//              problem set that can have up to three terms, but this
//              particular problem in that set is only presenting two
//              terms, e.g. 2+5 instead of 2+5+7.
//
// Returns true or false, based upon whether the terms are considered stale
//
bool RandManager::isStale(QList<int> vals, int terms)
{
    bool stale = false;

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

    if((stale = checkInverseTerms(vals)))
        return true;

    for(int i = 0; i < vVal.size(); ++i)
        vVal[i] << (i < terms ? vals[i] : INT_MIN);

    return false;
}

//////////////////////////////////////////////////////////////////////////////
//
// getTerms - obtain a unique set of terms to present in the problem
//
// terms - reference to a QList that will receive the terms
//
// Returns a reference to the QList that has the terms.
//
QList<int>& RandManager::getTerms(QList<int>& vals)
{
    getTerms(vals, dimension);
    return vals;
}

//////////////////////////////////////////////////////////////////////////////
//
// getTerms - obtain a unique set of terms to present in the problem
//
// terms - reference to a QList that will receive the terms
//
// Returns a reference to the QList that has the terms.
//
QList<int>& RandManager::getTerms(QList<int>& vals, int terms)
{
    // This loop obtains terms that are not stale
    //
    do {
        int j = 0;      // count the terms
        vals.clear();   // clear the vals list

        // This loop obtains the values for the number of terms
        //
        do {
            int k = pRand->IRandomX(minList[j], maxList[j]);
            bool small = (abs(k) < SMALLEST_NUM);
            smallcount += small ? 1 : 0;
            if(small && (smallcount > MAX_SMALLNUM))
                continue;
            vals << k;
            j++;
        } while(j < terms);
    } while(isStale(vals, terms));

    return vals;
}

/**********************************
 ** PRIVATE ROUTINES
 *********************************/

//////////////////////////////////////////////////////////////////////////////
//
// getValues  - get a "column" of values from the vVal vector of QLists of
//              values
//
// index    - index into the vVal vector of QLists of values
// terms    - the number of terms to read out of the list
// column   - reference to the "column" of terms read out of the list
//
// returns a reference to a QList containing the column of values read out of
// the vVal vector of QLists
//
QList<int>& RandManager::getValues(int index, int terms, QList<int> &column)
{
    column.clear();
    for(int i = 0; i < terms; ++i)
        column << vVal[i][index];
    return column;
}

//////////////////////////////////////////////////////////////////////////////
//
// checkInverseTerms - see if the terms contains all the same terms presented
//                     before, regardless of the order they are in.
//
// vals - reference to a QList of values to be checked.
//
// Checking is done by indexing through the vVals vector of QLists, extracting
// a "column" of values that have been presented previously for each index.
// The extracted column is subjected to a aSort, as is a copy of the vals
// QList reference that was passed as a parameter. If they are identical, then
// the terms are considered to be "stale".
//
// Returns "true" if the terms are stale, false otherwise.
//
bool RandManager::checkInverseTerms(QList<int>& vals)
{
    int terms = vals.size();    // The number of terms in this problem
    int sofar = vVal[0].size(); // The number of unique terms so far
    QList<int> aCol;            // A column of values
    QList<int> bCol = vals;     // A copy of the vals passed ins

    if(sofar == 0)
        return false;

    qSort(bCol.begin(), bCol.end());

    for(int i = 0; i < sofar; ++i) {
        aCol = getValues(i, terms, aCol);
        qSort(aCol.begin(), aCol.end());

        if(aCol == bCol)
            return true;
    }
    return false;
}
