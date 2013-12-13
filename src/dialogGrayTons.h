#ifndef DIALOGGRAYTONS_H
#define DIALOGGRAYTONS_H

#include "ui_dialogGrayTons.h"

class DialogGrayTons : public QDialog, public Ui::DialogGrayTons
{
		Q_OBJECT

	public:
		explicit DialogGrayTons(QStringList &_qStrList, QWidget *parent = 0);
		void setTonsList(QStringList & qStrList);

	public slots:
		void selectAll(int i);
		void selectAll();
		void invertSelection();
		void updateItem(QListWidgetItem* item);
		void updateCheckBox();
};

#endif // DIALOGGRAYTONS_H
