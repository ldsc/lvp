#include <QtGui>
#include "intrinsicPermeabilityByNetwork.h"

IntrinsicPermeabilityByNetwork::IntrinsicPermeabilityByNetwork(QMainWindow *_parent, DbmImageViewer *_child) : QDialog(_parent) {
	setupUi(this);
	parent = _parent;
	child	= _child;
	childInt = nullptr;
	this->groupBoxSementation->setEnabled(true);
	connect(buttonBox, SIGNAL(accepted()), parent, SLOT(exIntrinsicPermeabilityByNetwork()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

IntrinsicPermeabilityByNetwork::IntrinsicPermeabilityByNetwork(QMainWindow *_parent, DgmImageViewer *_child) : QDialog(_parent) {
	setupUi(this);
	parent = _parent;
	child	= nullptr;
	childInt = _child;
	this->groupBoxSementation->setEnabled(false);
	connect(buttonBox, SIGNAL(accepted()), parent, SLOT(exIntrinsicPermeabilityByNetwork()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

