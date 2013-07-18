#include "ploter.h"

using namespace std;

//Ploter::Ploter( Lvp * _parent ) {
Ploter::Ploter( QMainWindow * _parent ) : QwtPlot(_parent) {
	parent = _parent;
	setAttribute(Qt::WA_DeleteOnClose);
	setBackgroundRole(QPalette::Base);
	isNew = false;
	isAccumulated = false;
	curveAccumulated = NULL;
	item = NULL;

	legend = new QwtLegend(this);
	insertLegend ( legend, QwtPlot::BottomLegend );
	//setMargin ( 10 );

	colors << Qt::blue 	   << Qt::red 	   << Qt::cyan     << Qt::yellow     << Qt::magenta     << Qt::lightGray << Qt::gray  << Qt::green;
	colors << Qt::darkBlue << Qt::darkRed << Qt::darkCyan << Qt::darkYellow << Qt::darkMagenta << Qt::darkGray  << Qt::black << Qt::darkGreen;
	//myplot = NULL;
	/*
	 zoomer = new QwtPlotZoomer(QwtPlot::xBottom, QwtPlot::yLeft, QwtPicker::DragSelection | QwtPicker::CornerToCorner, QwtPicker::AlwaysOff, this->canvas(), true);
	 zoomer->setRubberBandPen(QColor(Qt::red));
	 zoomer->setMousePattern(QwtEventPattern::MouseSelect2, Qt::RightButton, Qt::ControlModifier);
	 zoomer->setMousePattern(QwtEventPattern::MouseSelect3, Qt::RightButton);
	 */
	position = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft, /*QwtPicker::PointSelection | QwtPicker::DragSelection, */ QwtPlotPicker::NoRubberBand,  QwtPicker::AlwaysOn, this->canvas());
	position->setTrackerPen(QColor(Qt::darkBlue));
	connect(position, SIGNAL(moved(const QPoint &)), this, SLOT(labelPos(const QPoint &)));

	connect( this, SIGNAL( destroyed() ), parent, SLOT( updateDockLista() ) );
}

