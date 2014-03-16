#include <msg.h>
#include <mpscore.h>
#include <testparmmanager.h>

/**********************************
** TestParmManager Initialization
***********************************/

//*******************************************************************
// TestParmManager constructor
//
// Simple constructor, no parameters.
//
TestParmManager::TestParmManager()
{
    m_testParmListInited = false;
    init();
}

//*******************************************************************
// TestParmManager constructor overload
//
// testCount - number of different test types
//
TestParmManager::TestParmManager(int testCount)
{
    initInstance(testCount);
}

//*******************************************************************
// ~TestParmManager destructor
//
TestParmManager::~TestParmManager()
{
    for(int i = 0; i < m_testParmList.size(); ++i)
        delete m_testParmList[i];

    m_testParmList.clear();
}

//*******************************************************************
// initInstance(testCount)
//
// testCount - total number of tests, including test levels
//
// Creates the QList of TestParm class instances
//
void TestParmManager::initInstance(int testCount)
{
    m_testParmList.clear();
    m_operandLimits.clear();

    for(int i = 0; i < testCount; ++i)
        m_testParmList << new TestParm;

    m_testParmListInited = true;
    init();
}

//*******************************************************************
// init
//
// These private members are always inited the same way.
//
void TestParmManager::init()
{
    m_firstRun = true;
    m_running = false;
    m_index = 0;
    m_totalCount = 0;
    m_totalCorrect = 0;
    m_maxopsLoaded = false;
}


//*******************************************************************
void TestParmManager::initTest(int idx, int cnt, int tmo, int lvl, bool ena,
                               const QString& mask,
                               const QString& tnm)
{
    m_testParmList[idx]->init(cnt, tmo, lvl, ena, mask, tnm);
    if(ena)
        m_totalCount += cnt;
    m_running = true;
}

/*******************************************
** Test State Machine
*******************************************/

//*******************************************************************
void TestParmManager::stopTest()
{
    m_firstRun = false;
    m_running = false;
    m_index = 0;
}

//*******************************************************************
int TestParmManager::getNextTestIndex()
{
    TestParm* pt = m_testParmList[m_index];

    // If the currently indexed test was not enabled, or if we've done all the
    // problems for the currently indexed test, then use an empty while loop
    // to find the next enabled test.
    //
    if(! pt->isEnabled || (pt->pass >= pt->count)) {
        int idx = m_index;

        while((++idx < m_testParmList.size())
        && ! (pt = m_testParmList[idx])->isEnabled)
            ;

        if(idx < m_totalCount) {
            m_index = idx;
            pt->pass = 0;
        }

        return idx;
    }

    return m_index;
}

//*******************************************************************
// Update the passes of the same test.
//
void TestParmManager::updatePass()
{
    TestParm* pt = m_testParmList[m_index];
    pt->pass++;
    pt->isCorrect = false;
    pt->isOnTime = false;
}

//*******************************************************************
// Prepare the Parm Manager and germaine variables for the next test.
//
void TestParmManager::updateTest()
{
    TestParm *pt = m_testParmList[m_index];
    pt->pass = 0;
}

//*******************************************************************
void TestParmManager::writeEndOfTest(Msg *pMsg)
{
    TestParm *pt = m_testParmList[m_index];
    MpScore score(pt->count, pt->numberCorrect);
    pt->percentScore = score.getPercentGrade();
    pt->letterScore =  score.getLetterGrade();

    QString text = QString("Your Score: %1%  %2")
            .arg(pt->percentScore).arg(pt->letterScore);
    pMsg->sendMessage(text, LVLCHK(pt->percentScore));
}

/*******************************************
** Test Scoring Functions
*******************************************/

//*******************************************************************
void TestParmManager::writeFinals(Msg *pMsg)
{
    MpScore score(m_totalCount, m_totalCorrect);
    m_finalPercentScore = score.getPercentGrade();
    m_finalLetterScore = score.getLetterGrade();

    QString text = QString("Your Final Score: %1%  %2")
            .arg(m_finalPercentScore).arg(m_finalLetterScore);
    pMsg->sendMessage(text, LVLCHK(m_finalPercentScore));
}

QString& TestParmManager::getTestScore()
{
    TestParm* pt = m_testParmList[m_index];
    static QString score = QString("%1%  %2")
            .arg(pt->percentScore).arg(pt->letterScore);
    return score;
}

QString& TestParmManager::getFinalScore()
{
    static QString score = QString("%1%  %2")
            .arg(m_finalPercentScore).arg(m_finalLetterScore);
    return score;
}

int TestParmManager::getElapsedTime()
{
    TestParm* pt = m_testParmList[m_index];
    pt->userTime = m_timer.elapsed() / 1000;
    return pt->userTime;
}

/*******************************************
** Access to the TestParm class
*******************************************/

//*******************************************************************
QVector<int>& TestParmManager::getMaxVals()
{
    TestParm* pt = m_testParmList[m_index];
    return pt->maxvals[pt->level];
}

//*******************************************************************
QVector<int>& TestParmManager::getMinVals()
{
    TestParm* pt = m_testParmList[m_index];
    return pt->minvals[pt->level];
}

//*******************************************************************
int TestParmManager::getMaxTerms()
{
    TestParm* pt = m_testParmList[m_index];
    return pt->maxterms[pt->level];
}

//*******************************************************************
int TestParmManager::getMinTerms()
{
    TestParm* pt = m_testParmList[m_index];
    return pt->minterms[pt->level];
}

//*******************************************************************
int TestParmManager::getCount()
{
    TestParm* pt = m_testParmList[m_index];
    return pt->count;
}

//*******************************************************************
RandManager& TestParmManager::getRandman()
{
    TestParm* pt = m_testParmList[m_index];
    pt->randman.init(getMaxTerms(), getCount(), getMinVals(), getMaxVals());
    return pt->randman;
}
