#include <QtGui>
#include "poresThroats.h"

PoresThroats::PoresThroats(QMainWindow *_parent, DbmImageViewer *_child) {
	setupUi(this);
	parent = _parent;
	child	= _child;
	connect(buttonBox, SIGNAL(accepted()), parent, SLOT(exSegmentationPoresThroats()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

