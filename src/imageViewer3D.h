#ifndef IMAGEVIEWER3D_H
#define IMAGEVIEWER3D_H

#ifndef baseImageViewer_h
#include "baseImageViewer.h"
#endif

template <typename T> class TCMatriz3D;

class ImageViewer3D : public BaseImageViewer
{
	Q_OBJECT
	// Construtor / Destrutor
public:
	ImageViewer3D ( QMainWindow * _parent );
	virtual ~ ImageViewer3D ( ) { }

	// Atributos
public:
	int curPlan;
	int nz;
	TCMatriz3D<int>::E_eixo direcao;

private:
	QString curPlanoFile;

	//Métodos
public:
	virtual bool loadFile(const QString &fileName);
	virtual bool save();
	virtual bool saveAs();
	inline QString currentPlanoFile() { return curPlanoFile; }
	bool ChangePlan( unsigned int plano, TCMatriz3D<int>::E_eixo axis );

protected:
	void closeEvent(QCloseEvent *event);
};

#endif
