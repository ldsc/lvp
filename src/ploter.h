#ifndef PLOTER_H
#define PLOTER_H

#include <QtGui>
#include <QScrollArea>
#include <QList>
#include <iostream>
#include <fstream>
#include "qwt_plot.h"
#include "qwt_plot_curve.h"
#include "qwt_symbol.h"
#include "qwt_legend.h"
#include "qwt_plot_picker.h"
#include "qwt_plot_dict.h"
//#include "qwt_plot_zoomer.h"

class QMainWindow;
class QListWidgetItem;

/*! Ploter é a classe que representa a visualização de gráficos através do QwtPlot.*/
class Ploter : public QwtPlot //public QScrollArea
{
		Q_OBJECT
		// Construtor/Destrutor
	public:
		Ploter( QMainWindow * _parent );

		virtual ~Ploter(){}

		// Atributos
	public:
		bool isNew; //true se for um novo gráfico.
		QStringList pathCurves;
		QListWidgetItem *item;

	private:
		QMainWindow *parent;
		//Lvp *parent;
		QString fullFileName;
		QString fileName;
		QString curFileNoExt;
		QString fileExt;
		QString filePath;
		QList<Qt::GlobalColor> colors;
		QwtPlotPicker * position;
		QwtLegend *legend;
		bool isAccumulated;
		QwtPlotCurve *curveAccumulated;

		// Métodos
	public:
		bool addCurves( const QStringList & _filesName );
		bool loadFile( const QString & _fileName );
		void reloadFile();
		bool save( QString * lop );
		bool saveAs( QString * lop );

		inline QString getFullFileName() { return fullFileName; }
		inline QString getFileName() { return fileName; }
		inline QString getFileNameNoExt() { return curFileNoExt;	}
		inline QString getFileExt() { return fileExt; }
		inline QString getFilePath() { return filePath; }
		inline bool IsAccumulated() { return isAccumulated; }

	public slots:
		void labelPos(const QPoint &pos);
		void accumulated();

	protected:
		void closeEvent(QCloseEvent *event);

	private:
		bool maybeSave();
		void getFileNames( const QString &fileName );
};

#endif
