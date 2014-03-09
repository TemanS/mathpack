/******************************************************************************
** NOTE WELL:
**      Many of the "get" functions return static data, so this class should
**      not be considered reentrant. Use spinlocks for multithreaded apps.
******************************************************************************/

#ifndef TESTPARM_H
#define TESTPARM_H

#include <QtCore>
#include <msg.h>
#include <randmanager.h>

#define LVLCHK(score) ((score > 75) ? msg::msg_notify : msg::msg_alert)

namespace tp
{
    enum oper_t {op_add, op_sub, op_mul, op_div, op_mod, op_sqr, op_sqrt };
    enum answer_t {ans_int, ans_float, ans_string};
    //enum opLimits_t {maxLeft, maxRight, minTerm, maxTerm, opLimits_end};
}

QT_BEGIN_NAMESPACE
class QString;
class QElapsedTimer;
QT_END_NAMESPACE

class Msg;

// class TestParm
//
// This class contains the test parameters for a test type.
//
// count        - number of problems to be presented
// timeout      - the max length of time allowed for each problem
// level        - the level of difficulty for the problem set
// isEnabled    - whether the test type is enabled to run
// inputMask    - input mask for input from the user
// testName     - name of the test
// numberCorrect- number of correct answers
// pass         - the current test number in the set
// isOnTime     - whether the user beat the timeout
// percentScore - user score expressed as a percent of corect answers
// userAnswer   - the answer given by the user for a given problem
// correctAnswer- the correct answer for the problem
// letterScore  - user score expressed as a letter (A - F)
// userTime     - time it took the user to answer the current problem
//
// One TestParm class will be instantiated for each test type, e.g.
// add, sub, mul, div, etc.
//
class TestParm
{
public:
    TestParm();
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

    // Test parameters can be presented in a number of different ways.
    // Originally, I thought that a simple vector containing all the
    // parameter limits could be used. However, as problems become more
    // complicated, they can have more than just two operands and each
    // operand can have its own max/min limits.
    //
    QVector<int> opLimits;      // Limt values for operands

    // These member variables are for problems that can have more than
    // two operands, and where each operand can have its own maximum and
    // minimum limits.
    //
    QVector<int> maxops;        // Problem can have different op limits for
    QVector<int> minops;        // : each term in the problem.

    // Each problem type can also have maximum and minimum number of
    // operands. For example, some problems may have a minimum of 3
    // operands (quadratics) and others may have maximum of 4 or more
    // terms.
    //
    int maxterms;
    int minterms;

    RandManager randman;        // An instance of random number manager

    QString inputMask;          // input mask for this problem type
    //
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

// class TestParmManager
//
// This class manages the test parameters for each of the test types in
// a given math drill application.
//
class TestParmManager
{
public:
    TestParmManager();  // must not use default constructor!
    TestParmManager(int testCount);
    ~TestParmManager();

    void initTest(int idx, int cnt, int tmo, int lvl, bool ena=true,
                  const QString& mask=QString("0000"),
                  const QString& tnm=QString(""));
    void startTest() {m_testParmList[m_index]->pass = 0;}
    void updateTest();
    void stopTest();
    void writeEndOfTest(Msg* pMsg);
    void writeFinals(Msg* pMsg);

    int  getNextTestIndex();
    int  getCurrentTestIndex() {return m_index;}

    void startTimer() {m_timer.start();}
    int  getElapsedTime();
    int  getUserTime() {return m_testParmList[m_index]->userTime;}

    void setRunning(bool state) {m_running = state;}
    bool isRunning(){return m_running;}

    void setMaxopsLoaded(bool state) {m_maxopsLoaded = state;}
    bool isMaxopsLoaded() {return m_maxopsLoaded;}

    bool isFirstRun() {return m_firstRun;}
    int  getIndex() {return m_index;}
    int  getTotalCount() {return m_totalCount;}
    void initTotalCount(int val=0) {m_totalCount = val;}
    void initTotalCorrect(int val=0) {m_totalCorrect = val;}

    QString getTestScore();
    QString getFinalScore();

    // Stuff that operates on the TestParm class members.
    //
    QList<TestParm*> getTestParmList(){return m_testParmList;}
    TestParm* getTestParm(){return m_testParmList[m_index];}

    QVector<int> getOperandLimits(int index, int count=1);
    QVector<int>& getMaxops(int index, int count);
    QVector<int>& getMinops(int index, int count);
    void initOperandLimits(QVector<int>& opLims, int index);

    void setProblem(const QString& problem)
        {m_testParmList[m_index]->problem = problem;}
    QString getProblem() {return m_testParmList[m_index]->problem;}

    int  getPass() {return m_testParmList[m_index]->pass;}
    void setPass (int pass) { m_testParmList[m_index]->pass = pass;}
    void bumpPass() {m_testParmList[m_index]->pass++;}
    void initPass() {m_testParmList[m_index]->pass = 0;}
    void updatePass();

    bool atTestEnd() {return (m_testParmList[m_index]->pass >=
                              m_testParmList[m_index]->count);}

    bool isEnabled() {return m_testParmList[m_index]->isEnabled;}
    bool isCorrect() {return m_testParmList[m_index]->isCorrect;}

    QString getUserAnswer() {return m_testParmList[m_index]->userAnswer;}
    void setUserAnswer(QString answer)
        {m_testParmList[m_index]->userAnswer = answer;}

    void bumpCorrect() {m_testParmList[m_index]->numberCorrect++;
                        m_testParmList[m_index]->isCorrect = true;
                        m_totalCorrect++;}

    QString getCorrectAnswer() {return m_testParmList[m_index]->correctAnswer;}
    void setCorrectAnswer(QString answer)
        {m_testParmList[m_index]->correctAnswer = answer;}

    int  getCurrentLevel() {return m_testParmList[m_index]->level;}

private:
    bool m_testParmListInited;  // True after TestParmLists are created
    bool m_firstRun;            // True only for first test, false thereafter
    bool m_running;             // Are tests running?
    int m_index;                // Index into the TestParmList
    int m_maxopsLoaded;         // Did we get the max ops ?
    int m_totalCount;
    int m_totalCorrect;
    int m_finalPercentScore;
    QElapsedTimer m_timer;        // an elapsed timer
    QString m_finalLetterScore;
    QList<TestParm*> m_testParmList;
    QVector<int> m_operandLimits;

    void writeEndOfTestCommon();
    void writeFinalsCommon();
    void clearTestParmList();       // Delete all TestParm ptrs and clear list
};

#endif // TESTPARM_H
