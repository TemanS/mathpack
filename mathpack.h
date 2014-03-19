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

#ifndef MATHPACK_H
#define MATHPACK_H

#include "mathpack_global.h"
#include "mpscore.h"
#include "randomc.h"
#include "randomop.h"
#include "testparm.h"
#include "resultfilemanager.h"
#include "leastcommult.h"
#include "factors.h"

class MATHPACKSHARED_EXPORT Mathpack {
public:
    Mathpack(){}
};

#define max(a,b)((a > b) ? a : b)
#define min(a,b)((a < b) ? a : b)
#define abs(x)((x < 0) ? (x * -1) : x)

#endif // MATHPACK_H
