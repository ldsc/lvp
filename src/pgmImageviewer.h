#ifndef PGMIMAGEVIEWER_H
#define PGMIMAGEVIEWER_H

#ifndef BASEPNMIMAGEVIEWER_H
#include "basePnmImageviewer.h"
#endif

/*! PgmImageViewer é a classe que representa imagens bidimensionais em tons de cinza.*/
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
	virtual bool loadFile(const QString &fileName);
	virtual void reloadFile();
	virtual bool save();
	virtual bool saveAs();
	inline virtual TCMatriz2D<bool> * getPMBool() { return nullptr; }
	inline virtual TCMatriz2D<int> * getPMInt() { return pm; }


};

#endif
