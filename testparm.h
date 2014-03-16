/******************************************************************************
** NOTE WELL:
**      Many of the "get" functions return static data, so this class should
**      not be considered reentrant. Use spinlocks for multithreaded apps.
******************************************************************************/

#ifndef TESTPARM_H
#define TESTPARM_H

#include <randmanager.h>

namespace tp
{
    enum oper_t {op_add, op_sub, op_mul, op_div, op_mod, op_sqr, op_sqrt };
    enum answer_t {ans_int, ans_float, ans_string};
    //enum opLimits_t {maxLeft, maxRight, minTerm, maxTerm, opLimits_end};
}

//********************************************************************
//
// class TestParm
//
// This class contains the test parameters for a test type.
// One of these should be instantiated for every test type and
// every level for that test type, because each test type and
// each level for a given test type can have its own parameters.
//
// count        - number of problems to be presented
// timeout      - the max length of time allowed for each problem
// level        - the level of difficulty for the problem set
// isEnabled    - whether the test type is enabled to run
// inputMask    - input mask for input from the user
// testName     - name of the test
// pass         - the current test number in the set
// isOnTime     - whether the user beat the timeout
// userAnswer   - the answer given by the user for a given problem
// numberCorrect- number of correct answers
// correctAnswer- the correct answer for the problem
// percentScore - user score expressed as a percent of corect answers
// letterScore  - user score expressed as a letter (A - F)
// userTime     - time it took the user to answer the current problem
// maxterms     - maximum number of terms for this problem type
// minterms     - minumum number of terms for this problem type
// maxvals      - an array of max values for each term
// minvals      - an array of min values for each term
//
// One TestParm class will be instantiated for each test type, e.g.
// add, sub, mul, div, etc.
//
class TestParm
{
public:
    TestParm(){init();}     // init to all defaults.
    TestParm(int cnt, int tmo, int lvl, bool ena = false,
             const QString& mask="0000",
             const QString& tnm=QString(""));

    void init(int cnt = 0, int tmo = 0, int lvl = 0, bool ena = false,
              const QString& mask="0000",
              const QString& tnm=QString(""));

    // These members should be initialized before the start of a test run.
    //
    QString testName;           // Name of the Test
    bool isEnabled;             // Is Tet scheduled to run?
    int level;                  // level of difficulty
    int timeout;                // time allowed to find the answer
    int count;                  // how many problems for this test type?

    // Each problem type and level has a maximum and minimum number of
    // operands. For example, some problems may have a minimum of 3
    // operands (quadratics) and others may have maximum of 4 or more
    // terms. The number of terms can also differ with levels of
    // difficulty.
    //
    QVector<int> maxterms;
    QVector<int> minterms;

    // Problems can have two or more operands, and each operand can have
    // its own maximum and minimum limits. These max/min limits for each
    // operand can differ at each level of difficulty. So the following
    // QVectors of QVectors are containers for the max and min values of
    // each operand for this problem at each level of difficlulty.
    //
    QVector<QVector<int> > maxvals;
    QVector<QVector<int> > minvals;

    RandManager randman;        // An instance of random number manager
    QString inputMask;          // input mask for this problem type

    //  The following are initialized or updated for each pass in the test.
    //
    int isOnTime;               // user took too long to answer
    int pass;                   // which pass this is
    bool isCorrect;             // user entered the correct answer
    int numberCorrect;          // number of correct answers for this test
    int percentScore;           // user's score as a percentage
    QString problem;            // problem string presented to user
    QString correctAnswer;      // the correct answer expected
    QString userAnswer;         // answer given by the user
    int userTime;               // how long it took the user to answer
    QString letterScore;        // user's score as a letter grade

private:

};


#endif // TESTPARM_H