bool Ploter::loadFile(const QString & _fileName) {
	if ( ! _fileName.isEmpty() ) {
		getFileNames ( _fileName );
		setWindowTitle ( fileName );
		string aux;
		ifstream fin (fullFileName.toStdString().c_str(), ios::in);
		int cont = 0, tam  = 512;
		if (fin.good() && fin.is_open()) {
			if ( fileExt == "cor" or fileExt == "COR" )	{
				QwtPlotCurve *curve = new QwtPlotCurve( fileName );
				double x[tam], y[tam];
				while ( (fin >> x[cont] >> y[cont]) && (cont < tam) ) {
					//cerr << x[cont] << "\t" << y[cont] << endl;
					cont++;
				}
				curve->setPen(QPen(colors[0], 2));
				curve->setSamples(x , y, cont);
				curve->attach(this);
				setTitle ("Auto Correlation");
				setAxisTitle ( QwtPlot::yLeft , "C(u)" );
				setAxisTitle ( QwtPlot::xBottom , "Displacement" );
			} else if ( fileExt == "dtp" or fileExt == "DTP" or fileExt == "dts" or fileExt == "DTS" or fileExt == "dtg" or fileExt == "DTG") {
				QwtPlotCurve *curve = new QwtPlotCurve( fileName );
				double x[tam], y[tam];
				while ( (! fin.fail()) && ( ! fin.eof()) && (cont < tam) ) {
					fin >> aux;
					if (aux.at(0) == '#') {
						getline(fin, aux, '\n');
					} else {
						x[cont] = atof(aux.c_str());
						fin >> y[cont];
						cont++;
					}
				}
				/*while ( (fin >> x[cont] >> y[cont]) && (cont < tam) ) {
					//cerr << x[cont] << "\t" << y[cont] << endl;
					cont++;
				}*/
				curve->setPen(QPen(colors[0], 2));
				curve->setSamples(x , y, cont-1);
				curve->attach(this);
				if ( fileExt == "dts" or fileExt == "DTS") {
					setTitle ("Solids Size Distribution");
					setAxisTitle ( QwtPlot::yLeft, "Area of solids (%)" );
				} else if ( fileExt == "dtg" or fileExt == "DTG") {\
					setTitle ("Throat Size Distribution");
					setAxisTitle ( QwtPlot::yLeft, "Porosity (%)" );
				} else {
					setTitle ("Pores Size Distribution");
					setAxisTitle ( QwtPlot::yLeft, "Porosity (%)" );
				}
				setAxisTitle ( QwtPlot::xBottom, "Radius (pixel)" );
			} else if ( fileExt == "rpc" or fileExt == "RPC" ) {
				QwtPlotCurve *curveOil = new QwtPlotCurve(tr("Oil-%1").arg(fileName));
				QwtPlotCurve *curveWater = new QwtPlotCurve(tr("Water-%1").arg(fileName));
				double saturacao[tam], water[tam], oil[tam];
				while ( (fin >> saturacao[cont] >> water[cont] >> oil[cont]) && (cont < tam) ) {
					//cerr << saturacao[cont] << "\t" << water[cont] << "\t" << oil[cont] << endl;
					cont++;
				}
				QwtSymbol *sym = new QwtSymbol();
				sym->setPen(QPen(QColor(Qt::black)));
				sym->setSize(QSize(5,5));
				sym->setStyle(QwtSymbol::XCross);

				curveWater->setSymbol(sym);
				curveWater->setPen(QPen(colors[patchCurves.size()], 2));
				curveWater->setSamples(saturacao, water, cont);

				curveOil->setSymbol(sym);
				curveOil->setPen(QPen(colors[15 - patchCurves.size()], 2));
				curveOil->setSamples(saturacao, oil, cont);

				curveWater->attach(this);
				curveOil->attach(this);

				//enableAxis ( QwtPlot::xTop, true );
				setTitle ("Relative Permeability");
				setAxisTitle ( QwtPlot::yLeft , "Permeability (kr)" );
				setAxisTitle ( QwtPlot::xBottom , "Water Saturation (%)" );
				//setAxisTitle ( QwtPlot::xTop , "Oil Saturation (%)" );
				setAxisScale ( QwtPlot::yLeft, 0, 1.0, 0 );
				setAxisScale ( QwtPlot::xBottom, 0, 100, 0 );
			}
			replot();
			fin.close();
			patchCurves.append(_fileName);	// lista de curvas plotadas
			return true;
		} else {
			fin.close();
			return false;
		}
	} else {
		return false;
	}
}

