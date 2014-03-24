#include "dbmImageViewer.h"

DbmImageViewer::DbmImageViewer(QMainWindow * _parent)
	: BaseDnmImageViewer( _parent ), pm(nullptr), pm3D(nullptr) {
}

//bool DbmImageViewer::loadFile(const QString &fileName) {
//	if (!fileName.isEmpty()) {
//		if ( pm3D ) {
//			delete pm3D;
//			pm3D = nullptr;
//		}
//		pm3D = new TCImagem3D<bool>( fileName.toStdString() ); //cria matriz 3D só para pegar o valor da dimensão NZ.
//		if ( ! pm3D )
//			return false;
//		nx = pm3D->NX();
//		ny = pm3D->NY();
//		nz = pm3D->NZ();
//		if ( ! pm ) {
//			pm = new TCMatriz2D<bool>(pm3D->NX(), pm3D->NY());
//			if ( ! pm )
//				return false;
//		}
//		pm3D->LePlano(pm, curPlan, direcao);
//		pm->Path(pm3D->Path());
//		//QString file = fileName.mid(fileName.lastIndexOf("/")+1, (fileName.lastIndexOf("."))-(fileName.lastIndexOf("/")+1) ); //Pega o nome do arquivo sem a extensão.
//		QString file = QFileInfo(fileName).fileName().section('.', 0, -2); //Pega o nome do arquivo sem a extensão.
//		if ( pm3D->GetFormato() == D1_X_Y_Z_ASCII ) {
//			pm->SetFormato(P1_X_Y_ASCII);
//		} else {
//			pm->SetFormato(P4_X_Y_BINARY);
//		}
//		file = tr(".lvp_%1.pbm").arg(file);
//		if ( pm->Write(file.toStdString()) ) {// se conseguiu salvar arquivo com nome temporário.
//			//delete pm;
//			curPlanoFile = tr("%1%2").arg(QString::fromStdString(pm->Path())).arg(file); //Pega o nome do arquivo temporário com o caminho relativo para poder ser apagado posteriormente.
//			//curPlanoFile = file.fromStdString(tempFile); //Pega o nome do arquivo temporário com o caminho relativo para poder ser apagado posteriormente.
//			image = new QImage(curPlanoFile);
//			if (image->isNull()) {
//				QMessageBox::information(parent, tr("LVP"), tr("Error! - Cannot load %1.").arg(curPlanoFile));
//				return false;
//			}
//			QApplication::setOverrideCursor(Qt::WaitCursor);
//			imageLabel->setPixmap(QPixmap::fromImage(*image));
//			QApplication::restoreOverrideCursor();
//			getFileNames(fileName);
//			setWindowTitle(curFile);
//			return true;
//		}
//		return false;
//	}
//	return false;
//}

bool DbmImageViewer::loadFile(const QString &fileName) {
	if (!fileName.isEmpty()) {
		if ( pm3D ) {
			delete pm3D;
			pm3D = nullptr;
		}
		pm3D = new TCImagem3D<bool>( fileName.toStdString() ); //cria matriz 3D só para pegar o valor da dimensão NZ.
		if ( ! pm3D )
			return false;
		nx = pm3D->NX();
		ny = pm3D->NY();
		nz = pm3D->NZ();
		if ( ! pm ) {
			pm = new TCMatriz2D<bool>(pm3D->NX(), pm3D->NY());
			if ( ! pm )
				return false;
		}
		pm3D->LePlano(pm, curPlan, direcao);
		pm->Path(pm3D->Path());
		if ( pm3D->GetFormato() == D1_X_Y_Z_ASCII ) {
			pm->SetFormato(P1_X_Y_ASCII);
		} else {
			pm->SetFormato(P4_X_Y_BINARY);
		}
		getFileNames(fileName);
		//setWindowTitle(curFile);
		if (! loadImage(pm)) {
			QMessageBox::information(parent, tr("LVP"), tr("Error! - Cannot load image."));
			return false;
		}
		return true;
	}
	return false;
}

void DbmImageViewer::reloadFile() {
	if ( pm3D ) {
		delete pm3D;
		pm3D = nullptr;
	}
	pm3D = new TCImagem3D<bool>( fullFileName.toStdString() ); //cria matriz 3D só para pegar o valor da dimensão NZ.
	if ( ! pm3D ) {
		QMessageBox::information(parent, tr("LVP"), tr("Error! - Cannot load image."));
		return;
	}
	nx = pm3D->NX();
	ny = pm3D->NY();
	nz = pm3D->NZ();
	if(pm->NX()!=nx or pm->NY()!=ny){
		delete pm;
		pm = nullptr;
	}
	if ( ! pm ) {
		pm = new TCMatriz2D<bool>(pm3D->NX(), pm3D->NY());
		pm->Path(pm3D->Path());
		if ( ! pm ) {
			QMessageBox::information(parent, tr("LVP"), tr("Error! - Cannot load image."));
			return;
		}
	}
	pm3D->LePlano(pm, curPlan, direcao);
	if ( pm3D->GetFormato() == D1_X_Y_Z_ASCII ) {
		pm->SetFormato(P1_X_Y_ASCII);
	} else {
		pm->SetFormato(P4_X_Y_BINARY);
	}
	if (! reloadImage(pm)) {
		QMessageBox::information(parent, tr("LVP"), tr("Error! - Cannot load image."));
	}
}

bool DbmImageViewer::ChangePlan( unsigned int plano, E_eixo axis ) {
	if ( ! pm ) {
		pm = new TCMatriz2D<bool>(pm3D->NX(), pm3D->NY());
		if ( ! pm )
			return false;
		pm->Path(pm3D->Path());
	}
	pm3D->LePlano(pm, plano, axis);
	if ( pm3D->GetFormato() == D1_X_Y_Z_ASCII ) {
		pm->SetFormato(P1_X_Y_ASCII);
	} else {
		pm->SetFormato(P4_X_Y_BINARY);
	}
	if ( ! BaseImageViewer::reloadImage(pm) ) {
		QMessageBox::information(parent, tr("LVP"), tr("Error! - Cannot load image."));
		return false;
	}
	curPlan = plano;
	direcao = axis;
	return true;
}


bool DbmImageViewer::save() {
	bool flag = false;
	if (isNew) {
		flag = saveAs();
	} else if ( ! pm3D ) {
		pm3D = new TCImagem3D<bool>(fullFileName.toStdString());
	}
	if ( pm3D ) {
		pm3D->Path("");
		flag = pm3D->Write(fullFileName.toStdString());
	}
	if (flag) {
		isModified = false;
		updateTitle();
	}
	return flag;
}

bool DbmImageViewer::saveAs() {
	QString fileName = QFileDialog::getSaveFileName(parent, tr("Save As"), fullFileName, tr("dbm Images (*.dbm)"));
	//QString fileName = QFileInfo(file).canonicalFilePath();
	if (fileName.isEmpty()) {
		//QMessageBox::information(parent, tr("LVP"), tr("Error! - File name is empty."));
		return false;
	}
	if ( ! pm3D )
		pm3D = new TCImagem3D<bool>(fullFileName.toStdString());
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
	if (msgBox.clickedButton() == asciiButton) {//ascii
		pm3D->SetFormato(D1_X_Y_Z_ASCII);
	} else if (msgBox.clickedButton() == alignedAsciiButton) { //ascii alinhado
		pm3D->SetFormato(D1_X_Y_Z_ASCII);
		pm3D->salvarAlinhado = true;
	} else { //binario.
		pm3D->SetFormato(D4_X_Y_Z_BINARY);
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
