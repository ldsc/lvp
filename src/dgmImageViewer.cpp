#include "dgmImageViewer.h"

DgmImageViewer::DgmImageViewer(QMainWindow * _parent)
	: BaseDnmImageViewer( _parent ), pm(NULL), pm3D(NULL) {
}

bool DgmImageViewer::loadFile(const QString &fileName) {
	if (!fileName.isEmpty()) {
		if ( pm3D ) {
			delete pm3D;
			pm3D = NULL;
		}
		pm3D = new TCImagem3D<int>( fileName.toStdString() ); //cria matriz 3D só para pegar o valor da dimensão NZ.
		if ( ! pm3D )
			return false;
		nx = pm3D->NX();
		ny = pm3D->NY();
		nz = pm3D->NZ();
		if ( ! pm ) {
			pm = new TCMatriz2D<int>(pm3D->NX(), pm3D->NY());
			if ( ! pm )
				return false;
		}
		pm3D->LePlano(pm, curPlan, direcao);
		pm->Path(pm3D->Path());
		if ( pm3D->GetFormato() == D2_X_Y_Z_GRAY_ASCII ) {
			pm->SetFormato(P2_X_Y_GRAY_ASCII);
		} else {
			pm->SetFormato(P5_X_Y_GRAY_BINARY);
		}
		pm->NumCores( pm3D->NumCores() );

		getFileNames(fileName);
		setWindowTitle(curFile);
		if ( ! loadImage(pm) ) {
			QMessageBox::information(parent, tr("LVP"), tr("Error! - Cannot load image."));
			return false;
		}
		return true;
	}
	return false;
}

void DgmImageViewer::reloadFile() {
	if ( pm3D ) {
		delete pm3D;
		pm3D = NULL;
	}
	pm3D = new TCImagem3D<int>( fullFileName.toStdString() ); //cria matriz 3D só para pegar o valor da dimensão NZ.
	if ( ! pm3D ) {
		QMessageBox::information(parent, tr("LVP"), tr("Error! - Cannot load image."));
		return;
	}
	nx = pm3D->NX();
	ny = pm3D->NY();
	nz = pm3D->NZ();
	if(pm->NX()!=nx or pm->NY()!=ny){
		delete pm;
		pm = NULL;
	}
	if ( ! pm ) {
		pm = new TCMatriz2D<int>(pm3D->NX(), pm3D->NY());
		pm->Path(pm3D->Path());
		if ( ! pm ) {
			QMessageBox::information(parent, tr("LVP"), tr("Error! - Cannot load image."));
		}
	}
	pm3D->LePlano(pm, curPlan, direcao);
	if ( pm3D->GetFormato() == D2_X_Y_Z_GRAY_ASCII ) {
		pm->SetFormato(P2_X_Y_GRAY_ASCII);
	} else {
		pm->SetFormato(P5_X_Y_GRAY_BINARY);
	}
	pm->NumCores( pm3D->NumCores() );
	if ( ! reloadImage(pm) ) {
		QMessageBox::information(parent, tr("LVP"), tr("Error! - Cannot load image."));
	}
}

bool DgmImageViewer::ChangePlan( unsigned int plano, E_eixo axis ) {
	if ( ! pm ) {
		pm = new TCMatriz2D<int>(pm3D->NX(), pm3D->NY());
		if ( ! pm )
			return false;
		pm->Path(pm3D->Path());
	}
	pm3D->LePlano(pm, plano, axis);
	if ( pm3D->GetFormato() == D2_X_Y_Z_GRAY_ASCII ) {
		pm->SetFormato(P2_X_Y_GRAY_ASCII);
	} else {
		pm->SetFormato(P5_X_Y_GRAY_BINARY);
	}
	pm->NumCores( pm3D->NumCores() );

	if ( ! BaseImageViewer::reloadImage(pm) ) {
		QMessageBox::information(parent, tr("LVP"), tr("Error! - Cannot load image."));
		return false;
	}
	curPlan = plano;
	direcao = axis;
	return true;
}

bool DgmImageViewer::save() {
	if (isNew) {
		return saveAs();
	} else if ( ! pm3D ) {
		pm3D = new TCImagem3D<int>(fullFileName.toStdString());
		if ( pm3D )
			return pm3D->Write(curFile.toStdString());
	}
	return false;
}

bool DgmImageViewer::saveAs() {
	QString fileName = QFileDialog::getSaveFileName(parent, tr("Save As"), fullFileName, tr("dgm Images (*.dgm)"));
	//QString fileName = QFileInfo(file).canonicalFilePath();
	if (fileName.isEmpty()) {
		//QMessageBox::information(parent, tr("LVP"), tr("Error! - File name is empty."));
		return false;
	}
	if ( ! pm3D )
		pm3D = new TCImagem3D<int>(fullFileName.toStdString());
	if ( ! pm3D ) {
		QMessageBox::information(parent, tr("LVP"), tr("Error! - Can't create image."));
		return false;
	}
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
		pm3D->SetFormato(D2_X_Y_Z_GRAY_ASCII);
	} else if (msgBox.clickedButton() == alignedAsciiButton) { //ascii alinhado
		pm3D->SetFormato(D2_X_Y_Z_GRAY_ASCII);
		pm3D->salvarAlinhado = true;
	} else { //binario.
		pm3D->SetFormato(D5_X_Y_Z_GRAY_BINARY);
	}
	string tmp = pm3D->Path();
	pm3D->Path("");
	if ( pm3D->Write( fileName.toStdString() ) ) {
		if ( isNew && (fileName != fullFileName) ) {
			QFile::remove(fullFileName);
		}
		isNew = false;
		loadFile(fileName);
		return true;
	} else {
		pm3D->Path(tmp);
		QMessageBox::information(parent, tr("LVP"), tr("Error! - Can't save file \"%1\"").arg(fileName));
		return false;
	}
}
