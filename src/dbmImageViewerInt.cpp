#include "dbmImageViewerInt.h"

DbmImageViewerInt::DbmImageViewerInt(QMainWindow * _parent)
	: BaseDnmImageViewer( _parent ), pm(NULL), pm3D(NULL) {
}

bool DbmImageViewerInt::loadFile(const QString &fileName) {
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
		//QString file = fileName.mid(fileName.lastIndexOf("/")+1, (fileName.lastIndexOf("."))-(fileName.lastIndexOf("/")+1) ); //Pega o nome do arquivo sem a extensão.
		QString file = QFileInfo(fileName).fileName().section('.', 0, -2); //Pega o nome do arquivo sem a extensão.
		if ( pm3D->GetFormato() == D1_X_Y_Z_ASCII ) {
			pm->SetFormato(P1_X_Y_ASCII);
		} else {
			pm->SetFormato(P4_X_Y_BINARY);
		}
		file = tr(".lvp_%1.pbm").arg(file);
		if ( pm->Write(file.toStdString()) ) {// se conseguiu salvar arquivo com nome temporário.
			//delete pm;
			curPlanoFile = tr("%1%2").arg(QString::fromStdString(pm->Path())).arg(file); //Pega o nome do arquivo temporário com o caminho relativo para poder ser apagado posteriormente.
			//curPlanoFile = file.fromStdString(tempFile); //Pega o nome do arquivo temporário com o caminho relativo para poder ser apagado posteriormente.
			image = new QImage(curPlanoFile);
			if (image->isNull()) {
				QMessageBox::information(parent, tr("LVP"), tr("Error! - Cannot load %1.").arg(curPlanoFile));
				return false;
			}
			QApplication::setOverrideCursor(Qt::WaitCursor);
			imageLabel->setPixmap(QPixmap::fromImage(*image));
			QApplication::restoreOverrideCursor();
			getFileNames(fileName);
			setWindowTitle(curFile);
			return true;
		}
		return false;
	}
	return false;
}

bool DbmImageViewerInt::ChangePlan( unsigned int plano, E_eixo axis ) {
	if ( ! pm ) {
		pm = new TCMatriz2D<int>(pm3D->NX(), pm3D->NY());
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
	if ( pm->Write( QFileInfo(curPlanoFile).fileName().toStdString()) ) {// se conseguiu salvar arquivo com nome temporário.
		if ( ! image->isNull() ) delete image;
		image = new QImage(curPlanoFile);
		if (image->isNull()) {
			QMessageBox::information(parent, tr("LVP"), tr("Error! - Cannot load %1.").arg(curPlanoFile));
			return false;
		}
		curPlan = plano;
		direcao = axis;
		QApplication::setOverrideCursor(Qt::WaitCursor);
		imageLabel->setPixmap(QPixmap::fromImage(*image));
		QApplication::restoreOverrideCursor();
		return true;
	}
	return false;
}

bool DbmImageViewerInt::save() {
	if (isNew) {
		return saveAs();
	} else if ( ! pm3D ) {
		pm3D = new TCImagem3D<int>(fullFileName.toStdString());
		if ( pm3D )
			return pm3D->Write(curFile.toStdString());
	}
	return false;
}

bool DbmImageViewerInt::saveAs() {
	QString fileName = QFileDialog::getSaveFileName(parent, tr("Save As"), fullFileName, tr("dbm Images (*.dbm)"));
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
	QPushButton *asciiButton = msgBox.addButton(tr("&ASCII"), QMessageBox::ActionRole);
	msgBox.setDefaultButton(asciiButton);
	msgBox.exec();
	if ( msgBox.clickedButton() == cancelButton )
		return false;
	if (msgBox.clickedButton() == asciiButton) //ascii
		pm3D->SetFormato(D1_X_Y_Z_ASCII);
	else //binario.
		pm3D->SetFormato(D4_X_Y_Z_BINARY);
	string tmp = pm3D->Path();
	pm3D->Path("");
	if ( pm3D->Write( fileName.toStdString() ) ) {
		if ( isNew && (fileName != fullFileName) ) {
			QFile::remove(fullFileName);
			QFile::remove(curPlanoFile);
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
