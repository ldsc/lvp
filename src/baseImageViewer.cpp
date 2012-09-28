#include "baseImageViewer.h"

BaseImageViewer::BaseImageViewer( QMainWindow * _parent ): /*pm(NULL), pm3D(NULL),*/ item(NULL), image(NULL), parent(_parent) {
	setAttribute(Qt::WA_DeleteOnClose);
	setBackgroundRole(QPalette::Shadow);
	imageLabel = new QLabel;
	imageLabel->setBackgroundRole(QPalette::Base);
	imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	imageLabel->setScaledContents(true);
	setWidget(imageLabel);
	scaleFactor = 1.0;
	isFitedToWindow = false;
	isNew = false;
	connect(this, SIGNAL(destroyed()), parent, SLOT(updateDockLista()));
}

void BaseImageViewer::zoomIn()
{
	  scaleImage(1.25);
}

void BaseImageViewer::zoomOut()
{
	scaleImage(0.8);
}

void BaseImageViewer::normalSize()
{
	imageLabel->adjustSize();
	scaleFactor = 1.0;
}

void BaseImageViewer::scaleImage(double factor)
{
	 Q_ASSERT(imageLabel->pixmap());
	 scaleFactor *= factor;
	 imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

	 adjustScrollBar(horizontalScrollBar(), factor);
	 adjustScrollBar(verticalScrollBar(), factor);
}

void BaseImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
	 scrollBar->setValue(int(factor * scrollBar->value() + ((factor - 1) * scrollBar->pageStep()/2)));
}

void BaseImageViewer::print()
{
	Q_ASSERT(imageLabel->pixmap());
#ifndef QT_NO_PRINTER
	QPrintDialog dialog(&printer, this);
	if (dialog.exec()) {
		QPainter painter(&printer);
		QRect rect = painter.viewport();
		QSize size = imageLabel->pixmap()->size();
		size.scale(rect.size(), Qt::KeepAspectRatio);
		painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
		painter.setWindow(imageLabel->pixmap()->rect());
		painter.drawPixmap(0, 0, *imageLabel->pixmap());
	}
#endif
}

void BaseImageViewer::getFileNames(const QString & _fileName) {
   fullFileName = _fileName;
   curFile = QFileInfo(fullFileName).fileName(); 	// retorna a string existente após a última /
   curFileNoExt = curFile.section('.', 0, -2); 		// retorna o nome do arquivo sem a extensão
   fileExt = QFileInfo(fullFileName).suffix(); 	// retorna a string existente após o último .
   filePath = QFileInfo(fullFileName).canonicalPath() + "/";
	if (isNew) {  									// é um novo arquivo.
		curFile = curFile.remove(0,1); 				// remove o ponto no início do nome do arquivo
	}
}

void BaseImageViewer::mouseMoveEvent(QMouseEvent *event)
{
	if (event->x() >= 0 && event->y() >= 0 && event->x() < image->width() && event->y() < image->height() && scaleFactor == 1.0 && !isFitedToWindow) {
		int gray = qGray(image->pixel( event->pos() ));
		parent->statusBar()->showMessage(tr("Coordinates (x = %1, y = %2) - RGB (%3,%4,%5)").arg(event->x()).arg(event->y()).arg(gray).arg(gray).arg(gray), 2000);
		//QToolTip::showText(event->pos(), tr("Coordinates"));
	}
}

void BaseImageViewer::mousePressEvent(QMouseEvent *event)
{
	event = event; // evitar warning
	setMouseTracking( ! hasMouseTracking () );
}

bool BaseImageViewer::maybeSave()
{
	if(isNew){
		QMessageBox::StandardButton ret;
		ret = QMessageBox::warning(parent, tr("LVP"), tr("'%1' isn't save.\n""Do you want to save it?").arg(getFileName()), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		if (ret == QMessageBox::Save)
			return save();
		else if (ret == QMessageBox::Cancel)
			return false;
	}
	return true;
}
