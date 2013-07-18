#include <QtGui>
#include <QFileDialog>
//#include <iostream>
#include "reconstruction.h"

using namespace std;

Reconstruction::Reconstruction( QString & _filepath, QMainWindow * _parent ) : QDialog(_parent){
     setupUi(this); // this sets up GUI
     parent = _parent;
     lineEditFile->setText(_filepath);
     
     //connect(pushButtonChange, SIGNAL(clicked()), parent, SLOT(reconstructionGT()));
     connect(pushButtonChange, SIGNAL(clicked()), this, SLOT(change()));
     connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
     connect(buttonBox, SIGNAL(accepted()), parent, SLOT(exReconstructionGT()));
}


/*
Reconstruction::~Reconstruction(){
//	cerr << "destrutor" << endl;
}*/

void Reconstruction::change(){
	QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"), "./lvp/bin/", tr("Correlation File (*.cor)"));
	if ( ! filePath.isEmpty() )
		lineEditFile->setText( filePath );
}
