#ifndef IMPORT_H
#define IMPORT_H

#include <QtGui>
#include "ui_dialogImport.h"

class QMainWindow;

class Import : public QDialog, public Ui::DialogImport
{
		Q_OBJECT

	public:
		Import( QMainWindow * _parent, QString _file );

		~Import();

		QString fullFileName;

	private:
		QMainWindow *parent;
};

#endif
