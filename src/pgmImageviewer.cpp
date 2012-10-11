#include "pgmImageviewer.h"

PgmImageViewer::PgmImageViewer(QMainWindow * _parent) : BasePnmImageViewer( _parent ), pm(NULL) {
	//setAttribute(Qt::WA_DeleteOnClose);
	//setBackgroundRole(QPalette::Shadow);
}

//bool PgmImageViewer::loadFile(const QString &fileName) {
//	if (!fileName.isEmpty()) {
//		pm = new TCMatriz2D<int>(fileName.toStdString());
//		if( pm ){
//			getFileNames(fileName);
//			if ( loadImage(pm)){
//				return true;
//			}
//		}
//	}
//	QMessageBox::information(parent, tr("LVP"), tr("Error! - Cannot load %1.").arg(fileName));
//	return false;
//}

bool PgmImageViewer::loadFile(const QString &fileName) {
	if (!fileName.isEmpty()) {
		pm = new TCMatriz2D<int>(fileName.toStdString());
		image = new QImage(fileName);
		if (image->isNull()) {
			QMessageBox::information(parent, tr("LVP"), tr("Error! - Cannot load %1.").arg(fileName));
			return false;
		}
		QApplication::setOverrideCursor(Qt::WaitCursor);
		imageLabel->setPixmap(QPixmap::fromImage(*image));
		//QMessageBox::information(this, tr("LVP"), tr("Formato da imagem: %1").arg(image->format()));
		QApplication::restoreOverrideCursor();
		getFileNames(fileName);
		setWindowTitle(getFileName());
		return true;
	} else {
		return false;
	}
}

bool PgmImageViewer::save() {
	if (isNew) {
		return saveAs();
	} else {
		if( ! pm )
			pm = new TCMatriz2D<int>(fullFileName.toStdString());
		if (pm)
			return pm->Write(curFile.toStdString());
		else
			return false;
	}
}

bool PgmImageViewer::saveAs() {
	QString fileName;
	if (isNew){
		fileName = filePath + curFile;
	} else {
		fileName = fullFileName;
	}
	fileName = QFileDialog::getSaveFileName(parent, tr("Save As"), fileName, tr("pgm Images (*.pgm)"));
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
		pm->SetFormato(P2_X_Y_GRAY_ASCII);
	} else { //binario.
		pm->SetFormato(P5_X_Y_GRAY_BINARY);
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
