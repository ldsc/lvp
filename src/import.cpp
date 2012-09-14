#include "import.h"

using namespace std;

Import::Import(QMainWindow *_parent, QString _file) {
	setupUi(this); // this sets up GUI
	parent = _parent;
	fullFileName = _file;

	connect(buttonBox, SIGNAL(accepted()), parent, SLOT(exImport()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

Import::~Import()
{
}
