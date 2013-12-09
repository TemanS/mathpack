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

RandOp::RandOp(QPoint &lmm, QPoint &rmm)
{
    this->init();
    m_Lmm = lmm;
    m_Rmm = rmm;
}

void RandOp::init()
{
    int seed = (int)time(0);
    pRand = new CRandomMersenne(seed);
}

void RandOp::setMinMax(QPoint& lmm, QPoint& rmm)
{
    m_Lmm = lmm;
    m_Rmm = rmm;
}

void RandOp::setMinMax(QRect& limits)
{
    m_Lmm = QPoint(limits.x(), limits.y());
    m_Rmm = QPoint(limits.width(), limits.height());
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
        left = pRand->IRandomX(m_Lmm.x(), m_Lmm.y());
    } while (findMatch(left, m_qlLopRepeats) >= 0);

    do {
        right = pRand->IRandomX(m_Rmm.x(), m_Rmm.y());
    } while (findMatch(right, m_qlRopRepeats) >= 0);

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
    } while (findMatch(val, m_qlRopRepeats) >= 0);

    return val;
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

    // If we reached the maximum number of repeats without finding a match,
    // then put the number into the first entry of the list.
    //
    if(i >= REPEATS)
        repeatList[0] = x;

    // If the list isn't full, put a unique number into it.
    //
    if(repeatList.size() < REPEATS)
        repeatList.append(x);

    return i >= repeatList.size() ? i : -1;
}