bool Ploter::addCurves(const QStringList & _filesName) {
	if ( ! _filesName.isEmpty() ) {
		QString fileName;
		QString ext;
		string aux;
		QApplication::setOverrideCursor(Qt::WaitCursor);
		for (int i = 0; i < _filesName.size(); ++i) {
			if ( patchCurves.size() >= 16 ) { // estou limitando para 15 o número de curvas para atender o array de cores.
				QApplication::restoreOverrideCursor();
				QMessageBox::information(parent, tr("LVP"), tr("Limit number of curves reached!"));
				return false;
			}
			fileName = _filesName.at(i);
			if ( ! patchCurves.contains( fileName ) ) { // a curva ainda não foi plotada.
				ext = fileName.mid(fileName.lastIndexOf(".")+1); // pega extenção do arquivo
				ifstream fin (fileName.toStdString().c_str(), ios::in);
				int cont = 0, tam  = 512;
				if (fin.good() && fin.is_open()) {
					if ( ext == "cor" or ext == "COR" )	{
						//QwtPlotCurve *curve = new QwtPlotCurve(fileName.mid(fileName.lastIndexOf("/")+1));
						QwtPlotCurve *curve = new QwtPlotCurve(QFileInfo(fileName).fileName());
						double x[tam], y[tam];
						while ( (fin >> x[cont] >> y[cont]) && (cont < tam) ) {
							//cerr << x[cont] << " " << y[cont] << endl;
							cont++;
						}
						curve->setPen(QPen(colors[patchCurves.size()], 2));
						curve->setSamples(x , y, cont);
						curve->attach(this);
					} else if ( fileExt == "dtp" or fileExt == "DTP" or fileExt == "dts" or fileExt == "DTS") {
						//QwtPlotCurve *curve = new QwtPlotCurve(fileName.mid(fileName.lastIndexOf("/")+1));
						QwtPlotCurve *curve = new QwtPlotCurve(QFileInfo(fileName).fileName());
						double x[tam], y[tam];
						while ( (! fin.fail()) && ( ! fin.eof()) && (cont < tam) ) {
							fin >> aux;
							if (aux.at(0) == '#') {
								getline(fin, aux, '\n');
							} else {
								x[cont] = atof(aux.c_str());
								fin >> y[cont];
								cont++;
							}
						}
						/*while ( (fin >> x[cont] >> y[cont]) && (cont < tam) ) {
							//cerr << x[cont] << " " << y[cont] << endl;
							cont++;
						}*/
						curve->setPen(QPen(colors[patchCurves.size()], 2));
						curve->setSamples(x, y, cont-1);
						curve->attach(this);
					} else if ( ext == "rpc" or ext == "RPC" ) {
						QwtPlotCurve *curveOil = new QwtPlotCurve(tr("Oil-%1").arg(fileName));
						QwtPlotCurve *curveWater = new QwtPlotCurve(tr("Water-%1").arg(fileName));
						double saturacao[tam], water[tam], oil[tam];
						while ( (fin >> saturacao[cont] >> water[cont] >> oil[cont]) && (cont < tam) ) {
							//cerr << saturacao[cont] << " " << water[cont] << " " << oil[cont] << endl;
							cont++;
						}
						QwtSymbol *sym = new QwtSymbol();
						sym->setPen(QPen(QColor(Qt::black)));
						sym->setSize(QSize(5,5));
						sym->setStyle(QwtSymbol::XCross);

						curveWater->setSymbol(sym);
						curveWater->setPen(QPen(colors[patchCurves.size()], 2));
						curveWater->setSamples(saturacao, water, cont);

						curveOil->setSymbol(sym);
						curveOil->setPen(QPen(colors[15 - patchCurves.size()], 2));
						curveOil->setSamples(saturacao, oil, cont);

						curveWater->attach(this);
						curveOil->attach(this);
					}
					replot();
					fin.close();
					patchCurves.append(fileName);	// lista de curvas plotadas

				} else {
					fin.close();
					continue;
				}
			} else {
				QMessageBox::information(parent, tr("LVP"), tr("The file \"%1\" has been plotted!").arg(fileName));
			}
		}
		QApplication::restoreOverrideCursor();
		return true;
	} else {
		return false;
	}
}

void Ploter::accumulated() {
	if ( ! isAccumulated ) {
		isAccumulated = true;
		QApplication::setOverrideCursor(Qt::WaitCursor);
		ifstream fin (fullFileName.toStdString().c_str(), ios::in);
		int cont = 0, tam  = 512;
		double tmp = 0.0;
		string aux;
		if (fin.good() && fin.is_open()) {
			if ( fileExt == "dtp" or fileExt == "DTP" or fileExt == "dts" or fileExt == "DTS" or fileExt == "dtg" or fileExt == "DTG") {
				curveAccumulated = new QwtPlotCurve(tr("accumulated-%1").arg(fileName));
				double x[tam], y[tam];
				while ( (! fin.fail()) && ( ! fin.eof()) && (cont < tam) ) {
					fin >> aux;
					if (aux.at(0) == '#') {
						getline(fin, aux, '\n');
					} else {
						x[cont] = atof(aux.c_str());
						fin >> tmp;
						if ( cont == 0 ){
							y[cont] = tmp;
						} else {
							y[cont] =  y[cont-1] + tmp;
						}
						cont++;
					}
				}
				curveAccumulated->setPen(QPen(colors[patchCurves.size()], 2));
				curveAccumulated->setSamples(x, y, cont-1);
				curveAccumulated->attach(this);
			}
			replot();
		}
		fin.close();
		QApplication::restoreOverrideCursor();
	} else {
		isAccumulated = false;
		curveAccumulated->detach();
		replot();
	}
}

