#include <QtGui>
#include "neuralNetworkConfig.h"

NeuralNetworkConfig::NeuralNetworkConfig(QMainWindow *_parent) 
{
    setupUi(this);
	parent = _parent;
}