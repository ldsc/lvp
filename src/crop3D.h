#ifndef CROP3D_H
#define CROP3D_H

#include <QtGui>
#include "ui_dialogCrop3D.h"

#include <QMainWindow>

/*! Crop3D é a classe que implementa a janela de diálogo para cortar imagens 3D. */
class Crop3D : public QDialog, public Ui::Crop3D
{
		Q_OBJECT
		
	public:
		explicit Crop3D(QMainWindow *_parent = 0);

		~Crop3D();

	private slots:
		void updateEndX();
		void updateEndY();
		void updateEndZ();
		void updateStartX();
		void updateStartY();
		void updateStartZ();
		
	private:
		QMainWindow *parent;
};

#endif // CROP3D_H
