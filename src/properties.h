#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <QtGui>
#include "ui_dialogProperties.h"

#include <QMainWindow>

/*! Properties é a classe que implementa a janela de diálogo para visualizar/setar propriedades em imagens 3D. */
class Properties : public QDialog, public Ui::DialogProperties
{
		Q_OBJECT
		
	public:
		explicit Properties(QMainWindow *_parent = 0);

		~Properties();

	private:
		QMainWindow *parent;
};

#endif // PROPERTIES_H
