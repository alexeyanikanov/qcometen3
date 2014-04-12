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
#include "decal.h"

Decal::Decal(GLuint _texture, const Vec3f & _position,  double _size,
             bool _billboard, const Vec3f & _normal, const Vec4f & _color,
             double _transparency)
:
texture(_texture),
position(_position),
size(_size),
billboard(_billboard),
normal(_normal),
color(_color),
transparency(_transparency)
{
}


Decal::~Decal()
{
}



void Decal::render(const Vec3f & campos) {
	Vec3f dir, up, right;
		
	if (billboard) {
		dir = campos - position;
		dir.normalize();
	} else {
		dir = normal;
	}
	
	Vec3f::generateOrthonormalBasis(dir, up, right);
	
	right *= 0.5 * size;
	up *= 0.5 * size;
	
	glPushMatrix();

	glTranslatef(position[0], position[1], position[2]);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glColor4f(color[0], color[1], color[2], color[3] * transparency);
	
	glBegin(GL_QUADS);
	
	glTexCoord2f(0.0, 1.0);	
	glVertex3f((right[0] + up[0]), (right[1] + up[1]), (right[2] + up[2]));
	glTexCoord2f(1.0, 1.0);
	glVertex3f((-right[0] + up[0]), (-right[1] + up[1]), (-right[2] + up[2]));
	glTexCoord2f(1.0, 0.0);
	glVertex3f((-right[0] - up[0]), (-right[1] - up[1]), (-right[2] - up[2]));
	glTexCoord2f(0.0, 0.0);
	glVertex3f((right[0] - up[0]), (right[1] - up[1]), (right[2] - up[2]));
	
	glEnd();
	
	glPopMatrix();	
}
