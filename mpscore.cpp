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

#include "mpscore.h"

int MpScore::getPercentGrade()
{
    return getPercentGradeCommon();
}

int MpScore::getPercentGrade(int count, int correct)
{
    m_count = count;
    m_correct = correct;
    return getPercentGradeCommon();
}

int MpScore::getPercentGradeCommon()
{
    if(m_count == 0) return 0;
    m_percent = (m_correct * 100)/m_count;
    return m_percent;
}

const char *MpScore::getLetterGrade()
{
    return getLetterGradeCommon();
}

const char *MpScore::getLetterGrade(int count, int correct)
{
    m_count = count;
    m_correct = correct;
    return getLetterGradeCommon();
}

const char *MpScore::getLetterGradeCommon()
{
    getPercentGrade();

    if (m_percent >= 98) m_letterGrade = "A+";
    else if (m_percent >= 93 && m_percent < 98) m_letterGrade = "A";
    else if (m_percent >= 90 && m_percent < 93) m_letterGrade = "A-";
    else if (m_percent >= 88 && m_percent < 90) m_letterGrade = "B+";
    else if (m_percent >= 83 && m_percent < 88) m_letterGrade = "B";
    else if (m_percent >= 80 && m_percent < 83) m_letterGrade = "B-";
    else if (m_percent >= 78 && m_percent < 80) m_letterGrade = "C+";
    else if (m_percent >= 73 && m_percent < 78) m_letterGrade = "C";
    else if (m_percent >= 70 && m_percent < 73) m_letterGrade = "C-";
    else if (m_percent >= 68 && m_percent < 70) m_letterGrade = "D+";
    else if (m_percent >= 63 && m_percent < 68) m_letterGrade = "D";
    else if (m_percent >= 60 && m_percent < 63) m_letterGrade = "D-";
    else m_letterGrade = "F";

    return m_letterGrade;
}
