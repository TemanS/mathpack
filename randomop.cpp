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
#include <QDebug>

enum { prLeft, prRight };

RandOp::RandOp()
{
    init();
}

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

// setMinMax - initialize the internal min/max operand value parameters.
//
// This should only be called ONCE for each set of tests, not for every
// problem.
//
// QPoint lmm - min/max values of the Left operand
// QPoint rmm - min/max values of the Right operand
//
void RandOp::setMinMax(QPoint& lmm, QPoint& rmm)
{
    m_Lmm = lmm;
    m_Rmm = rmm;
    setMaxOps();
}

// setMinMax - initialize the internal min/max operand value parameters.
//
// Overloaded version.
//
// QRect limits - contains the min/max values for the Left and Right
// operands.
//
void RandOp::setMinMax(QRect& limits)
{
    m_Lmm = QPoint(limits.x(), limits.y());
    m_Rmm = QPoint(limits.width(), limits.height());
    setMaxOps();
}

// setMaxOps - set the maximum number of operands
//
void RandOp::setMaxOps()
{
    m_maxNumLeftOps  = abs(m_Lmm.y() - m_Lmm.x());
    m_maxNumRightOps = abs(m_Rmm.y() - m_Rmm.x());

    // Determine the maximum number of possible operand pairs by
    // multiplying the maximum number of left operands times the
    // maximum number of right operands.
    //
    m_maxNumOperandPairs = m_maxNumLeftOps * m_maxNumRightOps;
    m_zeroCount = 0;
    m_sameCount = 0;

    qDebug() << "maxNumLeftOps:  " << m_maxNumLeftOps << endl
             << "maxNumRightOps: " << m_maxNumRightOps << endl
             << "m_MaxNumOperandPairs: " << m_maxNumOperandPairs << endl;
}

// getPair - overloaded function to return a pair of unique operands.
//
// QPoint ops - will contain the new operand pair.
// QPoint lmm - contains the min/max values of the Left operand
// QPoint rmm - contains the min/max values of the Right operand
// bool swap  - when true, puts the larger of the two operands in the
//              Left position. Default value is false.
//
void RandOp::getPair(QPoint& ops, QPoint& lmm, QPoint& rmm, bool swap)
{
    setMinMax(lmm, rmm);
    getTwoOps(ops, swap);
}

// getPair - overloaded function to return a pair of unique operands.
//
// QPoint ops   - will contain the new operand pair.
// QRect limits - contains the min/max values of the Left and Right
//                operands.
// bool swap  - when true, puts the larger of the two operands in the
//              Left position. Default value is false.
//
void RandOp::getPair(QPoint& ops, QRect& limits, bool swap)
{
    setMinMax(limits);
    getTwoOps(ops, swap);
}

// getPair - overloaded function to return a pair of unique operands.
//
// This instance of getPair() assumes that the limits have already been
// set and that setMaxOps has already been called.
//
// QPoint ops - will contain the new operand pair.
// bool swap  - when true, puts the larger of the two operands in the
//              Left position. Default value is false.
//
void RandOp::getPair(QPoint& ops, bool swap)
{
    getTwoOps(ops, swap);
}

#if 0
// getPair - overloaded function to return a pair of unique operands.
//
// This instance of getPair() will return an
// This instance of getPair() assumes that the limits have already been
// set and that setMaxOps has already been called.
//
// QPoint ops - will contain the new operand pair.
// bool swap  - when true, puts the larger of the two operands in the
//              Left position. Default value is false.
//
void RandOp::getPairMultiple(QPoint& ops, bool swap)
{

}
#endif

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

bool RandOp::checkUnique(QPoint &ops)
{
    return findMatchPair(ops.x(), ops.y());
}

/***********************************************
** PRIVATE FUNCTIONS
************************************************/

void RandOp::getTwoOps(QPoint& ops, bool swap)
{
    int left;
    int right;

    do {
            left  = pRand->IRandomX(m_Lmm.x(), m_Lmm.y());
            right = pRand->IRandomX(m_Rmm.x(), m_Rmm.y());

            // Don't allow both operands to be zero, and only allow
            // MAXZEROS zero operands and MAXSAMES operands to be
            // identical.
            //
            if((left == 0 && right == 0)
            || m_zeroCount >= MAXZEROS
            || m_sameCount >= MAXSAMES)
                continue;

            if(left == 0 || right == 0)
                m_zeroCount++;

            if(left == right)
                m_sameCount++;

    } while(findMatchPair(left, right) != op_unique);

    if(swap && (right > left)) {
        int temp = left;
        left = right;
        right = temp;
    }

    ops.setX(left);
    ops.setY(right);
}

int RandOp::findMatchPair(int leftOp, int rightOp)
{
    int index;

    qDebug() << "NEW OPS " << endl
             << "\tLeft:  " << leftOp << endl
             << "\tRight: " << rightOp;

    for(index = 0; index < m_qlPrRepeats[op_left].size(); index++) {
        if((leftOp  == m_qlPrRepeats[op_left][index])
        && (rightOp == m_qlPrRepeats[op_right][index])) {
            qDebug() << "Found match at index: " << index << endl
                     << "\tLeft:  " << m_qlPrRepeats[op_left][index] << endl
                     << "\tRight: " << m_qlPrRepeats[op_right][index] << endl;
            break;
        }
    }

    if(index >= m_maxNumOperandPairs) {
        m_qlPrRepeats[op_left].clear();
        m_qlPrRepeats[op_right].clear();
    }

    // If we have a new unique operand pair, store them in the operand
    // repeats lists. We must also bump the index, since we are increasing
    // the size of the arrays, and will be using the size of the arrays
    // to tell us whether we found a unique pair. If we don't bump the
    // index, we could return that the pair is unique, when it is not.
    //
    if(m_qlPrRepeats[op_left].size() < m_maxNumOperandPairs) {
        m_qlPrRepeats[op_left].append(leftOp);
        m_qlPrRepeats[op_right].append(rightOp);
        index++;
    }

    qDebug() << "INDEX: " << index << endl
             << "Current size of Repeats Lists: "
             << m_qlPrRepeats[op_left].size();

    if(index >= m_qlPrRepeats[op_left].size())
        qDebug() << "This pair is unique";
    else
        qDebug() << "This pair is NOT unique.";

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
