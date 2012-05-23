#ifndef CONFEQ_H
#define CONFEQ_H

#include "ui_dialogConfEq.h"
#include "baseImageViewer.h"
#include "imageviewer.h"
#include "imageViewer3D.h"

class QMainWindow;

class ConfEq : public QDialog, public Ui::DialogConfEq
{
		Q_OBJECT

 public:
		ConfEq( QMainWindow * _parent, BaseImageViewer *_obj );
		~ConfEq(){};

//      QCheckBox *checkBoxSalvarP;

// private slots:
//     void open(std::string file="0");
// protected:
//     void closeEvent(QCloseEvent *event);
 private:
	  QMainWindow *parent;
	  BaseImageViewer *obj;
};

#endif
