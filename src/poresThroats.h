#ifndef PORESTHROATS_H
#define PORESTHROATS_H

#include "ui_dialogPoresThroats.h"
#include "dbmImageViewer.h"

#include <QMainWindow>

/*! PoresThroats é a classe que implementa a janela de diálogo para segmentação de poros e gargantas. */
class PoresThroats : public QDialog, public Ui::DialogPoresThroats
{
		Q_OBJECT
		
	public:
		explicit PoresThroats(QMainWindow * _parent, DbmImageViewer * _child);
		~PoresThroats(){}

		DbmImageViewer * child;
		
	private:
		 QMainWindow *parent;
};

#endif // PORESTHROATS_H
