#include <QtGui>
#include "percolationNetwork.h"

PercolationNetwork::PercolationNetwork(QMainWindow *_parent, DbmImageViewer *_child) : QDialog(_parent) {
	setupUi(this);
	parent = _parent;
	child	= _child;
	childInt = nullptr;
	this->groupBoxSementation->setEnabled(true);
	connect(buttonBox, SIGNAL(accepted()), parent, SLOT(exPercolationNetwork()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
	connect(checkBoxUseDistributionsFiles, SIGNAL(toggled(bool)), this->groupBoxSementation, SLOT(setDisabled(bool)));
}

PercolationNetwork::PercolationNetwork(QMainWindow *_parent, DgmImageViewer *_child) : QDialog(_parent) {
	setupUi(this);
	parent = _parent;
	child	= nullptr;
	childInt = _child;
	this->groupBoxSementation->setEnabled(false);
	connect(buttonBox, SIGNAL(accepted()), parent, SLOT(exPercolationNetwork()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

