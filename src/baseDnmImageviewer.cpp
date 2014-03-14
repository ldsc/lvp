#include "baseDnmImageviewer.h"

BaseDnmImageViewer::BaseDnmImageViewer(QMainWindow * _parent)
	: BaseImageViewer( _parent ) {
	direcao = EIXO_Z;
	curPlan = 0;
	isModified = false;
}

void BaseDnmImageViewer::closeEvent(QCloseEvent *event) {
	if (maybeSave()) {
		if (isNew) {
			QFile::remove(fullFileName);
		}
		event->accept();
	} else {
		event->ignore();
	}
}

void BaseDnmImageViewer::documentWasModified() {
	isModified = true;
	updateTitle();
	setWindowModified(true);
	emit dataChanged();
}

void BaseDnmImageViewer::updateTitle(){
	if ( isModified ) {
		setWindowTitle(tr("%1[*]").arg(curFile));
	} else {
		setWindowTitle(tr("%1").arg(curFile));
	}

}
