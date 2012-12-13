#ifndef RECONSTRUCTION_H
#define RECONSTRUCTION_H

#include "ui_dialogReconstruction.h"

class QMainWindow;

/*! Reconstruction é a classe que implementa a janela de diálogo para o método de reconstrução tridimensional Gaussiana Truncada. */
class Reconstruction : public QDialog, public Ui::DialogReconstruction
{
	Q_OBJECT
 public:
	Reconstruction( QString & _filepath, QMainWindow * _parent );
   virtual ~Reconstruction(){}
      
 private slots:
	void change();
     
// protected:

 private:	
     QMainWindow *parent; 
};

#endif
