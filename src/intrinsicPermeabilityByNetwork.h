#ifndef INTRINSICPERMEABILITYBYNETWORK_H
#define INTRINSICPERMEABILITYBYNETWORK_H

#include "ui_dialogIntrinsicPermeabilityByNetwork.h"
#include "dbmImageViewer.h"
#include "dgmImageViewer.h"
#include "textEditor.h"

#include <QMainWindow>

/*! PoresThroats é a classe que implementa a janela de diálogo para segmentação de poros e gargantas. */
class IntrinsicPermeabilityByNetwork : public QDialog, public Ui::DialogIntrinsicPermeabilityByNetwork
{
		Q_OBJECT
		
	public:
		explicit IntrinsicPermeabilityByNetwork(QMainWindow * _parent, DbmImageViewer * _child);
		explicit IntrinsicPermeabilityByNetwork(QMainWindow * _parent, DgmImageViewer * _child);
		explicit IntrinsicPermeabilityByNetwork(QMainWindow * _parent, TextEditor * _child);
		~IntrinsicPermeabilityByNetwork(){}

		TextEditor * childRSL;
		DbmImageViewer * child;
		DgmImageViewer * childInt;

	private:
		 QMainWindow *parent;
};

#endif // PORESTHROATS_H
