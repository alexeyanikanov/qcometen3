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

#include "cometenmath.h"

bool TestDynamicSpherePlane(const Vec3f & c, double r, 
	const Vec3f & W, const Plane & plane, double & time)
{
	double dist, dotNW;
	
	dist = plane.N.dot(c) - plane.d;
	
	if (dist > r) {
		dotNW = plane.N.dot(W);
		if (dotNW < 0) {
			time = (r - dist) / dotNW;
			return true;
		}
		else {
			return false;
		}		  
	}
	else if (dist < -r) {
		dotNW = plane.N.dot(W);
		if (dotNW > 0) {
			time = -(r + dist) / dotNW;
			return true;
		}
		else {
			return false;
		}
	} else {
		time = 0.0;
		return true;
	}
}
