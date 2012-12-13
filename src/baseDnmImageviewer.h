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

	protected:
		QString curPlanoFile;

		//Métodos
	public:
		virtual bool loadFile(const QString &fileName) = 0;
		virtual bool save() = 0;
		virtual bool saveAs() = 0;
		inline QString currentPlanoFile() { return curPlanoFile; }

	protected:
		void closeEvent(QCloseEvent *event);
};

#endif
