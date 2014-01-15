/******************************************************************************
**
**  mathpack - a library of classes and templates originally devised to
**             accomodate a dialog-based suite of math exercises.
**
**  Tony Camuso
**  December, 2011
**
**  Version 0.1
**
**    mathpack is free software: you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation, either version 3 of the License, or
**    (at your option) any later version.
**
**    This program is distributed in the hope that it will be useful,
**    but WITHOUT ANY WARRANTY; without even the implied warranty of
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**    GNU General Public License for more details.
**
**  GNU General Public License http://www.gnu.org/licenses/gpl.html
**
**  Copyright 2011 by Tony Camuso.
**
******************************************************************************/

#include "randomop.h"

enum { prLeft, prRight };

RandOp::RandOp(QPoint &lmm, QPoint &rmm)
{
    this->init();
    setMinMax(lmm, rmm);
}

void RandOp::init()
{
    int seed = (int)time(0);
    pRand = new CRandomMersenne(seed);
    m_qlPrRepeats[op_left].clear();
    m_qlPrRepeats[op_right].clear();
}

void RandOp::setMinMax(QPoint& lmm, QPoint& rmm)
{
    m_Lmm = lmm;
    m_Rmm = rmm;
    setMaxOps();
}

void RandOp::setMinMax(QRect& limits)
{
    m_Lmm = QPoint(limits.x(), limits.y());
    m_Rmm = QPoint(limits.width(), limits.height());
    setMaxOps();
}

void RandOp::setMaxOps()
{
    m_maxNumRightOps = abs(m_Lmm.y() - m_Lmm.x());
    m_maxNumRightOps = abs(m_Rmm.y() - m_Rmm.x());

    // Determine the maximum number of possible operand pairs by
    // multiplying the maximum number of left operands times the
    // maximum number of right operands.
    //
    m_maxNumOperandPairs = m_maxNumLeftOps * m_maxNumRightOps;
}

void RandOp::getPair(QPoint& opr, bool swap)
{
    getTwoOps(opr, swap);
}

void RandOp::getPair(QPoint& opr, QPoint& lmm, QPoint& rmm, bool swap)
{
    setMinMax(lmm, rmm);
    getTwoOps(opr, swap);
}

void RandOp::getPair(QPoint& opr, QRect& limits, bool swap)
{
    setMinMax(limits);
    getTwoOps(opr, swap);
}

void RandOp::getTwoOps(QPoint& opr, bool swap)
{
    int left;
    int right;

    do {
            left  = pRand->IRandomX(m_Lmm.x(), m_Lmm.y());
            right = pRand->IRandomX(m_Rmm.x(), m_Rmm.y());
    } while(findMatchPair(left, right) != op_unique);

    if(swap && (right > left)) {
        int temp = left;
        left = right;
        right = temp;
    }

    opr.setX(left);
    opr.setY(right);
}

int RandOp::getOne(int min, int max)
{
    return pRand->IRandomX(min, max);
}

int RandOp::getOneUnique(int min, int max)
{
    int val;
    do {
        val = pRand->IRandomX(min, max);
    } while (findMatch(val, m_qlRopRepeats) != op_unique);

    return val;
}

int RandOp::findMatchPair(int left, int right)
{
    int index;

    for(index = 0; index < m_qlPrRepeats[op_left].size(); index++) {
        if((left  == m_qlPrRepeats[op_left][index])
        && (right == m_qlPrRepeats[op_right][index])) {
            break;
        }
    }

    if(index >= m_maxNumOperandPairs) {
        m_qlPrRepeats[op_left].clear();
        m_qlPrRepeats[op_right].clear();
    }

    if(m_qlPrRepeats[op_left].size() < m_maxNumOperandPairs) {
        m_qlPrRepeats[op_left].append(left);
        m_qlPrRepeats[op_right].append(right);
    }

    return index >= m_qlPrRepeats[op_left].size() ? op_unique : op_notunique;
}

// If it finds a match, returns the index of the match.
// Else, returns a -1 to indicate no match.
//
int RandOp::findMatch(int x, QList<int>& repeatList)
{
    int i;

    for(i = 0; i < repeatList.size(); ++i)
        if(x == repeatList[i])
            break;

    // If we reached the maximum number of operands, without finding a
    // match, then clear the list so we can start the list from the
    // beginning again.
    //
    if(i >= m_maxNumRightOps)
        repeatList.clear();

    // If we did not find a match, put the unique number into the list.
    //
    if(repeatList.size() < m_maxNumRightOps)
        repeatList.append(x);

    return i >= repeatList.size() ? op_unique : op_notunique;
}
