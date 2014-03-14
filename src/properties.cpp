#include "properties.h"

Properties::Properties(QMainWindow *_parent) : QDialog(_parent) {
	setupUi(this);
	parent = _parent;
	connect(buttonBox, SIGNAL(accepted()), parent, SLOT(exProperties()));
}

Properties::~Properties() {

}
