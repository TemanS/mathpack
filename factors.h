#ifndef FACTORS_H
#define FACTORS_H

#include <QVector>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE


class Factors
{
public:
    Factors();
    Factors(int a, int b);
    QVector<int> getCommonFactors(int a, int b);
    int getGreatestComFactor() {return m_gcf_exists ? m_facvec.last() : 1;}
    bool existCommonFactors() {return m_gcf_exists;}

private:
    QVector<int> getFactors(int x);

    QVector<int> m_avec;                // FactorsA
    QVector<int> m_bvec;                // FactorsB
    QVector<int> m_facvec;              // Common Factors
    int m_gcf;                          // Greatest Common Factor
    bool m_gcf_exists;
};

#endif // FACTORS_H
