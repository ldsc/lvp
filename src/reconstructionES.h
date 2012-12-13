#ifndef RECONSTRUCTIONES_H
#define RECONSTRUCTIONES_H

#include "ui_dialogReconstructionES.h"

class QMainWindow;

/*! ReconstructionES é a classe que implementa a janela de diálogo para o método de reconstrução tridimensional Esferas Sobrepostas. */
class ReconstructionES : public QDialog, public Ui::DialogReconstructionES
{
	Q_OBJECT
 public:
	ReconstructionES( QString & _filepath, QMainWindow * _parent );
	virtual ~ReconstructionES(){}

	void readInfo();

 private slots:
	void change();

 //protected:


 private:
	  QMainWindow *parent;
};

#endif
