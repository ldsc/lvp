#include "dialogGrayTons.h"

DialogGrayTons::DialogGrayTons(QStringList & _qStrList, QWidget *parent) : QDialog(parent) {
	setupUi(this);
	setTonsList(_qStrList);
	connect(checkBox, SIGNAL(clicked()), this, SLOT(selectAll()));
	connect(pushButton,  SIGNAL(released()), this, SLOT(invertSelection()));
	connect(listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(updateItem(QListWidgetItem*)));
}

void DialogGrayTons::updateItem(QListWidgetItem* item){
	if (item->checkState()==0)
		item->setCheckState(Qt::Checked);
	else
		item->setCheckState(Qt::Unchecked);
	//updateCheckBox();
	listWidget->itemSelectionChanged();
}

void DialogGrayTons::updateCheckBox(){
	bool allChecked = true;
	for (int r=0; r<listWidget->count() and allChecked; ++r) {
		QListWidgetItem *item = listWidget->item(r);
		if (item->checkState()==0)
			allChecked = false;
	}
	if (allChecked)
		checkBox->setCheckState(Qt::Checked);
	else
		checkBox->setCheckState(Qt::Unchecked);
}

void DialogGrayTons::setTonsList(QStringList & qStrList) {
	foreach (QString v, qStrList) {
		QListWidgetItem *item = new QListWidgetItem(v, listWidget)	;
		item->setCheckState(Qt::Checked);
	}
}

void DialogGrayTons::selectAll(int i) {
	for (int r=0; r<listWidget->count(); ++r) {
		QListWidgetItem *item = listWidget->item(r);
		item->setCheckState((Qt::CheckState)i);
	}
}

void DialogGrayTons::selectAll() {
	Qt::CheckState cs = checkBox->checkState();
	for (int r=0; r<listWidget->count(); ++r) {
		QListWidgetItem *item = listWidget->item(r);
		item->setCheckState(cs);
	}
}

void DialogGrayTons::invertSelection() {
	for (int r=0; r<listWidget->count(); ++r) {
		QListWidgetItem *item = listWidget->item(r);
		if (item->checkState()==0)
			item->setCheckState(Qt::Checked);
		else
			item->setCheckState(Qt::Unchecked);
	}
	updateCheckBox();
}
