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
#include <randomc.h>

#define DFLTMAXZEROS 1  // Default value for max number of 0 operands
#define DFLTMAXONES  1  // Default value for max number of 1 operands
#define DFLTMAXSAMES 1  // Default value for max number of same L & R operands

enum {
    op_left,
    op_right
};

class RandOp
{
public:
    RandOp();
    RandOp(QPoint& lmm, QPoint& rmm);
    void clear();
    void setMinMax(QPoint& lmm, QPoint& rmm);
    void setMinMax(QRect& limits);
    void setMinMax(int leftMin, int leftMax, int rightMin, int rightMax);
    void getPair(QPoint& opr, bool swap=false);
    void getPair(QPoint& opr, QPoint& lmm, QPoint& rmm, bool swap=false);
    void getPair(QPoint& opr, QRect& limits, bool swap=false);
    int getOne(int min, int max);
    int getOneUnique(int min, int max);
    bool checkUnique(QPoint& ops);
    void setMaxZeros(int maxZeros);
    void setMaxOnes (int maxOnes);
    void setMaxSames(int maxSames);
    void setCommutes(bool commutes);
    void setMaxOps(int maxZeros = DFLTMAXZEROS,
                   int maxOnes  = DFLTMAXONES,
                   int maxSames = DFLTMAXSAMES,
                   bool commutes = false);

private:
    void init();
    bool findMatch(int x, QList<int>& repeatList);
    bool findMatchPair(int leftOp, int rightOp);
    void getTwoOps(QPoint& opr, bool swap);

    QList<int> m_qlLopRepeats;
    QList<int> m_qlRopRepeats;
    QList<int> m_qlPrRepeats[2];

    // These values are used to calculate the maximum allowable size
    // for the QLists that contain operands and operand pairs that
    // have already been generated. We keep track of them in order
    // to eliminate, or at least minimize, the number of times the
    // user is presented with an operand or operand pairs having the
    // same values as were previously given.
    //
    int m_maxNumLeftOps;        // Maximum number of left operands
    int m_maxNumRightOps;       // Maximum number of right operands
    int m_maxNumOperandPairs;   // Maximum number of operand pairs
    bool m_commutes;            // Allow juxtaposed matched pairs

    // We need to limit the number of operands that come up as zero or 1,
    // and we need to limit the number of times that both Left and Right
    // operands are the same value.
    //
    int m_maxZeros;             // Max allowable 0 operands
    int m_maxOnes;              // Max allowable 1 operands
    int m_maxSames;             // Max allowable same Left & Right operands
    int m_zeroCount;            // Count number of operands = 0
    int m_onesCount;            // Count number of operands = 1
    int m_sameCount;            // Count number of times ops are the same
    bool m_isMinMaxSet;         // True value indicates minMax has been set

    QPoint m_Lmm;               // Left operand min/max values
    QPoint m_Rmm;               // Right operand min/max values
    CRandomMersenne *pRand;
};

#endif // RANDOMOP_H
