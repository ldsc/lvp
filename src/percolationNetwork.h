#ifndef PERCOLATIONNETWORK_H
#define PERCOLATIONNETWORK_H

#include "ui_dialogPercolationNetwork.h"
#include "dbmImageViewer.h"
#include "dgmImageViewer.h"

#include <QMainWindow>

/*! PoresThroats é a classe que implementa a janela de diálogo para segmentação de poros e gargantas. */
class PercolationNetwork : public QDialog, public Ui::DialogPercolationNetwork
{
		Q_OBJECT
		
	public:
		explicit PercolationNetwork(QMainWindow * _parent, DbmImageViewer * _child);
		explicit PercolationNetwork(QMainWindow * _parent, DgmImageViewer * _child);
		~PercolationNetwork(){}

		DbmImageViewer * child;
		DgmImageViewer * childInt;
		
	private:
		 QMainWindow *parent;
};

#endif // PORESTHROATS_H
