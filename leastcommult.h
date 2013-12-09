#ifndef LEASTCOMMULT_H
#define LEASTCOMMULT_H

#include <QVector>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class LeastComMult
{
public:
    LeastComMult();
    int getLeastCommonMultiple(QVector<int>& numList);
    int sumTable(QVector<int>& table);
    int getNextPrime(int prime);
};

#endif // LEASTCOMMULT_H
