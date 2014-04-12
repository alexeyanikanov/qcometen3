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
#include "explosion.h"

Explosion::Explosion(CometenScene* scene, double time, double _lifeTime,
                     const Vec3f& position, double size, int numParticle,
                     const Vec4f& startColor, const Vec4f& endColor)
: 
Comet(scene, size, position),
startTime(time),
lifeTime(_lifeTime)
{
	done = false;
	decal = new Decal(scene->textures[T_PARTICLE], position, size,
	                  true, Vec3f::UNIT_X, Vec4f(1.0, 1.0, 1.0, 1.0), 0.0);
		
	psystem = new ParticleSystem(time, scene->textures[T_PARTICLE], position, position,
	                             numParticle, numParticle, lifeTime/2, lifeTime, 
	                             false, 0.0, size/lifeTime, size*2/lifeTime, 
	                             true, size/4, startColor, endColor);
}


Explosion::~Explosion() {
	delete decal;
	delete psystem;
}

void Explosion::process(double time, double deltat) {
	double t = (time - startTime) / lifeTime;
	double v;
	
	if (t < 0.05) {
		v = 20*t; 
		v = max<double>(v, 0.5);
	}
	else if (t < 0.55) {
		v = 1-2*(t-0.05);
	}
	else {
		v = 0.0;
	}
	
	decal->setTransparency(v);
	psystem->process(time);
	
	if (time > startTime + lifeTime) {
		done = true;
	}
}

void Explosion::render(const Vec3f & campos) {
	decal->render(campos);
	psystem->render();
}

void Explosion::destroy(double time) {
}

bool Explosion::isDestroyed() {
	return true;
}

bool Explosion::isDone() {
	return done && psystem->done(); 
}

