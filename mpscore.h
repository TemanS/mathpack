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

#ifndef MPSCORE_H
#define MPSCORE_H

class MpScore {
public:
    MpScore() {m_count=0; m_correct=0; m_percent=0;}
    MpScore(int count, int correct) {m_count = count; m_correct = correct;}
    void setCount(int count) {m_count = count;}
    void setCorrect(int correct) {m_correct = correct;}
    int getPercentGrade();
    int getPercentGrade(int count, int correct);
    const char *getLetterGrade();
    const char *getLetterGrade(int count, int correct);


private:
    int m_count;                // Total number of problems
    int m_correct;              // Total of correct answers
    int m_percent;              // Percent as an integer
    int getPercentGradeCommon();
    const char *m_letterGrade;  // Letter Grade
    const char *getLetterGradeCommon();
};

#endif // MPSCORE_H
