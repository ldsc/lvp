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
	createStatusBar();
}

bool BaseImageViewer::loadImage(TCMatriz2D<bool> *&_pm) {
	if ( _pm ) {
		int nx = _pm->NX();
		int ny = _pm->NY();
		image = new QImage(nx, ny, QImage::Format_Mono); //QImage::Format_MonoLSB
		if (image->isNull()) {
			QMessageBox::information(parent, tr("LVP"), tr("Error! - Cannot create image"));
			return false;
		}
		QApplication::setOverrideCursor(Qt::WaitCursor);
		for (int i=0; i<nx; i++){
			for (int j=0; j<ny; j++){
				image->setPixel(i, j, _pm->data2D[i][j]);
			}
		}
		image->invertPixels();
		imageLabel->setPixmap(QPixmap::fromImage(*image));
		QApplication::restoreOverrideCursor();
		static int seqNumberLoadImage = 1;
		QString fname = getFileName();
		if (fname.isEmpty()){
			fname = tr("untitled-%1.pbm").arg(QString::number(seqNumberLoadImage++));
			getFileNames(fname);
		}
		setWindowTitle(fname);
		return true;
	} else {
		return false;
	}
}

bool BaseImageViewer::loadImage(TCMatriz2D<int> *&_pm) {
	if ( _pm ) {
		int nx = _pm->NX();
		int ny = _pm->NY();
		image = new QImage(nx, ny, QImage::Format_Indexed8); //QImage::Format_MonoLSB
		if (image->isNull()) {
			QMessageBox::information(parent, tr("LVP"), tr("Error! - Cannot create image"));
			return false;
		}
		QApplication::setOverrideCursor(Qt::WaitCursor);
		for (int i=0; i<nx; i++){
			for (int j=0; j<ny; j++){
				image->setPixel(i, j, _pm->data2D[i][j]);
			}
		}
		image->invertPixels();
		imageLabel->setPixmap(QPixmap::fromImage(*image));
		QApplication::restoreOverrideCursor();
		static int seqNumberLoadImage = 1;
		QString fname = getFileName();
		if (fname.isEmpty()){
			fname = tr("untitled-%1.pbm").arg(QString::number(seqNumberLoadImage++));
			getFileNames(fname);
		}
		setWindowTitle(fname);
		return true;
	} else {
		return false;
	}
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

void BaseImageViewer::mouseMoveEvent(QMouseEvent *event) {
	if (event->x() >= 0 && event->y() >= 0 && event->x() < image->width() && event->y() < image->height() && scaleFactor == 1.0 && !isFitedToWindow) {
		int gray = qGray(image->pixel( event->pos() ));
		parent->statusBar()->showMessage(tr("Coordinates (x = %1, y = %2) - RGB (%3,%4,%5)").arg(event->x()).arg(event->y()).arg(gray).arg(gray).arg(gray), 2000);
		//QToolTip::showText(event->pos(), tr("Coordinates"));
		lbX->setText(QString("%1").arg(event->x()));
		lbY->setText(QString("%1").arg(event->y()));
		lbRGB->setText(QString("%1,%2,%3").arg(gray).arg(gray).arg(gray));
	}
}

void BaseImageViewer::mousePressEvent(QMouseEvent *event) {
	event = event; // evitar warning
	setMouseTracking( ! hasMouseTracking () );
}

bool BaseImageViewer::maybeSave() {
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

void BaseImageViewer::createStatusBar() {
	lbXName = new QLabel();
	lbXName->setText(tr("Coordinates: X ="));

	lbX = new QLabel();
	lbX->setFrameShape(QFrame::Panel);
	lbX->setFrameShadow(QFrame::Sunken);
	lbX->setMinimumWidth(30);

	lbYName = new QLabel();
	lbYName->setText(tr("Y ="));

	lbY = new QLabel();
	lbY->setFrameShape(QFrame::Panel);
	lbY->setFrameShadow(QFrame::Sunken);
	lbY->setMinimumWidth(30);

	lbRGBName = new QLabel();
	lbRGBName->setText(tr("RGB:"));

	lbRGB = new QLabel();
	lbRGB->setFrameShape(QFrame::Panel);
	lbRGB->setFrameShadow(QFrame::Sunken);
	lbRGB->setMinimumWidth(70);

	parent->statusBar()->addPermanentWidget(lbXName);
	parent->statusBar()->addPermanentWidget(lbX);
	parent->statusBar()->addPermanentWidget(lbYName);
	parent->statusBar()->addPermanentWidget(lbY);
	parent->statusBar()->addPermanentWidget(lbRGBName);
	parent->statusBar()->addPermanentWidget(lbRGB);
}

void BaseImageViewer::destroyStatusBar() {
	parent->statusBar()->removeWidget(lbXName);
	parent->statusBar()->removeWidget(lbX);
	parent->statusBar()->removeWidget(lbYName);
	parent->statusBar()->removeWidget(lbY);
	parent->statusBar()->removeWidget(lbRGBName);
	parent->statusBar()->removeWidget(lbRGB);
}
