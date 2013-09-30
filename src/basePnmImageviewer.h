#ifndef BASEPNMIMAGEVIEWER_H
#define BASEPNMIMAGEVIEWER_H

#ifndef BASEIMAGEVIEWER_H
#include "baseImageViewer.h"
#endif

/*! BasePnmImageViewer é a classe base para representar imagens bidimensionais.*/
class BasePnmImageViewer : public BaseImageViewer
{
	Q_OBJECT
	// Construtor / Destrutor
public:
	BasePnmImageViewer( QMainWindow * _parent );
	virtual ~ BasePnmImageViewer( ) { }

	//Atributos

	//Métodos
public:
	//virtual bool loadFile(const QString &fileName);
	virtual bool loadFile(const QString &fileName) = 0;
	virtual void reloadFile() = 0;
	virtual bool save() = 0;
	virtual bool saveAs() = 0;
	virtual TCMatriz2D<bool> * getPMBool() = 0;
	virtual TCMatriz2D<int> * getPMInt() = 0;

protected:
	void closeEvent(QCloseEvent *event);
};

#endif
