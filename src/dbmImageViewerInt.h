#ifndef DBMIMAGEVIEWERINT_H
#define DBMIMAGEVIEWERINT_H

#ifndef BASEDNMIMAGEVIEWER_H
#include "baseDnmImageviewer.h"
#endif

class DbmImageViewerInt : public BaseDnmImageViewer
{
		Q_OBJECT
		// Construtor / Destrutor
	public:
		DbmImageViewerInt ( QMainWindow * _parent );
		virtual ~ DbmImageViewerInt ( ) {
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
