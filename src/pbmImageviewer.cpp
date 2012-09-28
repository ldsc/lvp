#include "pbmImageviewer.h"

PbmImageViewer::PbmImageViewer(QMainWindow * _parent) : BasePnmImageViewer( _parent ), pm(NULL) {
	//setAttribute(Qt::WA_DeleteOnClose);
	//setBackgroundRole(QPalette::Shadow);
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
	QPushButton *asciiButton = msgBox.addButton(tr("&ASCII"), QMessageBox::ActionRole);
	msgBox.setDefaultButton(asciiButton);
	msgBox.exec();
	if ( msgBox.clickedButton() == cancelButton )
		return false;
	if (msgBox.clickedButton() == asciiButton) { //ascii
		pm->SetFormato(P1_X_Y_ASCII);
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
