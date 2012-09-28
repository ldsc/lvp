#ifndef BASEPNMIMAGEVIEWER_H
#define BASEPNMIMAGEVIEWER_H

#ifndef BASEIMAGEVIEWER_H
#include "baseImageViewer.h"
#endif

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
	virtual bool loadFile(const QString &fileName);
	virtual bool save() = 0;
	virtual bool saveAs() = 0;

protected:
	void closeEvent(QCloseEvent *event);
};

#endif
