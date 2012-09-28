#include "basePnmImageviewer.h"

BasePnmImageViewer::BasePnmImageViewer(QMainWindow * _parent)
	: BaseImageViewer( _parent ) { }

bool BasePnmImageViewer::loadFile(const QString &fileName) {
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
	} else {
		return false;
	}
}

void BasePnmImageViewer::closeEvent(QCloseEvent *event) {
	if (maybeSave()) {
		if(isNew){
			QFile::remove(fullFileName);
		}
		event->accept();
	} else {
		event->ignore();
	}
}
