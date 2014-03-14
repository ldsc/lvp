#ifndef BASEDNMIMAGEVIEWER_H
#define BASEDNMIMAGEVIEWER_H

#ifndef BASEIMAGEVIEWER_H
#include "baseImageViewer.h"
#endif

/*! BaseDnmImageViewer é a classe base para representar imagens tridimensionais.*/
class BaseDnmImageViewer : public BaseImageViewer
{
	Q_OBJECT
		// Construtor / Destrutor
	public:
		BaseDnmImageViewer( QMainWindow * _parent );
		virtual ~ BaseDnmImageViewer( ) { }

		//Atributos
	public:
		E_eixo direcao;
		int curPlan;
		int nz;
		bool isModified;

		//Métodos
	public:
		virtual bool loadFile(const QString &fileName) = 0;
		virtual void reloadFile() = 0;
		virtual bool save() = 0;
		virtual bool saveAs() = 0;
		virtual TCMatriz2D<bool> * getPMBool() = 0;
		virtual TCMatriz2D<int> * getPMInt() = 0;
		virtual TCImagem3D<bool> * getPI3DBool() = 0;
		virtual TCImagem3D<int> * getPI3DInt() = 0;

	protected:
		void closeEvent(QCloseEvent *event);
		void updateTitle();

	public slots:
		void documentWasModified();

	signals:
		void dataChanged();
};

#endif
