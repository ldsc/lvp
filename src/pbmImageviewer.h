#ifndef PBMIMAGEVIEWER_H
#define PBMIMAGEVIEWER_H

#ifndef BASEPNMIMAGEVIEWER_H
#include "basePnmImageviewer.h"
#endif

/*! PbmImageViewer é a classe que representa imagens bidimensionais binárias (preto e branco).*/
class PbmImageViewer : public BasePnmImageViewer
{
	Q_OBJECT
	// Construtor / Destrutor
public:
	PbmImageViewer( QMainWindow * _parent );
	virtual ~ PbmImageViewer( ) {
		if (pm) delete pm;
	}

	//Atributos
	TCMatriz2D<bool> * pm;

	//Métodos
public:
	virtual bool loadFile(const QString &fileName);
	virtual bool save();
	virtual bool saveAs();

};

#endif
