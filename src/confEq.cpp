#include <QtGui>
#include "confEq.h"

using namespace std;

ConfEq::ConfEq( QMainWindow * _parent, BaseImageViewer *_obj ){	
     setupUi(this); // this sets up GUI
     parent = _parent;
     obj = _obj;
	
	if( qobject_cast<ImageViewer *>(obj) != 0 )
		connect(buttonBox, SIGNAL(accepted()), parent, SLOT(exConfEq()));
	else if( qobject_cast<ImageViewer3D *>(obj) != 0 )
		connect(buttonBox, SIGNAL(accepted()), parent, SLOT(exConfEq3D()));
     
     connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}
