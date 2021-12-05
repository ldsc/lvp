#include <QtGui>
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

    int rowNumber = layersTable->rowCount();

    layersTable->insertRow(layersTable->rowCount());
    layersTable->setItem(rowNumber, 0, new QTableWidgetItem("1"));
    layersTable->setCellWidget(rowNumber, 1, typeComboBox);
    layersTable->setCellWidget(rowNumber, 2, funcComboBox);
}

void NeuralNetworkConfig::on_buttonBox_accepted()
{
    LayerConfig layerConfig;
    for (int i = 0; i < layersTable->rowCount(); i++)
    {
        layerConfig.numNeurons = layersTable->item(i, 0)->text().toInt();
        layerConfig.layerType = ((QComboBox*) layersTable->cellWidget(i, 1))->currentText();
        layerConfig.layerFunction = ((QComboBox*) layersTable->cellWidget(i, 2))->currentText();

        layerConfigData.append(layerConfig);
    }     
}
