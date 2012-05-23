#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#ifndef baseImageViewer_h
#include "baseImageViewer.h"
#endif

class ImageViewer : public BaseImageViewer
{
	Q_OBJECT
	// Construtor / Destrutor
public:
	ImageViewer( QMainWindow * _parent );
	virtual ~ ImageViewer( ) { }

	//Atributos

	//Métodos
public:
	virtual bool loadFile(const QString &fileName);
	virtual bool save();
	virtual bool saveAs();

protected:
	void closeEvent(QCloseEvent *event);
};

#endif
