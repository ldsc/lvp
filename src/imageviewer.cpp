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
		if( ! pm ) pm = new TCMatriz2D<int>(fullFileName.toStdString());
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
	if ( ! pm ) {
		pm = new TCMatriz2D<int>(fullFileName.toStdString());
	}
	if ( ! pm ) {
		QMessageBox::information(parent, tr("LVP"), tr("Error! - Can't create image."));
		return false;
	}
	//
	QMessageBox msgBox(this);
	msgBox.setWindowTitle(tr("Save As"));
	msgBox.setText(tr("Saving Format:"));
	QPushButton *cancelButton = msgBox.addButton(QMessageBox::Cancel);
	msgBox.addButton(tr("&Binary"), QMessageBox::ActionRole);
	QPushButton *asciiButton = msgBox.addButton(tr("&ASCII"), QMessageBox::ActionRole);
	msgBox.setDefaultButton(asciiButton);
	msgBox.exec();
	if ( msgBox.clickedButton() == cancelButton )
		return false;
	if (msgBox.clickedButton() == asciiButton) { //ascii
		switch(pm->GetFormato()){ //se o formato atual for binário muda para ascii mantendo o número de cores
			case P4_X_Y_BINARY:
				pm->SetFormato(P1_X_Y_ASCII);
				break;
			case P5_X_Y_GRAY_BINARY:
				pm->SetFormato(P2_X_Y_GRAY_ASCII);
				break;
			case P6_X_Y_COLOR_BINARY:
				pm->SetFormato(P3_X_Y_COLOR_ASCII);
				break;
			default: break; //evitar warming do compilador
		}
	} else { //binario.
		switch(pm->GetFormato()){ //se o formato atual for ascii muda para binário mantendo o número de cores
			case P1_X_Y_ASCII:
				pm->SetFormato(P4_X_Y_BINARY);
				break;
			case P2_X_Y_GRAY_ASCII:
				pm->SetFormato(P5_X_Y_GRAY_BINARY);
				break;
			case P3_X_Y_COLOR_ASCII:
				pm->SetFormato(P6_X_Y_COLOR_BINARY);
				break;
			default: break; //evitar warming do compilador
		}
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
