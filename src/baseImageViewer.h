#ifndef BASEIMAGEVIEWER_H
#define BASEIMAGEVIEWER_H

#include <iostream>
#include <QScrollArea>
#include <QtGui>
#include <QPrinter>
#include <Matriz/TCMatriz2D.h>
#include <Matriz/CImagem3D.h>

class QLabel;
class QListWidgetItem;
class QImage;
class QSignalMapper;
class QMainWindow;

class BaseImageViewer : public QScrollArea
{
	// Construtor / Destrutor
public:
	BaseImageViewer( QMainWindow * _parent );
	virtual ~ BaseImageViewer( )
	{
		//std::cerr << "virtual ~ BaseImageViewer( ) "<< std::endl;
		if (pm) delete pm;
		if (pm3D) delete pm3D;
		if (image) delete image;
		if (imageLabel) delete imageLabel;
	}

	// Atributos
public:
	double scaleFactor;
	bool isFitedToWindow;
	bool isNew; //true se for uma nova imagem.
	QLabel *imageLabel;
	TCMatriz2D<int> * pm;
	CImagem3D * pm3D;
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

	// Métodos
public:
	virtual bool loadFile(const QString &fileName) = 0;
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

protected:
	void adjustScrollBar(QScrollBar *scrollBar, double factor);
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
   void getFileNames(const QString & _fileName);
	void scaleImage(double factor);
	bool maybeSave();
};

#endif
