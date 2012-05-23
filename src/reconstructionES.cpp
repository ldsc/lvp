#include <QtGui>
#include <QFileDialog>
#include <QFile>
#include "reconstructionES.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

ReconstructionES::ReconstructionES( QString & _filepath, QMainWindow * _parent ) {
	setupUi(this); // this sets up GUI
	parent = _parent;
	lineEditFile->setText(_filepath);
	readInfo();
	connect(pushButtonChange, SIGNAL(clicked()), 	this, 	SLOT(change()));
	connect(buttonBox, 	   SIGNAL(rejected()),	this,   SLOT(reject()));
	connect(buttonBox, 	   SIGNAL(accepted()),	parent, SLOT(exReconstructionES()));
}

/*
ReconstructionES::~ReconstructionES(){
//	cerr << "destrutor" << endl;
}*/

void ReconstructionES::change() {
   QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"), "./lvp/bin/", tr("Correlation File (*.dts)"));
	if ( ! filePath.isEmpty() ) {
		lineEditFile->setText( filePath );
		readInfo();
	}
}

void ReconstructionES::readInfo() {
	if ( ! lineEditFile->text().isEmpty() ) {
		cerr << lineEditFile->text().toStdString() << endl;
		ifstream file( lineEditFile->text().toStdString().c_str() );
      if ( file.is_open() && file.good() ) {
			string line;
			int pos;
			while ( (! file.fail()) && ( ! file.eof()) ) { //percorre o arquivo até chegar o fim do arquivo ou atingir o tamanho máximo do vetor correlação
				pos = file.tellg(); // pega a posição atual de leitura
				getline(file, line, '\n'); // le toda a linha
				if (line.substr(0,9) == "# Solids:") {
					double value;
					file.seekg(pos+10); // reposiciona o ponteiro de leitura para pegar a porosidade
					file >> value; //pega o valor da porosidade
					cerr << "value: " << value << endl;
					doubleSpinBoxPhi->setValue( value );
					break;
				}
			}
		}
		file.close();
	}
}
