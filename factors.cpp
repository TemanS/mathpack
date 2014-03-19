#include "factors.h"

Factors::Factors()
{
}

Factors::Factors(int a, int b)
{
    this->init(a, b);
}

void Factors::init(int a, int b)
{
    m_avec.clear();
    m_bvec.clear();
    getCommonFactors(a, b);
}

int Factors::getGreatestComFactor(int a, int b)
{
    this->init(a, b);
    return m_gcf_exists ? m_facvec.last() : 1;
}

QVector<int> Factors::getCommonFactors(int a, int b)
{
    m_avec = getFactors(a);
    m_bvec = getFactors(b);
    int count = m_avec.size() > m_bvec.size() ? m_bvec.size() : m_avec.size();

    for(int i = 0; i < count; ++i) {
        if((m_avec[i] != 0) && (m_bvec[i] != 0) && (m_avec[i] == m_bvec[i]))
            m_facvec << m_avec[i];
    }

    m_gcf_exists = !m_facvec.isEmpty();
    return m_facvec;
}

QVector<int> Factors::getFactors(int x)
{
    static QVector<int> vec;
    vec.clear();

    for(int i = 2; i <= x; ++i) {
        if(x % i == 0)
            vec << i;
        else
            vec << 0;
    }
    return vec;
}
