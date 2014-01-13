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

#ifndef RANDOMOP_H
#define RANDOMOP_H

#include <QList>
#include <QPoint>
#include <QRect>
#include <time.h>
#include "randomc.h"

#define REPEATS 7  // After 10 int pairs, repeats are allowed again

class RandOp
{
public:
    RandOp() { init(); };
    RandOp(QPoint& lmm, QPoint& rmm);
    void setMinMax(QPoint& lmm, QPoint& rmm);
    void setMinMax(QRect& limits);
    void setMaxCombo(int maxCombo);
    int getMaxCombo();
    void getPair(QPoint& opr, bool swap=false);
    void getPair(QPoint& opr, QPoint& lmm, QPoint& rmm, bool swap=false);
    void getPair(QPoint& opr, QRect& limits, bool swap=false);
    int getOne(int min, int max);
    int getOneUnique(int min, int max);

private:
    void init();
    int findMatch(int x, QList<int>& repeatList);
    void getTwoOps(QPoint& opr, bool swap);

    QList<int> m_qlLopRepeats;
    QList<int> m_qlRopRepeats;
    QList<int> m_qlPrRepeats[2];

    int m_maxCombo;  // Maximum number of possible operand pairs

    QPoint m_Lmm;    // Left operand min/max randomizing limits
    QPoint m_Rmm;    // Right operand min/max randomizing limits
    CRandomMersenne *pRand;
};

#endif // RANDOMOP_H
