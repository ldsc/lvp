#ifndef DBMIMAGEVIEWER_H
#define DBMIMAGEVIEWER_H

#ifndef BASEDNMIMAGEVIEWER_H
#include "baseDnmImageviewer.h"
#endif

class DbmImageViewer : public BaseDnmImageViewer
{
		Q_OBJECT
		// Construtor / Destrutor
	public:
		DbmImageViewer ( QMainWindow * _parent );
		virtual ~ DbmImageViewer ( ) {
			if (pm) delete pm;
			if (pm3D) delete pm3D;
		}

		// Atributos
	public:
		TCMatriz2D<bool> * pm;
		TCImagem3D<bool> * pm3D;

		//Métodos
	public:
		virtual bool loadFile(const QString &fileName);
		virtual bool save();
		virtual bool saveAs();
		bool ChangePlan( unsigned int plano, E_eixo axis );

};

#endif
