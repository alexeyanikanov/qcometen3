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
#include "particlesystem.h"

ParticleSystem::ParticleSystem(double time, GLuint _texture,
                               const Vec3f & _emPos1,
                               const Vec3f & _emPos2,
                               int _maxParticle,
                               int numCreateAtStart,
                               double _minLifeTime,
                               double _maxLifeTime,
                               bool _createNewParticle, 
                               double _newParticlePerSec, 
                               double _minVel, double _maxVel,
                               bool _slowdown,
                               double _particleSize, 
                               const Vec4f & _startColor, 
                               const Vec4f & _endColor) :
emitterPos1(_emPos1),
emitterPos2(_emPos2),
maxParticle(_maxParticle),
minLifeTime(_minLifeTime), 
maxLifeTime(_maxLifeTime),
createNewParticle(_createNewParticle),
newParticlePerSec(_newParticlePerSec),
minVelocity(_minVel),
maxVelocity(_maxVel),
slowdown(_slowdown),
particleSize(_particleSize),
startColor(_startColor),
endColor(_endColor),
texture(_texture)
{
	particleArr = new Particle[maxParticle];	
	indexArr = new int[maxParticle];
	
	numCreateAtStart = min<int>(maxParticle, numCreateAtStart);
	numNewParticle = (double) numCreateAtStart;
	numActiveParticle = 0;	
		
	for (int i = 0; i < maxParticle; i++) {
		particleArr[i].active = false;
	}	
	
	lastUpdateTime = time;
}

ParticleSystem::~ParticleSystem() {
	delete[] particleArr;
	delete[] indexArr;
}

void ParticleSystem::process(double time) {
	Particle *p;
	double t;
	int j = 0;
	double deltat = time - lastUpdateTime;
	lastUpdateTime = time;
	
	if (createNewParticle)
		numNewParticle += newParticlePerSec * deltat;
	
	for (int i = 0; i < maxParticle; i++) {
		p = particleArr + i;

		// particle die
		if (p->startTime + p->lifeTime < time) {
			p->active = false;
		}
		
		if (!p->active && numNewParticle < 1.0) {
			continue;
		}
		
		if (!p->active) {
			double q = frand();
			p->position = (emitterPos1 * q) + (emitterPos2 * (1-q));
			p->velocity = Vec3f::randomUnit() 
				* frand(minVelocity, maxVelocity);
			p->startTime = time;
			p->lifeTime = frand(minLifeTime, maxLifeTime);
			p->lifeTime = max<double>(0.001, p->lifeTime);
			p->invLifeTime = 1/ p->lifeTime;
			p->active = true;
			numNewParticle -= 1.0;
			t = (time - p->startTime) * p->invLifeTime;
		} else {
			t = (time - p->startTime) * p->invLifeTime;
			if (slowdown) 
				p->position += p->velocity * deltat * (1-t);
			else
				p->position += p->velocity * deltat;
		}

		p->color = (startColor * (1-t)) + (endColor * t);

		indexArr[j] = i;
		j++;
	}
	
	numActiveParticle = j;
}

void ParticleSystem::render() {
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvf(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
	
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);		
	glColorPointer(4, GL_FLOAT, sizeof(Particle), &particleArr[0].color);
	glVertexPointer(3, GL_FLOAT, sizeof(Particle), &particleArr[0].position);
	
	// pointparameter in cometenscene->init()

	glEnable(GL_POINT_SPRITE_ARB);
	glPointSize(particleSize);

	glDrawElements(GL_POINTS, numActiveParticle, GL_UNSIGNED_INT, indexArr);
	
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	glDisable(GL_POINT_SPRITE_ARB);
	glTexEnvf(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_FALSE);
	
	glDisable(GL_BLEND);
}

void ParticleSystem::stop() {
	createNewParticle = false;
	numNewParticle = 0.0;
}

bool ParticleSystem::done() {
	return (!createNewParticle) && (numActiveParticle == 0);
}
