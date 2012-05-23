#include "imageviewer.h"

ImageViewer::ImageViewer(QMainWindow * _parent) : BaseImageViewer( _parent )
{
	//setAttribute(Qt::WA_DeleteOnClose);
	//setBackgroundRole(QPalette::Shadow);
}

bool ImageViewer::loadFile(const QString &fileName)
{
	if (!fileName.isEmpty()) {
		image = new QImage(fileName);
		if (image->isNull()) {
			QMessageBox::information(parent, tr("LVP"), tr("Error! - Cannot load %1.").arg(fileName));
			return false;
		}
		QApplication::setOverrideCursor(Qt::WaitCursor);
		imageLabel->setPixmap(QPixmap::fromImage(*image));
		QApplication::restoreOverrideCursor();
		getFileNames(fileName);
		setWindowTitle(getFileName());
		return true;
	}else{
		return false;
	}
}

bool ImageViewer::save()
{
	if (isNew) {
		return saveAs();
	} else {
		//return image->save(curFile);
		if( ! pm ) pm = new CMatriz2D(fullFileName.toStdString());
		if(pm->Write(curFile.toStdString())) {
			return true;
		} else {
			return false;
		}
	}
}

bool ImageViewer::saveAs() {
   QString fileName;
   if (isNew){
      fileName = filePath + curFile;
   } else {
      fileName = fullFileName;
   }
   fileName = QFileDialog::getSaveFileName(parent, tr("Save As"), fileName, tr("Images (*.pbm *.pgm)"));
	if (fileName.isEmpty()){
      return false;
	}
	if ( ! pm ) pm = new CMatriz2D(fullFileName.toStdString());
	if ( ! pm ) {
		QMessageBox::information(parent, tr("LVP"), tr("Error! - Can't create image."));
		return false;
	}
	string tmp = pm->Path();
	pm->Path("");
	if( pm->Write( fileName.toStdString() ) ) {
		if ( isNew && (fileName != fullFileName) ) {
			QFile::remove(fullFileName);
		}
		isNew = false;
		loadFile(fileName);
		return true;
	}else{
		pm->Path(tmp);
		QMessageBox::information(parent, tr("LVP"), tr("Error! - Can't save file \"%1\"").arg(fileName));
		return false;
	}
}

void ImageViewer::closeEvent(QCloseEvent *event)
{
	if (maybeSave()) {
		if(isNew){
			QFile::remove(fullFileName);
		}
		event->accept();
	} else {
		event->ignore();
	}
}
