#include <QLabel>
#include <QList>
#include <QString>
#include <QElapsedTimer>
#include "mathpack_global.h"
#include "mpscore.h"
#include "testparm.h"

using namespace tp;

TestParm::TestParm()
{
    opLimits.clear();
    init();
}

TestParm::TestParm(int cnt, int tmo, int lvl, bool ena,
                   const QString& mask,
                   const QString& tnm)
{

    init(cnt, tmo, lvl, ena, mask, tnm);
}

void TestParm::init(
        int cnt, int tmo, int lvl, bool ena,
        const QString& mask,
        const QString& tnm)
{
    count = cnt;
    timeout = tmo;
    level = lvl;
    isEnabled = ena;
    inputMask = mask;
    testName = tnm;
    numberCorrect = 0;
    pass = 0;
    isOnTime = true;
    percentScore = 0;
    userAnswer.clear();
    letterScore.clear();
}

TestParmManager::TestParmManager()
{
    m_firstRun = true;
    m_running = false;
    m_index = 0;
    m_totalCount = 0;
    m_totalCorrect = 0;
    m_maxopsLoaded = false;
    m_testParmListInited = false;
}

TestParmManager::TestParmManager(int testCount)
{
    m_testParmList.clear();
    m_operandLimits.clear();

    for(int i = 0; i < testCount; ++i)
        m_testParmList << new TestParm;

    m_firstRun = true;
    m_running = false;
    m_index = 0;
    m_totalCount = 0;
    m_totalCorrect = 0;
    m_maxopsLoaded = false;
    m_testParmListInited = true;
}

TestParmManager::~TestParmManager()
{
    for(int i = 0; i < m_testParmList.size(); ++i)
        delete m_testParmList[i];

    m_testParmList.clear();
}

void TestParmManager::initTest(int idx, int cnt, int tmo, int lvl, bool ena,
                               const QString& mask,
                               const QString& tnm)
{
    m_testParmList[idx]->init(cnt, tmo, lvl, ena, mask, tnm);
    if(ena)
        m_totalCount += cnt;
    m_running = true;
}

void TestParmManager::stopTest()
{
    m_firstRun = false;
    m_running = false;
    m_index = 0;
}

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

// Update the passes of the same test.
//
void TestParmManager::updatePass()
{
    TestParm* pt = m_testParmList[m_index];
    pt->pass++;
    pt->isCorrect = false;
    pt->isOnTime = false;
}

// Prepare the Parm Manager and germaine variables for the next test.
//
void TestParmManager::updateTest()
{
    TestParm *pt = m_testParmList[m_index];
    pt->pass = 0;
}

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

void TestParmManager::writeFinals(Msg *pMsg)
{
    MpScore score(m_totalCount, m_totalCorrect);
    m_finalPercentScore = score.getPercentGrade();
    m_finalLetterScore = score.getLetterGrade();

    QString text = QString("Your Final Score: %1%  %2")
            .arg(m_finalPercentScore).arg(m_finalLetterScore);
    pMsg->sendMessage(text, LVLCHK(m_finalPercentScore));
}

void TestParmManager::initOperandLimits(QVector<int>& opLims, int index)
{
    TestParm* pt = m_testParmList[index];
    pt->opLimits << opLims;
}

QVector<int> TestParmManager::getOperandLimits(int index, int count)
{
    TestParm* pt = m_testParmList[m_index];

    m_operandLimits.clear();
    for(int i = 0; i < count; ++i) {
        m_operandLimits << pt->opLimits[index+i];
    }
    return m_operandLimits;
}

QString TestParmManager::getTestScore()
{
    TestParm* pt = m_testParmList[m_index];
    QString score = QString("%1%  %2")
            .arg(pt->percentScore).arg(pt->letterScore);
    return score;
}

QString TestParmManager::getFinalScore()
{
    QString score = QString("%1%  %2")
            .arg(m_finalPercentScore).arg(m_finalLetterScore);
    return score;
}

int TestParmManager::getElapsedTime()
{
    TestParm* pt = m_testParmList[m_index];
    pt->userTime = m_timer.elapsed() / 1000;
    return pt->userTime;
}
