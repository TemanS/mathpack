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

//#define DEBUG_RANDOP

#ifdef DEBUG_RANDOP
#include <QDebug>
#define DBG(x) {x}
#else
#define DBG(x)
#endif

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
    this->clear();
}

void RandOp::clear()
{
    m_qlPrRepeats[op_left].clear();
    m_qlPrRepeats[op_right].clear();
    m_qlLopRepeats.clear();
    m_qlRopRepeats.clear();
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
void RandOp::setMaxOps(int maxZeros, int maxOnes, int maxSames, bool commutes)
{
    // The number of possible Left and Right operands is determined by
    // subtracting the lowest allowable value from the highest allowable
    // value of the respective operands.
    //
    m_maxNumLeftOps  = abs(m_Lmm.y() - m_Lmm.x());
    m_maxNumRightOps = abs(m_Rmm.y() - m_Rmm.x());

    // Initialize the counters and maximum allowable number of zero and
    // one operands, and the maximum allowable number of identical
    // operand pairs.
    //
    m_zeroCount = 0;
    m_onesCount = 0;
    m_sameCount = 0;
    m_maxZeros  = maxZeros;
    m_maxOnes   = maxOnes;
    m_maxSames  = maxSames;
    m_commutes  = commutes;

    // If we allow juxtaposed repeat operand pairs, then the number of
    // possible "unique" operand pairs is the number of Left operands
    // times the number of Right operands.
    // Else the number of "unique" operand pairs is the larger of the
    // number of possible Left or Right operands.
    //
    if(m_commutes)
        m_maxNumOperandPairs = m_maxNumLeftOps * m_maxNumRightOps;
    else
        m_maxNumOperandPairs = m_maxNumLeftOps > m_maxNumRightOps ?
                               m_maxNumLeftOps : m_maxNumRightOps;

#ifdef DEBUG_RANDOP
    qDebug() << "maxNumLeftOps:  " << m_maxNumLeftOps << endl
             << "maxNumRightOps: " << m_maxNumRightOps << endl
             << "m_MaxNumOperandPairs: " << m_maxNumOperandPairs << endl
             << "max zeros: " << m_maxZeros << endl
             << "max ones:  " << m_maxOnes << endl
             << "max sames: " << m_maxSames << endl;
#endif

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

int RandOp::getOne(int min, int max)
{
    return pRand->IRandomX(min, max);
}

int RandOp::getOneUnique(int min, int max)
{
    int val;
    bool isMatch;
    do {
        val = pRand->IRandomX(min, max);
        isMatch = findMatch(val, m_qlLopRepeats);
    } while (isMatch);

    return val;
}

bool RandOp::checkUnique(QPoint &ops)
{
    return findMatchPair(ops.x(), ops.y());
}

void RandOp::setMaxZeros(int maxZeros)
{
    m_maxZeros = maxZeros;
}

void RandOp::setMaxOnes(int maxOnes)
{
    m_maxOnes = maxOnes;
}

void RandOp::setMaxSames(int maxSames)
{
    m_maxSames = maxSames;
}

void RandOp::setCommutes(bool commutes)
{
    m_commutes = commutes;
}

/***********************************************
** PRIVATE FUNCTIONS
************************************************/

void RandOp::getTwoOps(QPoint& ops, bool swap)
{
    int left;
    int right;
    bool isRepeat = true;

    do {

#ifdef DEBUG_RANDOP
        qDebug() << "Zeros: " << m_zeroCount
                 << " Ones: " << m_onesCount
                 << " Sames: " << m_sameCount;
#endif
        left  = pRand->IRandomX(m_Lmm.x(), m_Lmm.y());
        right = pRand->IRandomX(m_Rmm.x(), m_Rmm.y());

        // We never want both operands to be zero.
        //
        if(left == 0 && right == 0) {
            DBG(qDebug() << "LEFT & RIGHT = 0!" << endl;)
            continue;
        }

        // If either operand is a zero or one, or if both operands
        // are identical, bump the corresponding counters and check
        // to see if we've exceeded the maximum allowable number
        // for those conditions.
        //
        if(left == 0 || right == 0) {
            m_zeroCount++;
            if(m_zeroCount > m_maxZeros)
                continue;
        }

        if(left == 1 || right == 1) {
            m_onesCount++;
            if(m_onesCount > m_maxOnes)
                continue;
        }

        if(left == right) {
            m_sameCount++;
            if(m_sameCount > m_maxSames)
                continue;
        }

        isRepeat = findMatchPair(left, right);

    } while(isRepeat);

    if(swap && (right > left)) {
        int temp = left;
        left = right;
        right = temp;
    }

    ops.setX(left);
    ops.setY(right);
}

bool RandOp::findMatchPair(int leftOp, int rightOp)
{
    int index;
    bool isRepeat = false;

#ifdef DEBUG_RANDOP
    qDebug() << "NEW OPS " << endl
             << "\tLeft:  " << leftOp << endl
             << "\tRight: " << rightOp;
#endif

    for(index = 0; index < m_qlPrRepeats[op_left].size(); index++) {

        // If we allow juxtaposed matching operand pairs, then we only
        // have a repeat if we find exactly the same two operands in
        // the same position, Left and Right.
        // Else, we must be certain that we do not have the same pair
        // of operands, even if their Right and Left positions are
        // juxtaposed.
        //
        if(m_commutes) {
            if((leftOp  == m_qlPrRepeats[op_left] [index])
            && (rightOp == m_qlPrRepeats[op_right][index])) {
                isRepeat = true;
                break;
            }
        } else {
            if(((leftOp  == m_qlPrRepeats[op_left] [index])
            &&  (rightOp == m_qlPrRepeats[op_right][index]))
            || ((leftOp  == m_qlPrRepeats[op_right][index])
            &&  (rightOp == m_qlPrRepeats[op_left] [index]))) {
                isRepeat = true;
                break;
            }
        }
    }

    // If we find a match, and the repeatList is full, then clear the
    // lists so we can start the lists from the beginning again.
    //
    if(isRepeat && (index >= m_maxNumOperandPairs)) {
        m_qlPrRepeats[op_left].clear();
        m_qlPrRepeats[op_right].clear();
    }

    // If we have a new unique operand pair, store them in the operand
    // repeats lists.
    //
    if( ! isRepeat && (m_qlPrRepeats[op_left].size() < m_maxNumOperandPairs)) {
        m_qlPrRepeats[op_left].append(leftOp);
        m_qlPrRepeats[op_right].append(rightOp);
    }

#ifdef DEBUG_RANDOP
    qDebug() << "INDEX: " << index << endl
             << "Current size of Repeats Lists: "
             << m_qlPrRepeats[op_left].size();

    if(index >= m_qlPrRepeats[op_left].size())
        qDebug() << "This pair is unique";
    else
        qDebug() << "This pair is NOT unique.";
#endif

    return isRepeat;
}

// If it finds a match, returns true, else returns false.
//
bool RandOp::findMatch(int x, QList<int>& repeatList)
{
    int i;
    bool isMatch = false;

    for(i = 0; i < repeatList.size(); ++i) {
        if(x == repeatList[i]) {
            isMatch = true;
            break;
        }
    }

    // If we find a match, and the repeatList is full, then clear the
    // list so we can start the list from the beginning again.
    //
    if(isMatch && (repeatList.size() >= m_maxNumRightOps))
        repeatList.clear();

    // If we did not find a match, and the list is still smaller than
    // the maximum allowable, put the unique number into the list.
    //
    if( ! isMatch && (repeatList.size() < m_maxNumRightOps))
        repeatList.append(x);

    return isMatch;
}
