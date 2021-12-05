#ifndef NEURALNETWORKCONFIG_H
#define NEURALNETWORKCONFIG_H

#include "ui_neuralNetworkConfig.h"
#include <QMainWindow>
#include <QComboBox>
#include <QVector>
#include <QStringList>

struct LayerConfig
{
	int numNeurons;
	QString layerType, layerFunction;
};

class NeuralNetworkConfig : public QDialog, public Ui::NeuralNetworkConfig
{
	Q_OBJECT

public:
	explicit NeuralNetworkConfig(QMainWindow *_parent);
	inline QVector<LayerConfig> getLayerConfigData() { return layerConfigData; }
	~NeuralNetworkConfig() {}

private:
	QComboBox *createTableCombobox(QStringList optionsList);

private slots:
	void on_actionAddLayer_clicked();

    void on_buttonBox_accepted();

private:
	QMainWindow *parent;
	QVector<LayerConfig> layerConfigData;
};

#endif // NEURALNETWORKCONFIG_H
