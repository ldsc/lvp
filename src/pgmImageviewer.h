#ifndef PGMIMAGEVIEWER_H
#define PGMIMAGEVIEWER_H

#ifndef BASEPNMIMAGEVIEWER_H
#include "basePnmImageviewer.h"
#endif

class PgmImageViewer : public BasePnmImageViewer
{
	Q_OBJECT
	// Construtor / Destrutor
public:
	PgmImageViewer( QMainWindow * _parent );
	virtual ~ PgmImageViewer( ) {
		if (pm) delete pm;
	}

	//Atributos
	TCMatriz2D<int> * pm;

	//Métodos
public:
	virtual bool save();
	virtual bool saveAs();

};

#endif
