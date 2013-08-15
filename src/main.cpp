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
#include "omp.h"

int main(int argc, char *argv[]) {
	Q_INIT_RESOURCE(application); //inicia os recursos especificados no arquivo .qrc
	QApplication app(argc, argv); //
	app.setApplicationName("LVP");
	app.setOrganizationName("LENEP");
	
	QPixmap pixmap(":/images/splashScream.png");
	QSplashScreen splash(pixmap);
	splash.show();
	Qt::Alignment BottomRight = Qt::AlignBottom | Qt::AlignRight;
	Qt::Alignment BottomLeft = Qt::AlignBottom | Qt::AlignLeft;
	splash.showMessage(QObject::tr("Working on %1 cores.").arg(omp_get_num_procs()), BottomLeft, Qt::white);
	splash.showMessage(QObject::tr("Loading Main Window"), BottomRight, Qt::white);

	// Identify locale and load translation if available
	QString locale = QLocale::system().name();
	QTranslator translator;
	//translator.load("lvp/src/lvp_pt");
	translator.load(QString("lvp_") + locale);
	app.installTranslator(&translator);

	//sleep(1);
	splash.showMessage(QObject::tr("Main Window Loaded."), BottomRight, Qt::white);
	//sleep(1);

	Lvp *mw = new Lvp();
	mw->show();
	if (argc > 1)
		for (int i=1; i < argc; i++)
			mw->open(argv[i], false);

	std::cout << "LVP: " << omp_get_num_procs() << " núcleo(s) de processamento disponível(is)!" << endl;
	splash.finish(mw);
	return app.exec();
}

