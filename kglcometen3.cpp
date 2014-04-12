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

#include "kglcometen3.h"
#include "cometenmath.h"

#include <QSettings>

KGLCometen3::KGLCometen3(QWidget *parent)
: QGLWidget(parent)
{
	cometen3 = new Cometen3();
	settings = new Setting[SE_NUM];
	textures = new GLuint[T_NUM];
}

KGLCometen3::~KGLCometen3() {
	delete cometen3;
	delete settings;
	delete textures;
}

void KGLCometen3::start() 
{
	QSettings *conf = new QSettings("QCometen3");
	if(conf->allKeys().empty() && QFile::exists(QDir::homePath()+"/.kde/share/config/kcometen3.kssrc")) {
		delete conf;
		conf = new QSettings(QDir::homePath()+"/.kde/share/config/kcometen3.kssrc", QSettings::IniFormat);
	}
	conf->beginGroup("Settings");
	settings[SE_FREECAMERA].intValue = conf->value("freecamera", true).toBool();
	settings[SE_FREECAMERASPEED].doubleValue = clamp<double>(conf->value("freecamspeed", 1.5).toDouble(), 0.0, 10.0);
	settings[SE_DOBLITZ].intValue = conf->value("lightning", true).toBool();
	settings[SE_DOSPLITCOMET].intValue = conf->value("splitcomet", true).toBool();
	settings[SE_DOROTATECOMET].intValue = conf->value("rotatecomet", true).toBool();
	settings[SE_DOCURVECOMET].intValue = conf->value("curvecomet", true).toBool();
	settings[SE_DOMATRIX].intValue = conf->value("matrix", true).toBool();
	settings[SE_DOSLOWMOTION].intValue = conf->value("slowmotion", true).toBool();
	settings[SE_CREATEINTERVAL].doubleValue = clamp<double>(conf->value("interval", 1.25).toDouble(), 0.1, 10.0);
	settings[SE_TIMESCALE].doubleValue = clamp<double>(conf->value("timescale", 1.0).toDouble(), 0.1, 5.0);
	settings[SE_SIZESCALE].doubleValue = clamp<double>(conf->value("sizescale", 2.5).toDouble(), 0.5, 5.0);
	
	settings[SE_COLORS].intValue = conf->value("color", 0).toInt();
	
	bgtype = conf->value("bgtype", 0).toInt();
	bgfile = conf->value("bgfile", "").toString().replace("$HOME", QDir::homePath());
	bgsize = conf->value("bgsize", 2).toInt();
	bgdir = conf->value("bgdir", "").toString().replace("$HOME", QDir::homePath());
	maxfps = clamp<int>(conf->value("maxfps", 999).toInt(), 10, 999);

	delete conf;
	startTimer(1000/maxfps);
}

void KGLCometen3::initializeGL() {
	QImage image;
	
	srand(time(NULL));

	if (bgtype == 2) {
		QString file = randomFileFromDir(bgdir);
		if (!image.load(file)) {
			image = QPixmap::grabWindow(QApplication::desktop()->winId()).toImage();
		}
	}
	else if (bgtype == 1) {
		if (!image.load(bgfile)) {
			image = QPixmap::grabWindow(QApplication::desktop()->winId()).toImage();
		}
	}
	else {
		image = QPixmap::grabWindow(QApplication::desktop()->winId()).toImage();
	}
	
	switch (bgsize) {
	case 0:
		image = image.scaled(QSize(128, 128), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		break;
	case 1:
		image = image.scaled(QSize(256, 256), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		break;
	case 2:
		image = image.scaled(QSize(512, 512), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		break;
	case 3:
	default:
		break;
	}
		
	image = QGLWidget::convertToGLFormat(image);
	
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &textures[T_BACKGROUND]);
	glBindTexture(GL_TEXTURE_2D, textures[T_BACKGROUND]); 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image.width(), image.height(),
	             0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());	
	
	textures[T_PARTICLE] = loadTexture("particle.png");
	textures[T_LIGHTMAP] = loadTexture("lightmap.png");
	
	t.start();
	settings[SE_DOMATRIX].intValue = 1;
	cometen3->init(settings, textures, 640, 480, t.elapsed() / 1000.0);
}

void KGLCometen3::resizeGL(int res_x, int res_y) {
	cometen3->resize(res_x, res_y);
}

void KGLCometen3::paintGL() {
	cometen3->render();
}

void KGLCometen3::timerEvent(QTimerEvent * event) {
	cometen3->process(t.elapsed() / 1000.0);
	updateGL();
}

QString KGLCometen3::randomFileFromDir(const QString& dirname) {
	QDir dir(dirname, "*.jpg;*.jpeg;*.png;*.gif",
	         QDir::Unsorted, QDir::Files | QDir::Readable);
	
	if (dir.count() < 1) {
		return QString("");
	}
	
	int num = rand() % dir.count();
	
	return dir.absolutePath() + "/" + dir[num];
}

GLuint KGLCometen3::loadTexture(const char* file) {
	QImage image;
	GLuint tex = 0;

	if (image.load(QString(":/") + file)) {
		image = QGLWidget::convertToGLFormat(image);
		
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex); 
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image.width(), image.height(),
		             0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
	}

	return tex;
}
		
