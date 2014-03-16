#include <QtCore>
#include <QElapsedTimer>

#include <mathpack_global.h>
#include <mpscore.h>
#include <testparm.h>

using namespace tp;

/***********************
** TestParm Routines
***********************/

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

