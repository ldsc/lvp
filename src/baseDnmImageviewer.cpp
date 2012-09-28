#include "baseDnmImageviewer.h"

BaseDnmImageViewer::BaseDnmImageViewer(QMainWindow * _parent)
	: BaseImageViewer( _parent ) {
	direcao = EIXO_Z;
	curPlan = 0;
}

void BaseDnmImageViewer::closeEvent(QCloseEvent *event) {
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
