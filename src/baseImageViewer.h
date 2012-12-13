#ifndef BASEIMAGEVIEWER_H
#define BASEIMAGEVIEWER_H

#include <iostream>
#include <QScrollArea>
#include <QtGui>
#include <QPrinter>
#include <Matriz/TCMatriz2D.h>
#include <Matriz/TCImagem3D.h>

class QLabel;
class QListWidgetItem;
class QImage;
class QSignalMapper;
class QMainWindow;

/*! BaseImageViewer é a classe base para representar imagens.*/
class BaseImageViewer : public QScrollArea
{
		// Construtor / Destrutor
	public:
		BaseImageViewer( QMainWindow * _parent );
		virtual ~ BaseImageViewer( ) {
			//std::cerr << "virtual ~ BaseImageViewer( ) "<< std::endl;
			//if (pm) delete pm;
			//if (pm3D) delete pm3D;
			if (lbX) delete lbX;
			if (lbY) delete lbY;
			if (image) delete image;
			if (lbRGB) delete lbRGB;
			if (lbXName) delete lbXName;
			if (lbYName) delete lbYName;
			if (lbRGBName) delete lbRGBName;
			if (imageLabel) delete imageLabel;
		}

		// Atributos
	public:
		double scaleFactor;
		bool isFitedToWindow;
		bool isNew; //true se for uma nova imagem.
		QLabel *imageLabel;
		//TCMatriz2D<int> * pm;
		//TCImagem3D<int> * pm3D;
		QListWidgetItem *item;
		int nx;
		int ny;

	protected:
		QImage *image;
		QMainWindow *parent;
		QString fullFileName;
		QString curFile;
		QString curFileNoExt;
		QString fileExt;
		QString filePath;

#ifndef QT_NO_PRINTER
		QPrinter printer;
#endif

	private:
		QLabel *lbX, *lbXName;
		QLabel *lbY, *lbYName;
		QLabel *lbRGB, *lbRGBName;

		// Métodos
	public:
		virtual bool loadFile(const QString &fileName) = 0;
		bool loadImage(TCMatriz2D<bool> *&_pm);
		bool loadImage(TCMatriz2D<int>* &_pm);
		virtual bool save() = 0;
		virtual bool saveAs() = 0;
		inline QString getFullFileName() { return fullFileName; }
		inline QString getFileName() { return curFile; }
		inline QString getFileNameNoExt() { return curFileNoExt; }
		inline QString getFileExt() { return fileExt; }
		inline QString getFilePath() { return filePath; }
		inline double ScaleFactor() { return scaleFactor; }
		void print();
		void zoomIn();
		void zoomOut();
		void normalSize();
		void createStatusBar();
		void destroyStatusBar();

	protected:
		void adjustScrollBar(QScrollBar *scrollBar, double factor);
		void mouseMoveEvent(QMouseEvent *event);
		void mousePressEvent(QMouseEvent *event);
		void getFileNames(const QString & _fileName);
		void scaleImage(double factor);
		bool maybeSave();
};

#endif
