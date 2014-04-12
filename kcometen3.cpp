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

#include <stdlib.h>
#include "kcometen3.h"
#include <QApplication>

//! libkscreensaver interface
extern "C"
{
    const char *kss_applicationName = "kcometen3.kss";
    const char *kss_version = "2.2.0";

    KCometen3 *kss_create( WId id )
    {
        return new KCometen3( id );
    }
}

KCometen3::KCometen3( WId id ) : KScreenSaver( id )
{
	kglcometen3 = new KGLCometen3();
	
	kglcometen3->start();
	
	embed(kglcometen3);
	

	kglcometen3->show();
}

KCometen3::~KCometen3()
{
	delete kglcometen3;
}

