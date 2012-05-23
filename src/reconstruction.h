#ifndef RECONSTRUCTION_H
#define RECONSTRUCTION_H

#include "ui_dialogReconstruction.h"

class QMainWindow;

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
