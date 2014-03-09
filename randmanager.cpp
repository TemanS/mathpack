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
RandManager(int terms, int probs, QVector<int> &mins, QVector<int> &maxs)
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
void
RandManager::init(int terms, int probs, QVector<int> &mins, QVector<int> &maxs)
{
    m_dimension = terms;
    m_problems = probs;

    // Create an array of QVector<int> for each term (dimension) in the
    // problem to be presented. This should be the maximum number of
    // terms that can be expected for a given problem type. Some problems
    // can only have two terms, while others can have two or more.
    // These lists will be used to store values that have already been
    // presented in problems.
    //
    m_vals.clear();
    m_vals.resize(m_dimension);

    // If the caller only sent one min and one max value, then the
    // min/max for each dimension(term) is the same.
    //
    m_minList.clear();
    m_maxList.clear();

    for(int i = 0; i < m_dimension; ++i) {
        if(mins.size() == 1)
            m_minList << mins[0];
        else
            m_minList << mins[i];

        if(maxs.size() == 1)
            m_maxList << maxs[0];
        else
            m_maxList << maxs[i];
    }

    // Initialize the random number generator.
    //
    int seed = (int)time(0);
    m_rnd.RandomInit(seed);

    m_smallcount = 0;
}

//////////////////////////////////////////////////////////////////////////////
//
// bool RandManager::isStale - determines whether the values passed are stale
//
// QVector vals - a list of values, which can be thought of as a "column"
//              These are the values that are being proposed for presentation
//              to the user. This function will determine if these values
//              are "stale" based on whether they are present anywhere else
//              in the array (QVector m_vals) and depending on the "staleness"
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
bool RandManager::isStale(QVector<int> vals, int terms)
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

    for(int i = 0; i < m_vals.size(); ++i)
        m_vals[i] << (i < terms ? vals[i] : INT_MIN);

    return false;
}

//////////////////////////////////////////////////////////////////////////////
//
// getTerms - obtain a unique set of terms to present in the problem
//
// terms - reference to a QVector that will receive the terms
//
// Returns a reference to the QVector that has the terms.
//
QVector<int>& RandManager::getTerms(QVector<int>& vals)
{
    getTerms(vals, m_dimension);
    return vals;
}

//////////////////////////////////////////////////////////////////////////////
//
// getTerms - obtain a unique set of terms to present in the problem
//
// terms - reference to a QVector that will receive the terms
//
// Returns a reference to the QVector that has the terms.
//
QVector<int>& RandManager::getTerms(QVector<int>& vals, int terms)
{
    // This loop obtains terms that are not stale
    //
    do {
        int j = 0;      // count the terms
        vals.clear();   // clear the vals list

        // This loop obtains the values for the number of terms
        //
        do {
            int k = m_rnd.IRandom(m_minList[j], m_maxList[j]);
            bool small = (abs(k) < SMALLEST_NUM);
            m_smallcount += small ? 1 : 0;
            if(small && (m_smallcount > MAX_SMALLNUM))
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
// getValues  - get a "column" of values from the m_vals vector of QVectors of
//              values
//
// index    - index into the m_vals vector of QVectors of values
// terms    - the number of terms to read out of the list
// column   - reference to the "column" of terms read out of the list
//
// returns a reference to a QVector containing the column of values read out of
// the m_vals vector of QVectors
//
QVector<int>&
RandManager::getValues(int index, int terms, QVector<int> &column)
{
    column.clear();
    for(int i = 0; i < terms; ++i)
        column << m_vals[i][index];
    return column;
}

//////////////////////////////////////////////////////////////////////////////
//
// checkInverseTerms - see if the terms contains all the same terms presented
//                     before, regardless of the order they are in.
//
// vals - reference to a QVector of values to be checked.
//
// Checking is done by indexing through the m_valss vector of QVectors, extracting
// a "column" of values that have been presented previously for each index.
// The extracted column is subjected to a aSort, as is a copy of the vals
// QVector reference that was passed as a parameter. If they are identical, then
// the terms are considered to be "stale".
//
// Returns "true" if the terms are stale, false otherwise.
//
bool RandManager::checkInverseTerms(QVector<int>& vals)
{
    int terms = vals.size();    // The number of terms in this problem
    int sofar = m_vals[0].size(); // The number of unique terms so far
    QVector<int> aCol;            // A column of values
    QVector<int> bCol = vals;     // A copy of the vals passed ins

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
