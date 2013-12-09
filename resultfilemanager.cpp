#include <QtCore>
#include <QStringBuilder>
#include <QIODevice>

#include <qpfile.h>
#include "testparm.h"
#include "resultfilemanager.h"

static QString deco =
"***************************************************************************\n";

static QString decoLine =
"---------------------------------------------------------------------------\n";

QString logTitle = "%1,  %2 Problems,  Timeout: %3,  Level %4\n";

static QString header = decoLine %
"                                   Your    Time \n"
"              Problem     Answer  Answer  Seconds  Status\n"
                        % decoLine;

ResultFileManager::ResultFileManager()
{
    m_haveFile = false;
}

void ResultFileManager::init(QFile *pFile)
{
    if(m_haveFile)
        delete m_pFile;

    m_pFile = pFile;
    m_haveFile = true;
}

bool ResultFileManager::startFile(QString &userName, QString &timeStamp)
{
    if((m_haveFile == false) || (m_pFile->isOpen() == false))
        return false;

    QTextStream stream(m_pFile);
    QString nameAndDate =
            "**   User Name: " % userName % "  Date & Time: " % timeStamp;
    QString spaces;
    int size = deco.size() - nameAndDate.size() - 3;
    spaces.resize(size);
    spaces.fill(' ');
    spaces.append("**\n");

    stream << deco << nameAndDate << spaces << deco << endl;
    stream.flush();
    return true;
}

bool ResultFileManager::startTest(TestParmManager *ptm)
{
    if((m_haveFile == false) || (m_pFile->isOpen() == false))
        return false;

    TestParm* testParm = ptm->getTestParm();
    QTextStream stream(m_pFile);

    QString tmo = (testParm->timeout == -1)
            ? QString("none")
            : QString("%1 seconds").arg(testParm->timeout);

    QString title = QString("%1,  %2 Problems,  Timeout: %3,  Level %4\n")
                .arg(testParm->testName)
                .arg(testParm->count)
                .arg(tmo)
                .arg(testParm->level+1);

    stream << title << header;
    stream.flush();
    return true;
}

bool ResultFileManager::updateTest(TestParmManager *ptm)
{
    if((m_haveFile == false) || (m_pFile->isOpen() == false))
        return false;

    TestParm* testParm = ptm->getTestParm();
    QTextStream stream(m_pFile);

    QString logEntry = QString("%1. %2  %3  %4  ")
            .arg(testParm->pass+1, 3)
            .arg(testParm->problem, 12)
            .arg(testParm->correctAnswer, 10)
            .arg(testParm->userAnswer, 10);

    if(testParm->timeout > 0)
        logEntry += QString("%1  ").arg(testParm->userTime);

    logEntry += testParm->isCorrect ? "Correct" : "Wrong";

    if(testParm->timeout > 0)
        logEntry += testParm->isOnTime ? "" : "  Timed out!";

    logEntry += "\n";

    stream << logEntry;

    return true;
}

bool ResultFileManager::writeEndOfTest(TestParmManager *ptm)
{
    if((m_haveFile == false) || (m_pFile->isOpen() == false))
        return false;

    QTextStream stream(m_pFile);

    QString logEntry = QString("Your score %1%\n\n")
            .arg(ptm->getTestScore());

    stream << logEntry;
    stream.flush();
    return true;
}

bool ResultFileManager::writeFinals(TestParmManager *testParmManager)
{
    if((m_haveFile == false) || (m_pFile->isOpen() == false))
        return false;

    QTextStream stream(m_pFile);

    QString logEntry = decoLine % QString("Your Final Score: %1\n\n")
            .arg(testParmManager->getFinalScore());

    stream << logEntry;
    stream.flush();
    return true;
}

bool ResultFileManager::isOpen()
{
    if(m_haveFile)
        return m_pFile->isOpen();
    else
        return false;
}
