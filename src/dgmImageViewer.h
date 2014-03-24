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
		virtual void reloadFile();
		virtual bool save();
		virtual bool saveAs();
		inline virtual TCMatriz2D<bool> * getPMBool() { return nullptr; }
		inline virtual TCMatriz2D<int> * getPMInt() { return pm; }
		inline virtual TCImagem3D<bool> * getPI3DBool() { return nullptr; }
		inline virtual TCImagem3D<int> * getPI3DInt(){ return pm3D; }


		bool ChangePlan( unsigned int plano, E_eixo axis );

};

#endif
