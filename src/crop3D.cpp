#include "crop3D.h"

Crop3D::Crop3D(QMainWindow *_parent) {
	setupUi(this);
	parent = _parent;

	connect(buttonBox,			SIGNAL(accepted()),					parent, SLOT(exCrop3DImage()));
	connect(buttonBox,			SIGNAL(rejected()),					this,		SLOT(reject()));
	connect(spinBoxStartX,	SIGNAL(editingFinished()),	this,		SLOT(updateEndX()));
	connect(spinBoxStartY,	SIGNAL(editingFinished()),	this,		SLOT(updateEndY()));
	connect(spinBoxStartZ,	SIGNAL(editingFinished()),	this,		SLOT(updateEndZ()));
	connect(spinBoxEndX,		SIGNAL(editingFinished()),	this,		SLOT(updateStartX()));
	connect(spinBoxEndY,		SIGNAL(editingFinished()),	this,		SLOT(updateStartY()));
	connect(spinBoxEndZ,		SIGNAL(editingFinished()),	this,		SLOT(updateStartZ()));
}

Crop3D::~Crop3D() {

}

void Crop3D::updateEndX() {
	if ( spinBoxStartX->value() >= spinBoxEndX->value() )
		spinBoxEndX->setValue( spinBoxStartX->value()+1 );
}

void Crop3D::updateEndY() {
	if ( spinBoxStartY->value() >= spinBoxEndY->value() )
		spinBoxEndY->setValue( spinBoxStartY->value()+1 );
}

void Crop3D::updateEndZ() {
	if ( spinBoxStartZ->value() >= spinBoxEndZ->value() )
		spinBoxEndZ->setValue( spinBoxStartZ->value()+1 );
}

void Crop3D::updateStartX( ) {
	if ( spinBoxEndX->value() <= spinBoxStartX->value() )
		spinBoxStartX->setValue( spinBoxEndX->value()-1 );
}

void Crop3D::updateStartY( ) {
	if ( spinBoxEndY->value() <= spinBoxStartY->value() )
		spinBoxStartY->setValue( spinBoxEndY->value()-1 );
}

void Crop3D::updateStartZ( ) {
	if ( spinBoxEndZ->value() <= spinBoxStartZ->value() )
		spinBoxStartZ->setValue( spinBoxEndZ->value()-1 );
}
