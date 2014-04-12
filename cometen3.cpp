/***************************************************************************
 *   Copyright (C) 2005 by Peter Müller                                    *
 *   pmueller@cs.tu-berlin.de                                              *
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

#include "cometen3.h"

Cometen3::Cometen3() {
	scene = 0;
	camera = 0;
	cameraCurve = 0;
}

Cometen3::~Cometen3() {
	quit();
}

void Cometen3::init(const Setting* conf, const GLuint* text,
                    int resX, int resY, double time) 
{
	quit();
	
	config = conf;
	textures = text;
	
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(1.0);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glFrontFace(GL_CW);
	glPointParameterfARB(GL_POINT_FADE_THRESHOLD_SIZE_ARB, 60.0);
	glPointParameterfARB(GL_POINT_SIZE_MIN_ARB, 0.0);
	glPointParameterfARB(GL_POINT_SIZE_MAX_ARB, 1000.0);
	
	
	
	sceneTimer.setScale(1.0, time);
	sceneTimer.setTime(0.0, time);
	
	timeScale.setScale(config[SE_TIMESCALE].doubleValue,
	                   sceneTimer.getTime(time));
	timeScale.setTime(0.0, sceneTimer.getTime(time));
	
	scene = new CometenScene(config, textures, 
	                         timeScale.getTime(sceneTimer.getTime(time)),
	                         1.0, 1.0);
	camera = new Camera(Vec3f::ZERO, Vec3f::ZERO); 
	cameraCurve = 0;
	curveCreateTime = 0.0;
	curveTime = 120.0 
		/ (1.0 + 2*clamp<double>(config[SE_FREECAMERASPEED].doubleValue, 0, 10));

	resize(resX, resY);
	
	nextEffect = time + frand(45.0, 120.0);

	state = S_NORMAL;
}

void Cometen3::quit() {
	if (scene != 0) {
		delete scene;
		scene = 0;
	}
	
	if (camera != 0) {
		delete camera;
		camera = 0;
	}
	
	if (cameraCurve != 0) {
		delete cameraCurve;
		cameraCurve = 0;
	}
}

void Cometen3::changeViewport(int x, int y, int width, int height) {
	// a, b, c: size' = size * sqrt[1 / (a + b*d + c*d^2)]
	float dist_attenuation[] = {0.0, 0.0, 
			4.0 * tanf(deg2rad(fov/2)) * tanf(deg2rad(fov/2)) /(height*height)};
	glPointParameterfvARB(GL_POINT_DISTANCE_ATTENUATION_ARB, dist_attenuation);
	
	glViewport(x, y, width, height);	
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, (double)width / height, 1.0, 10000.0);
	glMatrixMode(GL_MODELVIEW);
}

void Cometen3::resize(int newResX, int newResY) {
	resX = newResX;
	resY = newResY;

	changeViewport(0,0,resX, resY);
	
	double boxWidth = 150.0 * config[SE_SIZESCALE].doubleValue;
	double boxHeight = boxWidth * resY / resX;
	scene->resizeBox(boxWidth, boxHeight);
}

void Cometen3::process(double time) {
	processDefaultCameraPos(time);
	
	switch (state) {
	case S_NORMAL:
		processNormal(time);
		break;
	case S_MATRIX:
		processMatrix(time);
		break;
	case S_SLOWMOTION:
		processSlowmotion(time);
		break;
	default:
		state = S_NORMAL;
		stateStart = time;
		break;
	}
	
	scene->process(timeScale.getTime(sceneTimer.getTime(time)));
	
	if (state == S_NORMAL && time > nextEffect) {
		if (config[SE_DOMATRIX].intValue 
		    && config[SE_DOSLOWMOTION].intValue) {
			switch(rand() % 2) {
			case 0:
				state = S_SLOWMOTION;
				stateStart = time;
				break;
			case 1:
			default:
				state = S_MATRIX;
				stateStart = time;
				break;
			}	
		}
		else if (config[SE_DOMATRIX].intValue) {
			state = S_MATRIX;
			stateStart = time;
		}
		else if (config[SE_DOSLOWMOTION].intValue) {
			state = S_SLOWMOTION;
			stateStart = time;
		}
		
		nextEffect = time + frand(120.0, 180.0);
	}	
}


void Cometen3::newCameraCurve(double time) {
	Vec3f points[4];
	double bw = scene->getBoxWidth() / 2 - 3;
	double bh = scene->getBoxHeight() / 2 - 3;
	
	if (cameraCurve != 0) {
		points[0] = cameraCurve->position(1.0);
		points[1] = cameraCurve->position(2.0);
		delete cameraCurve;
	}
	else {
		points[0] = Vec3f(bw,frand(-bh,bh),frand(-bw,bw));
		points[1] = Vec3f(bw,frand(-bh,bh),frand(-bw,bw));
	}
	
	switch (rand() % 6) {
	case 0:
		points[2] = Vec3f(bw,frand(-bh,bh),frand(-bw,bw));
		points[3] = Vec3f(bw,frand(-bh,bh),frand(-bw,bw));
		break;
	case 1:
		points[2] = Vec3f(bw,frand(-bh,bh),frand(-bw,bw));
		points[3] = Vec3f(bw,frand(-bh,bh),frand(-bw,bw));
		break;
	case 2:
		points[2] = Vec3f(frand(-bw,-bw),bh,frand(-bw,bw));
		points[3] = Vec3f(frand(-bw,-bw),bh,frand(-bw,bw));
		break;
	case 3:
		points[2] = Vec3f(frand(-bw,-bw),bh,frand(-bw,bw));
		points[3] = Vec3f(frand(-bw,-bw),bh,frand(-bw,bw));
		break;
	case 4:			
		points[2] = Vec3f(frand(-bw,-bw),frand(-bh,bh),bw);
		points[3] = Vec3f(frand(-bw,-bw),frand(-bh,bh),bw);
		break;
	case 5:
	default:
		points[2] = Vec3f(frand(-bw,-bw),frand(-bh,bh),bw);
		points[3] = Vec3f(frand(-bw,-bw),frand(-bh,bh),bw);
		break;
	}
	
	points[3] = (points[3] + points[2]) * 0.5;
	
	cameraCurve = new Bezier3(points, 4);
	curveCreateTime = time;	
}

void Cometen3::processDefaultCameraPos(double time) {
	if (config[SE_FREECAMERA].intValue) {
		if (cameraCurve == 0 
		    || time > curveCreateTime + curveTime) 
		{
			newCameraCurve(time);
		}
		
		assert(cameraCurve != 0);
		
		defaultCameraPosition = cameraCurve->position(
			(time - curveCreateTime) / curveTime);
	}
	else {
		double boxCamDistance = scene->getBoxHeight() / 2 / tanf(deg2rad(fov/2));
		defaultCameraPosition = 
			Vec3f(0.0, 0.0, boxCamDistance + scene->getBoxWidth()/2);
	}
}

void Cometen3::processNormal(double time) {
 	camera->position = defaultCameraPosition;
	camera->viewpoint = Vec3f::ZERO;

	camera->fade = 0.0;
	camera->blackBars = false;
}

void Cometen3::processMatrix(double time) {
	double t = time - stateStart;
	
	camera->viewpoint = Vec3f::ZERO;
	
	if (t < 0.5) {
		camera->position = defaultCameraPosition;
		camera->fade = (t) / 0.5;
		camera->blackBars = false;
		sceneTimer.setScale(1.0, time);
	}
	else if (t < 1.0) {
		camera->position = defaultCameraPosition;
		camera->fade = 1.0;
		camera->blackBars = true;
		sceneTimer.setScale(0.1, time);
	}
	else if (t < 31.0) {
		camera->blackBars = true;
		sceneTimer.setScale(0.1, time);
		if (t < 1.5) {
			camera->fade = 1 - (t - 1.0) / 0.5;
		}
		else if (t > 30.5) {
			camera->fade = (t - 30.5) / 0.5;
		}
		else {
			camera->fade = 0.0;
		}
		
		double w = (t - 1.0) / 30.0 * Pi * 2;
		camera->position[0] = sinf(w) * scene->getBoxWidth()/2;
		camera->position[1] = 0;
		camera->position[2] = cosf(w) * scene->getBoxWidth()/2;
		camera->viewpoint = Vec3f::ZERO;
	}
	else if (t < 31.5) {
		camera->position = defaultCameraPosition;
		camera->fade = 1.0;
		camera->blackBars = true;
		sceneTimer.setScale(0.1, time);
	}
	else if (t < 32.0) {
		camera->position = defaultCameraPosition;
		camera->fade = 1 - (t - 31.5) / 0.5;
		sceneTimer.setScale(1.0, time);
		camera->blackBars = false;
	}
	else {
		camera->blackBars = false;
		camera->fade = 0.0;
		camera->position = defaultCameraPosition;
		sceneTimer.setScale(1.0, time);
		
		nextEffect = time + frand(120.0, 180.0);
		state = S_NORMAL;
		stateStart = time;
	}
}

void Cometen3::processSlowmotion(double time) {
	double t = time - stateStart;
	
	camera->viewpoint = Vec3f::ZERO;
	
	if (t < 0.5) {
		camera->position = defaultCameraPosition;
		camera->fade = (t) / 0.5;
		camera->blackBars = false;
		sceneTimer.setScale(1.0, time);
	}
	else if (t < 1.0) {
		camera->position = defaultCameraPosition;
		camera->fade = 1.0;
		camera->blackBars = true;
		sceneTimer.setScale(0.25f, time);
	}
	else if (t < 16.0) {
		camera->blackBars = true;
		if (t < 1.5) {
			camera->fade = 1 - (t - 1.0) / 0.5;
		}
		else if (t > 15.5) {
			camera->fade = (t - 15.5) / 0.5;
		}
		else {
			camera->fade = 0.0;
		}
		
		camera->position = Vec3f(scene->getBoxWidth()/2 - 10, 
		                        -scene->getBoxHeight()/2 + 10, 
		                        scene->getBoxWidth()/2 - 10);
		sceneTimer.setScale(0.25f, time);
	}
	else if (t < 31.0) {
		camera->blackBars = true;
		if (t < 16.5) {
			camera->fade = 1 - (t - 16.0) / 0.5;
		}
		else if (t > 30.5) {
			camera->fade = (t - 30.5) / 0.5;
		}
		else {
			camera->fade = 0.0;
		}
		
		camera->position = Vec3f(-scene->getBoxWidth()/2 + 10, 
		                        +scene->getBoxHeight()/2 - 10, 
		                        -scene->getBoxWidth()/2 + 10);
		sceneTimer.setScale(0.25f, time);
	}
	else if (t < 31.5) {
		camera->position = defaultCameraPosition;
		camera->fade = 1.0;
		camera->blackBars = true;
		sceneTimer.setScale(0.25f, time);
	}
	else if (t < 32.0) {
		camera->fade = 1 - (t - 31.5) / 0.5;
		sceneTimer.setScale(1.0, time);
		camera->blackBars = false;
		camera->position = defaultCameraPosition;
	}
	else {
		camera->blackBars = false;
		camera->fade = 0.0;
		camera->position = defaultCameraPosition;
		sceneTimer.setScale(1.0, time);
		
		nextEffect = time + frand(120.0, 180.0);
		state = S_NORMAL;
		stateStart = time;
	}
}


void Cometen3::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	gluLookAt(camera->position[0],camera->position[1],camera->position[2],
	          camera->viewpoint[0],camera->viewpoint[1],camera->viewpoint[2],
	          0.0, 1.0, 0.0);
	
	scene->render(camera->position);
	
	glPopMatrix();
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, 640.0, 0.0, 480.0, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);
	
	if (camera->blackBars) {
		glDisable(GL_BLEND);
		glColor4f(0.0, 0.0, 0.0, 1.0);
		double bar_height = 640.0 * 3 / 32;
		glBegin(GL_QUADS);
		glVertex2f(0.0, 0.0);
		glVertex2f(0.0, bar_height);
		glVertex2f(640.0, bar_height);
		glVertex2f(640.0, 0.0);
		glVertex2f(0.0, 480.0);
		glVertex2f(0.0, 480.0-bar_height);
		glVertex2f(640.0, 480.0-bar_height);
		glVertex2f(640.0, 480.0);
		glEnd();
	}
	
	if (camera->fade != 0.0) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glEnable(GL_BLEND);
		glColor4f(1.0, 1.0, 1.0, camera->fade);
		glBegin(GL_QUADS);
		glVertex2f(0.0, 0.0);
		glVertex2f(0.0, 480.0);
		glVertex2f(640.0, 480.0);
		glVertex2f(640.0, 0.0);
		glEnd();
		
	}

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}
