/***************************************************************************
 *   Copyright (C) 2009 by Leandro Puerari - puerari@gmail.com             *
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

#include <QApplication>
#include <QSplashScreen>
//#include <QTranslator>
#include "lvp.h"

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(application); //inicia os recursos especificados no arquivo .qrc
	QApplication app(argc, argv); //
	
    QPixmap pixmap(":/images/splashScream.png");
    QSplashScreen splash(pixmap);
    splash.show();
    Qt::Alignment topRight = Qt::AlignBottom | Qt::AlignRight;
    splash.showMessage(QObject::tr("Loading Main Window"), topRight, Qt::white);

    //QTranslator translator;
    //translator.load("lvp/src/lvp_pt");
    //app.installTranslator(&translator);

    sleep(1);
    splash.showMessage(QObject::tr("Main Window Loaded."), topRight, Qt::white);
    sleep(1);

	Lvp *mw = new Lvp();
	mw->show();
	if (argc > 1)
        for (int i=1; i < argc; i++)
            mw->open(argv[i], false);

    splash.finish(mw);
    return app.exec();
}

