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

#ifndef KGLCOMETEN3_H
#define KGLCOMETEN3_H

#include <QDir>
#include <QtOpenGL>
#include <QImage>
#include <QApplication>
#include <QPixmap>
#include <QDateTime>
#include <QTimerEvent>
#include "cometen3.h"
#include "global.h"

/**
@author Peter Mueller
*/
class KGLCometen3 : public QGLWidget
{
Q_OBJECT
public:
    KGLCometen3(QWidget *parent = 0);
    virtual ~KGLCometen3();
	
	void start();
	
protected:
	void initializeGL();
	void resizeGL( int, int );
	void paintGL();
	void timerEvent( QTimerEvent * );
	
private:
	GLuint loadTexture(const char* file);
	QString randomFileFromDir(const QString& dir);
		
	Cometen3 * cometen3;
	Setting* settings;
	GLuint* textures;
	
	int bgtype;
	QString bgfile;
	QString bgdir;
	int bgsize;
	int maxfps;
	
	QTime t;
};

#endif
