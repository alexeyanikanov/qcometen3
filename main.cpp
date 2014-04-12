/* This file is part of the KDE libraries

    Copyright (c) 2001  Martin R. Jones <mjones@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#include <iostream>

#include <QKeyEvent>
#include <QCloseEvent>
#include <QX11Info>
#include <QApplication>
#include <QMessageBox>

#include "kscreensaver.h"
#include "kscreensaver_vroot.h"

extern "C"
{
    extern const char *kss_applicationName;
    extern const char *kss_description;
    extern const char *kss_version;
    KScreenSaver *kss_create( WId d );
}

//----------------------------------------------------------------------------

class DemoWindow : public QWidget
{
public:
    DemoWindow() : QWidget()
    {
	setFixedSize(600, 420);
    }

protected:
    virtual void keyPressEvent(QKeyEvent *e)
    {
        if (e->key() == Qt::Key_Q)
        {
		QApplication::instance()->quit();
        }
    }

    virtual void closeEvent( QCloseEvent * )
    {
	    QApplication::instance()->quit();
    }
};


//----------------------------------------------------------------------------
#if defined(Q_WS_QWS) || defined(Q_WS_MACX)
typedef WId Window;
#endif

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    DemoWindow *demoWidget = 0;
    Window saveWin = 0;
    KScreenSaver *target;

    for(int i = 0; i < QApplication::arguments().count(); i++) {
	    QString a = QApplication::arguments().at(i);
	    if(a == "--root" || a == "-root") {
		saveWin = RootWindow(QX11Info::display(), QX11Info::appScreen());
	    }
	    if(a == "--demo" || a == "-demo") {
		saveWin = 0;
	    }
	    if(a == "--window-id" || a == "-window-id") {
		if(i+1 >= QApplication::arguments().count()) {
		    std::cerr << "--window-id requires a parameter" << std::endl;
		    return 1;
		}
		else {
		    saveWin = QApplication::arguments().at(i+1).toInt();
		}
	    }
	    if(a == "--setup" || a == "-setup") {
		QMessageBox::critical(0, "Setup not supported", "This version of QCometen3 doesn't have a setup dialog.");
		return 1;
	    }
	    if(a == "--help" || a == "-help") {
		std::cerr << "options: [-demo] [-root] [-window-id ID]" << std::endl;
		return 1;
	    }
    }

    if (saveWin == 0)
    {
        demoWidget = new DemoWindow();
        demoWidget->setAutoFillBackground(false);
        saveWin = demoWidget->winId();
        app.processEvents();
    }

    target = kss_create( saveWin );

    if ( demoWidget )
    {
        demoWidget->setFixedSize( 600, 420 );
        demoWidget->show();
    }
    app.exec();

    delete target;
    delete demoWidget;

    return 0;
}

