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
    RandManager(int terms, int probs, QVector<int>& mins, QVector<int>& maxs);
    void init(int terms, int probs, QVector<int>& mins, QVector<int>& maxs);
    bool isStale(QVector<int>& vals, int terms);
    QVector<int>& getValues(QVector<int>& vals);
    QVector<int>& getValues(QVector<int>& vals, int terms);
    int& getValues(int& term);
    void setSames(int val) {m_sames = val;}
    void setSmall(int val) {m_small = val;}
    void setNoZero(bool z) {m_nozero = z;}

private:
    int m_dimension;          // The number of terms to track
    int m_problems;           // The number of problems being presented
    int m_nozero;             // Allow NO zeros
    int m_sames;              // Max allowable same numbers
    int m_small;              // Max allowable smallnumbers 0, 1, 2
    int m_smallest;           // Smallest allowable number
    int m_smallcount;         // Running count of small numbers
    QVector<int> m_minList;   // List of min values of terms for each dimension
    QVector<int> m_maxList;   // List of max values of terms for each dimension
    CRandomMersenne m_rnd;    // Instance of Random Generator class

    // QVector vVal is a container for the QVectors of values for each term.
    // Consider a problem set that can require up to three terms in the
    // problem.
    //
    // vVal[0] is a QVector of values that have been presented for the 1st term
    // vVal[1] is a QVector of values that have been presented for the 2nd term
    // vVal[2] is a QVector of values that have been presented for the 3rd term
    //
    QVector<QVector<int> > m_vals; // Array of Lists of values that have already
                                   // been presented.

    QVector<int>& getColumn(int index, int terms, QVector<int>& column);
    bool checkInverseTerms(QVector<int>& vals);
    bool checkSames(QVector<int>& vals);
};

#endif // RANDMANAGER_H
