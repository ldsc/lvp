#include "imageViewer3D.h"

ImageViewer3D::ImageViewer3D(QMainWindow * _parent) : BaseImageViewer( _parent )
{
	//setAttribute(Qt::WA_DeleteOnClose);
	//setBackgroundRole(QPalette::Shadow);
	curPlan = 0;
	direcao = CMatriz3D::EIXO_Z;
}

bool ImageViewer3D::loadFile(const QString &fileName)
{
	if (!fileName.isEmpty()) {
		if ( pm3D ) {
			delete pm3D;
			pm3D = NULL;
		}
		pm3D = new CImagem3D( fileName.toStdString() ); //cria matriz 3D só para pegar o valor da dimensão NZ.
		if ( ! pm3D )
			return false;
		nx = pm3D->NX();
		ny = pm3D->NY();
		nz = pm3D->NZ();
		if ( ! pm ) {
			pm = new CMatriz2D(pm3D->NX(), pm3D->NY());
			if ( ! pm )
				return false;
		}
		pm3D->LePlano(pm, curPlan, direcao);
		pm->Path(pm3D->Path());
      //QString file = fileName.mid(fileName.lastIndexOf("/")+1, (fileName.lastIndexOf("."))-(fileName.lastIndexOf("/")+1) ); //Pega o nome do arquivo sem a extensão.
      QString file = QFileInfo(fileName).fileName().section('.', 0, -2); //Pega o nome do arquivo sem a extensão.
		switch (pm3D->GetFormat()) {
		case WRITEFORM_DI_X_Y_Z_ASCII:
			pm->WriteFormat(WRITEFORM_PI_X_Y_ASCII);
			file = tr(".lvp_%1.pbm").arg(file);
			break;
		case WRITEFORM_DI_X_Y_Z_GRAY_ASCII:
			pm->WriteFormat(WRITEFORM_PI_X_Y_GRAY_ASCII);
			pm->NumCores( pm3D->NumCores() );
			file = tr(".lvp_%1.pgm").arg(file);
			break;
		default:
			pm->WriteFormat(WRITEFORM_PI_X_Y_ASCII);
			file = tr(".lvp_%1.pbm").arg(file);
		}
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

bool ImageViewer3D::ChangePlan( unsigned int plano, CMatriz3D::E_eixo axis )
{
	if ( ! pm ) {
		pm = new CMatriz2D(pm3D->NX(), pm3D->NY());
		if ( ! pm )
			return false;
		pm->Path(pm3D->Path());
	}
	pm3D->LePlano(pm, plano, axis);
	switch ( pm3D->GetFormat() ) {
	case WRITEFORM_DI_X_Y_Z_ASCII:
		pm->WriteFormat(WRITEFORM_PI_X_Y_ASCII);
		break;
	case WRITEFORM_DI_X_Y_Z_GRAY_ASCII:
		pm->WriteFormat(WRITEFORM_PI_X_Y_GRAY_ASCII);
		pm->NumCores( pm3D->NumCores() );
		break;
	default:
		pm->WriteFormat(WRITEFORM_PI_X_Y_ASCII);
	}
   //if ( pm->Write( curPlanoFile.mid(curPlanoFile.lastIndexOf("/")+1).toStdString()) ) {// se conseguiu salvar arquivo com nome temporário.
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

bool ImageViewer3D::save()
{
	if (isNew) {
		return saveAs();
	} else {
		//return image->save(curFile);
		//CImagem3D * pm = new CImagem3D(fullFileName.toStdString());
		if ( ! pm3D ) pm3D = new CImagem3D(fullFileName.toStdString());
		if (pm3D->Write(curFile.toStdString())) {
			return true;
		} else {
			return false;
		}
	}
}

bool ImageViewer3D::saveAs()
{
	QString fileName = QFileDialog::getSaveFileName(parent, tr("Save As"), fullFileName, tr("Images (*.dbm *.dgm)"));
	//QString fileName = QFileInfo(file).canonicalFilePath();
	if (fileName.isEmpty()) {
		//QMessageBox::information(parent, tr("LVP"), tr("Error! - File name is empty."));
		return false;
	}
	if ( ! pm3D ) pm3D = new CImagem3D(fullFileName.toStdString());
	if ( ! pm3D ) {
		QMessageBox::information(parent, tr("LVP"), tr("Error! - Can't create image."));
		return false;
	}
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

void ImageViewer3D::closeEvent(QCloseEvent *event)
{
	if (maybeSave()) {
		if (isNew) {
			QFile::remove(fullFileName);
		}
		QFile::remove(curPlanoFile);
		event->accept();
	} else {
		event->ignore();
	}
}
