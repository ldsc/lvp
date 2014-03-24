#include "pbmImageviewer.h"

PbmImageViewer::PbmImageViewer(QMainWindow * _parent) : BasePnmImageViewer( _parent ), pm(nullptr) {
	//setAttribute(Qt::WA_DeleteOnClose);
	//setBackgroundRole(QPalette::Shadow);
}

bool PbmImageViewer::loadFile(const QString &fileName) {
	if (!fileName.isEmpty()) {
		pm = new TCMatriz2D<bool>(fileName.toStdString());
		if( pm ){
			getFileNames(fileName);
			if ( loadImage(pm)){
				return true;
			}
		}
	}
	QMessageBox::information(parent, tr("LVP"), tr("Error! - Cannot load %1.").arg(fileName));
	return false;
}

void PbmImageViewer::reloadFile() {
	if (pm) {
		delete pm;
		pm = nullptr;
	}
	pm = new TCMatriz2D<bool>( fullFileName.toStdString() );
	if ( ! reloadImage(pm) ){
		QMessageBox::information(parent, tr("LVP"), tr("Error! - Cannot load image."));
	}
}

bool PbmImageViewer::save() {
	if (isNew) {
		return saveAs();
	} else {
		if( ! pm )
			pm = new TCMatriz2D<bool>(fullFileName.toStdString());
		if (pm)
			return pm->Write(curFile.toStdString());
		else
			return false;
	}
}

bool PbmImageViewer::saveAs() {
	QString fileName;
	if (isNew){
		fileName = filePath + curFile;
	} else {
		fileName = fullFileName;
	}
	fileName = QFileDialog::getSaveFileName(parent, tr("Save As"), fileName, tr("pbm Images (*.pbm)"));
	if (fileName.isEmpty()){
		return false;
	}
	if ( ! pm ) {
		pm = new TCMatriz2D<bool>(fullFileName.toStdString());
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
	QPushButton *alignedAsciiButton = msgBox.addButton(tr("A&ligned ASCII"), QMessageBox::ActionRole);
	QPushButton *asciiButton = msgBox.addButton(tr("&ASCII"), QMessageBox::ActionRole);
	msgBox.setDefaultButton(asciiButton);
	msgBox.exec();
	if ( msgBox.clickedButton() == cancelButton )
		return false;
	if (msgBox.clickedButton() == asciiButton) { //ascii
		pm->SetFormato(P1_X_Y_ASCII);
	} else if (msgBox.clickedButton() == alignedAsciiButton) { //ascii alinhado
		pm->SetFormato(P1_X_Y_ASCII);
		pm->salvarAlinhado = true;
	} else { //binario.
		pm->SetFormato(P4_X_Y_BINARY);
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
