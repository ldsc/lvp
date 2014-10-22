#include <QtGui>
#include "intrinsicPermeabilityByNetwork.h"

IntrinsicPermeabilityByNetwork::IntrinsicPermeabilityByNetwork(QMainWindow *_parent, DbmImageViewer *_child) : QDialog(_parent) {
	setupUi(this);
	parent = _parent;
	child	= _child;
	childInt = nullptr;
	childRSL = nullptr;
	this->groupBoxSementation->setEnabled(true);
	this->groupBoxPercolationNetwork->setEnabled(true);
	connect(buttonBox, SIGNAL(accepted()), parent, SLOT(exIntrinsicPermeabilityByNetwork()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

IntrinsicPermeabilityByNetwork::IntrinsicPermeabilityByNetwork(QMainWindow *_parent, DgmImageViewer *_child) : QDialog(_parent) {
	setupUi(this);
	parent = _parent;
	child	= nullptr;
	childInt = _child;
	childRSL = nullptr;
	this->groupBoxSementation->setEnabled(false);
	this->groupBoxPercolationNetwork->setEnabled(true);
	connect(buttonBox, SIGNAL(accepted()), parent, SLOT(exIntrinsicPermeabilityByNetwork()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

IntrinsicPermeabilityByNetwork::IntrinsicPermeabilityByNetwork(QMainWindow *_parent, TextEditor *_child) : QDialog(_parent) {
	setupUi(this);
	parent = _parent;
	child	= nullptr;
	childInt = nullptr;
	childRSL = _child;
	this->groupBoxSementation->setEnabled(false);
	this->groupBoxPercolationNetwork->setEnabled(false);
	this->checkBoxSaveDistributions->setEnabled(false);
	this->checkBoxSPN->setEnabled(false);
	connect(buttonBox, SIGNAL(accepted()), parent, SLOT(exIntrinsicPermeabilityByNetwork()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

