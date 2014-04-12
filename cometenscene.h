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
#ifndef COMETENSCENE_H
#define COMETENSCENE_H

#include "global.h"
#include "math.h"
#include "particlesystem.h"
#include "comet.h"


class Comet;

class CometenScene {
public:
	CometenScene(const Setting* config, const GLuint* textures, 
	             double time, double boxWidth, double boxHeight);
	~CometenScene();
	
	void process(double time);
	void render(const Vec3f& campos);

	void addComet(Comet * comet);
	list<Comet*> comets;

	void resizeBox(double boxWidth, double boxHeight);
	double getBoxWidth();
	double getBoxHeight();
	const Plane& getPlane(int i);
	int getNumPlanes();
	
	const Setting* config;
	const GLuint* textures;
private:
	vector<Plane> collisionPlanes;
	double cometInterval;
	double boxWidth;
	double boxHeight;
	double lastCometProcess;
	double nextComet;
	
	void getCometColors(Vec4f& acolor, Vec4f& ecolor);
	void createComet(double time);

	void renderBackground();
};

inline double CometenScene::getBoxWidth() {
	return boxWidth;
}

inline double CometenScene::getBoxHeight() {
	return boxHeight;
}

inline const Plane& CometenScene::getPlane(int i) {
	assert(i >= 0 && i < getNumPlanes());
	return collisionPlanes[i];
}

inline int CometenScene::getNumPlanes() {
	return collisionPlanes.size();
}


#endif
