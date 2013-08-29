#ifndef DGMIMAGEVIEWER_H
#define DGMIMAGEVIEWER_H

#ifndef BASEDNMIMAGEVIEWER_H
#include "baseDnmImageviewer.h"
#endif

#include <QMessageBox>

template <typename T> class TCMatriz3D;

/*! DgmImageViewer é a classe que representa imagens tridimensionais em tons de cinza.*/
class DgmImageViewer : public BaseDnmImageViewer
{
		Q_OBJECT
		// Construtor / Destrutor
	public:
		DgmImageViewer ( QMainWindow * _parent );
		virtual ~ DgmImageViewer ( ) {
			if (pm) delete pm;
			if (pm3D) delete pm3D;
		}

		// Atributos
	public:
		TCMatriz2D<int> * pm;
		TCImagem3D<int> * pm3D;

		//Métodos
	public:
		virtual bool loadFile(const QString &fileName);
		virtual bool save();
		virtual bool saveAs();
		bool ChangePlan( unsigned int plano, E_eixo axis );

};

#endif
