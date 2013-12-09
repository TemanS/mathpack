#ifndef RESULTFILE_H
#define RESULTFILE_H

QT_BEGIN_NAMESPACE
class QString;
class QFile;
QT_END_NAMESPACE

class TestParm;
class TestParmManager;

class ResultFileManager
{
public:
    ResultFileManager();
    void init(QFile* file);
    bool startFile(QString& userName, QString& timeStamp);
    bool startTest(TestParmManager* ptm);
    bool updateTest(TestParmManager* ptm);
    bool writeEndOfTest(TestParmManager* ptm);
    bool writeFinals(TestParmManager* testParmManager);
    bool isOpen();
    bool isValid() {return m_haveFile;}
    QFile* getFile() {return m_pFile;}

private:
    QFile* m_pFile;
    bool m_haveFile;

    void startFileCommon(QString& userName, QString& timeStamp);
};

#endif // RESULTFILE_H