void Ploter::getFileNames(const QString & _fileName) {
	fullFileName = _fileName;
	fileName = QFileInfo(fullFileName).fileName(); 	// retorna a string existente apÃ³s a Ãºltima /
	curFileNoExt = fileName.section('.', 0, -2); 		// retorna o nome do arquivo sem a extensÃ£o
	fileExt = QFileInfo(fullFileName).suffix(); 	// retorna a string existente apÃ³s o Ãºltimo .
	filePath = QFileInfo(fullFileName).canonicalPath() + "/";
	if ( isNew ) {  										// é um novo arquivo.
		fileName = fileName.remove(0,1); 						// remove o ponto no início do nome do arquivo
	}
}

bool Ploter::save( QString * lop ) {
	if (isNew) {
		return saveAs( lop );
	}/* else {
	if( ! pm )
	pm = new TCMatriz2D<int>(fullCurFile.toStdString());
	if(pm->Write(curFile.toStdString())) {
	return true;
	} else {
	return false;
	}
 }*/
	return false;
}

bool Ploter::saveAs( QString * _lop ) {
	QString newFileName;
	if ( fileExt == "cor" || fileExt == "COR" ) {
		newFileName = QFileDialog::getSaveFileName(this, tr("Save As"), tr("%1%2").arg(_lop->left(_lop->lastIndexOf("/")+1)).arg(fileName), tr("Correlation Files (*.cor)"));
	} else if( fileExt == "dtp" || fileExt == "DTP") {
		newFileName = QFileDialog::getSaveFileName(this, tr("Save As"), tr("%1%2").arg(_lop->left(_lop->lastIndexOf("/")+1)).arg(fileName), tr("Distribution Files (*.dtp)"));
	} else if( fileExt == "dts" || fileExt == "DTS") {
		newFileName = QFileDialog::getSaveFileName(this, tr("Save As"), tr("%1%2").arg(_lop->left(_lop->lastIndexOf("/")+1)).arg(fileName), tr("Distribution Files (*.dts)"));
	} else if( fileExt == "rpc" || fileExt == "RPC") {
		newFileName = QFileDialog::getSaveFileName(this, tr("Save As"), tr("%1%2").arg(_lop->left(_lop->lastIndexOf("/")+1)).arg(fileName), tr("Relative Permeability Curves (*.rpc)"));
	}
	if ( newFileName.isEmpty() ) {
		return false;
	}
	*_lop = newFileName;
	//QDir dir(newFileName);
	//*_lop = dir.canonicalPath();
	//cerr << "newFileName: " << newFileName.toStdString() << endl;
	//cerr << "_lop: " << _lop->toStdString() << endl;
	//cerr << "dir.canonicalPath(): " << dir.canonicalPath().toStdString() << endl;
	if (QFile::copy(fullFileName, newFileName)){
		if (isNew) {
			QFile::remove(fullFileName);
		}
		isNew = false;
		if ( loadFile(newFileName) ){
			return true;
		} else {
			QMessageBox::information(this, tr("LVP"), tr("Erro! - Was not possible to open the file!"));
			return false;
		}
	} else {
		QMessageBox::information(this, tr("LVP"), tr("Erro! - Was not possible to copy the file!"));
		return false;
	}
}

void Ploter::closeEvent(QCloseEvent *event) {
	if (maybeSave()) {
		if(isNew){
			QFile::remove(fullFileName);
		}
		event->accept();
	} else {
		event->ignore();
	}
}

bool Ploter::maybeSave() {
	if(isNew){
		QMessageBox::StandardButton ret;
		ret = QMessageBox::warning(parent, tr("LVP"), tr("'%1' isn't save.\n""Do you want to save it?").arg(getFileName()), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		if (ret == QMessageBox::Save) {
			static QString lop("./");
			return save( & lop );
		} else if (ret == QMessageBox::Cancel) {
			return false;
		}
	}
	return true;
}

void Ploter::labelPos(const QPoint &pos) {
	QString info;
	info.sprintf("Last active position:  X=%g , Y=%g",
							 this->invTransform(QwtPlot::xBottom, pos.x()),
							 this->invTransform(QwtPlot::yLeft, pos.y()));
	parent->statusBar()->showMessage(info, 9999);
}
