#ifndef NEURALNETWORKCONFIG_H
#define NEURALNETWORKCONFIG_H

#include "ui_neuralNetworkConfig.h"
#include <QMainWindow>

class NeuralNetworkConfig : public QDialog, public Ui::NeuralNetworkConfig
{
		Q_OBJECT
		
	public:
		explicit NeuralNetworkConfig(QMainWindow * _parent);
		~NeuralNetworkConfig(){}

	private:
		 QMainWindow *parent;
};

#endif // NEURALNETWORKCONFIG_H
