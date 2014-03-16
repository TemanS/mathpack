#ifndef TESTPARMMANAGER_H
#define TESTPARMMANAGER_H

#include <QtCore>
#include <testparm.h>

#define LVLCHK(score) ((score > 75) ? msg::msg_notify : msg::msg_alert)

//********************************************************************
//
// class TestParmManager
//
// This class manages the test parameters for each of the test types in
// a given math drill application. It will contain a TestParm class for
// each level of each test. Provision is made for tests that may have
// only one level.
//
// Tests consist of:
//
//  test    - the test type
//  level   - the level of difficulty (see class TestParm above)
//  pass    - the sequence number of the test type
//  terms   - or operands to be presented in a problem
//  problem - a problem presented to the user at a pass of the test
//
class TestParmManager
{
public:
    TestParmManager();  // must not use default constructor!
    TestParmManager(int testCount);
    ~TestParmManager();

    void initInstance(int testCount);
    void init();
    void initTest(int idx, int cnt, int tmo, int lvl, bool ena=true,
                  const QString& mask=QString("0000"),
                  const QString& tnm=QString(""));

    // Test State Machine
    //
    void startTest() {m_testParmList[m_index]->pass = 0;}
    void updateTest();
    void stopTest();
    int  getNextTestIndex();
    int  getCurrentTestIndex() {return m_index;}
    void startTimer() {m_timer.start();}
    int  getElapsedTime();
    int  getUserTime() {return m_testParmList[m_index]->userTime;}
    void setRunning(bool state) {m_running = state;}
    bool isRunning(){return m_running;}
    bool isMaxopsLoaded() {return m_maxopsLoaded;}
    bool isEnabled() {return m_testParmList[m_index]->isEnabled;}
    bool isFirstRun() {return m_firstRun;}
    int  getIndex() {return m_index;}
    int  getTotalCount() {return m_totalCount;}
    void initTotalCount(int val=0) {m_totalCount = val;}
    void initTotalCorrect(int val=0) {m_totalCorrect = val;}
    void setMaxopsLoaded(bool state) {m_maxopsLoaded = state;}
    void writeEndOfTest(Msg* pMsg);
    void writeFinals(Msg* pMsg);

    // Stuff that operates on the TestParm class members.
    //
    QList<TestParm*>& getTestParmList(){return m_testParmList;}
    TestParm* getTestParm(){return m_testParmList[m_index];}
    RandManager& getRandman();

    // Access to the TestParm class
    //
    void initOperandLimits(QVector<int>& opLims, int index);
    QVector<int>& getOperandLimits(int index, int count=1);
    QVector<int>& getMaxVals();
    QVector<int>& getMinVals();
    int getMaxTerms();
    int getMinTerms();
    int getCount();

    // Problem presentation
    //
    void setProblem(const QString& problem)
        {m_testParmList[m_index]->problem = problem;}
    QString& getProblem() {return m_testParmList[m_index]->problem;}

    // Test pass sequencing
    //
    void initPass() {m_testParmList[m_index]->pass = 0;}
    int  getPass() {return m_testParmList[m_index]->pass;}
    void setPass (int pass) { m_testParmList[m_index]->pass = pass;}
    void bumpPass() {m_testParmList[m_index]->pass++;}
    void updatePass();

    // Test status
    //
    bool atTestEnd() {return (m_testParmList[m_index]->pass >=
                              m_testParmList[m_index]->count);}
    bool isCorrect() {return m_testParmList[m_index]->isCorrect;}
    QString& getTestScore();
    QString& getFinalScore();
    QString& getUserAnswer() {return m_testParmList[m_index]->userAnswer;}
    QString& getCorrectAnswer() {return m_testParmList[m_index]->correctAnswer;}
    void setUserAnswer(QString answer)
        {m_testParmList[m_index]->userAnswer = answer;}
    void setCorrectAnswer(QString answer)
        {m_testParmList[m_index]->correctAnswer = answer;}
    void bumpCorrect() {m_testParmList[m_index]->numberCorrect++;
                        m_testParmList[m_index]->isCorrect = true;
                        m_totalCorrect++;}
    int  getCurrentLevel() {return m_testParmList[m_index]->level;}

private:

    // A TestParm class is created for each level of each test type. If
    // there are three distinct test types, each having four levels of
    // difficulty, then there there will be twelve instances of class
    // TestParm, whose pointers will be stored in this list.
    //
    QList<TestParm*> m_testParmList;    // A list of pointers to the TestParm

    bool m_testParmListInited;  // True after TestParmLists are created
    bool m_firstRun;            // True only for first test, false thereafter
    bool m_running;             // Are tests running?
    int m_index;                // Test Type
    int m_level;                // Level of difficulty
    int m_maxopsLoaded;         // Did we get the max ops ?
    int m_totalCount;           // Counts each problem presented
    int m_totalCorrect;         // Counts the correct answers
    int m_finalPercentScore;
    QElapsedTimer m_timer;        // an elapsed timer
    QString m_finalLetterScore;
    QVector<int> m_operandLimits;

    void writeEndOfTestCommon();
    void writeFinalsCommon();
    void clearTestParmList();       // Delete all TestParm ptrs and clear list
};


#endif // TESTPARMMANAGER_H
