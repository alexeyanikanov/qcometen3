/***************************************************************************
 *   Copyright (C) 2005 by Peter Müller   *
 *   pmueller@cs.tu-berlin.de   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef GLOBAL_H
#define GLOBAL_H

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <vector>
#include <list>
#include <cassert>

using namespace std;

extern GLuint *textures;

enum {
	T_PARTICLE,
	T_LIGHTMAP,
	T_BACKGROUND,
	T_NUM,
};

struct Setting {
	int intValue;
	double doubleValue;
};

extern Setting* config;

enum {
	SE_COLORS,
	SE_CREATEINTERVAL,
	SE_DOMATRIX,
	SE_DOSLOWMOTION,
	SE_DOBLITZ,
	SE_DOSPLITCOMET,
	SE_DOCURVECOMET,
	SE_DOROTATECOMET,
	SE_FREECAMERA,
	SE_FREECAMERASPEED,
	SE_TIMESCALE,
	SE_SIZESCALE,
	SE_NUM,
};

enum {
	C_RED,
	C_BLUE,
	C_GREEN,
	C_BUNT,
	C_EXTREMBUNT
};

#endif
