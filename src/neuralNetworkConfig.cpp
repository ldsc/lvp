#include <QtGui>
#include <QLineEdit>
#include <QIntValidator>
#include <limits>
#include <QMessageBox>
#include <QDebug>
#include "neuralNetworkConfig.h"

NeuralNetworkConfig::NeuralNetworkConfig(QMainWindow *_parent) 
{
    setupUi(this);
	parent = _parent;
}

QComboBox* NeuralNetworkConfig::createTableCombobox(QStringList optionsList) 
{
    QComboBox *comboBox = new QComboBox();
    comboBox->addItems(optionsList);

    return comboBox;
}

void NeuralNetworkConfig::on_actionAddLayer_clicked()
{
    QStringList layerTypes = {"Input", "Deep", "Output"};
    QComboBox *typeComboBox = createTableCombobox(layerTypes);

    QStringList layerFunctions = {"Liner", "ReLU", "Sigmoid"};
    QComboBox *funcComboBox = createTableCombobox(layerFunctions);

    QSpinBox *numNeurons = new QSpinBox();
    numNeurons->setValue(1);
    numNeurons->setMinimum(1);
    numNeurons->setMaximum(std::numeric_limits<int>::max());

    int rowNumber = layersTable->rowCount();

    layersTable->insertRow(layersTable->rowCount());
    layersTable->setCellWidget(rowNumber, 0, numNeurons);
    layersTable->setCellWidget(rowNumber, 1, typeComboBox);
    layersTable->setCellWidget(rowNumber, 2, funcComboBox);

    layersTable->setTabKeyNavigation(true);

}

void NeuralNetworkConfig::on_buttonBox_accepted()
{
    LayerConfig layerConfig;
    for (int i = 0; i < layersTable->rowCount(); i++)
    {
        layerConfig.numNeurons = ((QSpinBox*) layersTable->cellWidget(i, 0))->value();
        layerConfig.layerType = ((QComboBox*) layersTable->cellWidget(i, 1))->currentText();
        layerConfig.layerFunction = ((QComboBox*) layersTable->cellWidget(i, 2))->currentText();

        layerConfigData.append(layerConfig);
    }     
}

void NeuralNetworkConfig::on_actionResetLayers_clicked()
{
    QMessageBox::StandardButton replay;
	replay = QMessageBox::question(this, "LVP", "Are you sure?", QMessageBox::Yes | QMessageBox::No);

	if (replay == QMessageBox::Yes)
	{
		layersTable->clearContents();
		layersTable->setRowCount(0);
	}
}

void NeuralNetworkConfig::onKeyPressEvent(QKeyEvent *event) 
{
    qDebug() << event->key();
}
