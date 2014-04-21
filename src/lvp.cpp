#include <QtGui>
#include <string>
#include <set>
#include <cstdlib> // Utilizada em Correlation
#include "lvp.h"
//Bibliotecas LIB_LDSC
#include <AnaliseImagem/Simulacao/ConfiguracaoEquilibrio/CConfiguracoesEquilibrio2D.h>
#include <AnaliseImagem/Simulacao/ConfiguracaoEquilibrio/CConfiguracoesEquilibrio3D.h>
#include <AnaliseImagem/Caracterizacao/Correlacao/CCorrelacaoFFT.h>
#include <AnaliseImagem/Caracterizacao/Correlacao/CCorrelacaoEspacial.h>
#include <AnaliseImagem/Caracterizacao/Distribuicao/CDistribuicao.h>
#include <AnaliseImagem/Caracterizacao/Distribuicao/CDistribuicao3D.h>
#include <AnaliseImagem/Caracterizacao/Distribuicao/CDistribuicaoTamanhoPorosGargantas.h>
#include <AnaliseImagem/Filtro/FEspacial/TCFEPassaAlta.h>
#include <AnaliseImagem/Filtro/FEspacial/TCFEPassaBaixa.h>
#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMorfologiaMatematica.h>
#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIRA.h>
#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFd4.h>
#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFd8.h>
#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFd34.h>
#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFd5711.h>
#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFEuclidiana.h>
//#include <AnaliseImagem/Filtro/FEspacial/FEInversao/CFEInversao.h>
#include <AnaliseImagem/Filtro/FEspacial/FEEsqueleto/CFEEsqueletoV1.h>
#include <AnaliseImagem/Filtro/FEspacial/FEEsqueleto/CFEEsqueletoV2.h>
#include <AnaliseImagem/Filtro/FEspacial/FEEsqueleto/CFEEsqueletoV3.h>
#include <AnaliseImagem/Filtro/FEspacial/FEEsqueleto/CFEEsqueletoV4.h>
#include <AnaliseImagem/Filtro/FEspacial/FEEsqueleto/CFEEZhangSuen.h>
#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIRA3D.h>
#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFd3453D.h>
#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFEuclidiana3D.h>
#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMorfologiaMatematica3D.h>
#include <AnaliseImagem/Filtro/FEspacial/FEConectividade/TCFEConectividade3D.h>
//#include <AnaliseImagem/Filtro/FEspacial3D/FEInversao3D/CFEInversao3D.h>
#include <MetNum/Matriz/TCMatriz2D.h>
#include <MetNum/Matriz/TCMatriz3D.h>
#include <MetNum/Matriz/TCImagem3D.h>
#include <AnaliseImagem/Simulacao/Permeabilidade/CSimPermeabilidadeRelativa.h>
#include <AnaliseImagem/Simulacao/Permeabilidade/CSimPermeabilidadeIntrinseca.h>
#include <AnaliseImagem/Simulacao/Permeabilidade/CPermeabilidadeIntrinsecaByRede.h>
#include <AnaliseImagem/Reconstrucao/CReconstrucaoBueno.h>
#include <AnaliseImagem/Reconstrucao/CReconstrucaoEsferas.h>
#include <AnaliseImagem/Filtro/FEspacial/FERotulagem/TCRotulador2D.h>
#include <AnaliseImagem/Segmentacao/PorosGargantas/CAberturaDilatacao.h>
#include <AnaliseImagem/Segmentacao/PorosGargantas/CAberturaDilatacao3D.h>
#include <AnaliseImagem/Segmentacao/PorosGargantas/CSegPorosGargantas3D.h>

using namespace std;

Lvp::Lvp() {
	setupUi(this); // this sets up GUI
	
	mdiArea = new QMdiArea;
	mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	mdiArea->setViewMode( QMdiArea::TabbedView );
	mdiArea->setTabsClosable(true);
	setCentralWidget(mdiArea);

	fileWatcher = new QFileSystemWatcher(this);
	
	//Lista de imagens 2D
	listWidget = new QListWidget(dockWidgetLista);
	listWidget->setFocusPolicy( Qt::NoFocus );
	listWidget->setSortingEnabled ( true );
	listWidget->setAlternatingRowColors ( true );
	listWidget->setSelectionMode( QAbstractItemView::ExtendedSelection );
	dockWidgetLista->setWidget(listWidget);
	dockWidgetLista->setVisible( false );
	
	//Lista de imagens 3D
	gridLayout = new QGridLayout(dockWidgetContents3D);
	gridLayout->setSizeConstraint(QLayout::SetMinimumSize);
	gridLayout->setContentsMargins(0, 0, 0, 0);
	gridLayout->setColumnMinimumWidth(1, 90);
	gridLayout->setRowMinimumHeight(0, 20);
	gridLayout->setRowMinimumHeight(1, 25);
	gridLayout->setRowMinimumHeight(2, 25);
	
	labelPlano3D = new QLabel(dockWidgetContents3D);
	labelPlano3D->setAlignment(Qt::AlignCenter);
	labelPlano3D->setWordWrap(true);
	labelPlano3D->setText(QString::fromUtf8("Plan of the 3D Image"));
	gridLayout->addWidget(labelPlano3D, 0, 0, 1, 1);
	
	groupBox = new QGroupBox(dockWidgetContents3D);
	groupBox->setObjectName(QString::fromUtf8("groupBox"));
	groupBox->setTitle(QApplication::translate("Form", "axis", 0, -1));
	
	radioButtonX = new QRadioButton(groupBox);
	radioButtonX->setGeometry(QRect(0, 55, 41, 25));
	radioButtonX->setText(QApplication::translate("Form", "X", 0, -1));
	
	radioButtonY = new QRadioButton(groupBox);
	radioButtonY->setGeometry(QRect(20, 18, 41, 25));
	radioButtonY->setText(QApplication::translate("Form", "Y", 0, -1));
	
	radioButtonZ = new QRadioButton(groupBox);
	radioButtonZ->setGeometry(QRect(45, 45, 41, 25));
	radioButtonZ->setText(QApplication::translate("Form", "Z", 0, -1));
	radioButtonZ->setChecked(true);
	
	labelAxis = new QLabel(groupBox);
	labelAxis->setGeometry(QRect(19, 37, 30, 30));
	labelAxis->setPixmap(QPixmap(QString::fromUtf8(":/images/3d.png")));
	labelAxis->setScaledContents(true);
	gridLayout->addWidget(groupBox, 0, 1, 3, 1);
	
	spinBoxPlano3D = new QSpinBox(dockWidgetContents3D);
	gridLayout->addWidget(spinBoxPlano3D, 1, 0, 1, 1);
	
	horizontalSliderPlano3D = new QSlider(dockWidgetContents3D);
	horizontalSliderPlano3D->setOrientation(Qt::Horizontal);
	gridLayout->addWidget(horizontalSliderPlano3D, 2, 0, 1, 1);
	
	listWidget3D = new QListWidget(dockWidgetContents3D);
	listWidget3D->setFocusPolicy( Qt::NoFocus );
	listWidget3D->setSortingEnabled ( true );
	listWidget3D->setAlternatingRowColors ( true );
	gridLayout->addWidget(listWidget3D, 3, 0, 1, 2);
	dockWidgetLista3D->setVisible( false );
	
	//Controles da visualização 3D
	dockWidgetVisualisation3D->setVisible( false );
	
	//Lista de gráficos
	gridLayoutChart = new QGridLayout(dockWidgetContentsListaChart);
	gridLayoutChart->setSizeConstraint(QLayout::SetMinimumSize);
	gridLayoutChart->setContentsMargins(0, 0, 0, 0);
	gridLayoutChart->setColumnMinimumWidth(1, 90);
	gridLayoutChart->setRowMinimumHeight(0, 20);
	gridLayoutChart->setRowMinimumHeight(1, 25);
	gridLayoutChart->setRowMinimumHeight(2, 25);
	
	pushButtonAddCurve = new QPushButton(dockWidgetContentsListaChart);
	pushButtonAddCurve->setText(QString::fromUtf8("Add Curve"));
	pushButtonAddCurve->setEnabled( false );
	gridLayoutChart->addWidget(pushButtonAddCurve, 0, 0, 1, 1);
	
	pushButtonAverage = new QPushButton(dockWidgetContentsListaChart);
	pushButtonAverage->setText(QString::fromUtf8("Average"));
	pushButtonAverage->setEnabled( false );
	gridLayoutChart->addWidget(pushButtonAverage, 1, 1, 1, 1);
	
	pushButtonAccumulated = new QPushButton(dockWidgetContentsListaChart);
	pushButtonAccumulated->setText(QString::fromUtf8("Show Accumulated"));
	pushButtonAccumulated->setEnabled( false );
	gridLayoutChart->addWidget(pushButtonAccumulated, 1, 0, 1, 1);
	
	pushButtonSource = new QPushButton(dockWidgetContentsListaChart);
	pushButtonSource->setText(QString::fromUtf8("Source"));
	pushButtonSource->setEnabled( false );
	gridLayoutChart->addWidget(pushButtonSource, 0, 1, 1, 1);
	
	listWidgetChart = new QListWidget(dockWidgetContentsListaChart);
	listWidgetChart->setSortingEnabled ( true );
	listWidgetChart->setAlternatingRowColors ( true );
	listWidgetChart->setSelectionMode( QAbstractItemView::ExtendedSelection );
	gridLayoutChart->addWidget(listWidgetChart, 2, 0, 1, 2);
	dockWidgetListaChart->setVisible( false );
	
	//Lista de editores
	listWidgetEditor = new QListWidget(dockWidgetListaTextEditor);
	listWidgetEditor->setFocusPolicy( Qt::NoFocus );
	listWidgetEditor->setSortingEnabled ( true );
	listWidgetEditor->setAlternatingRowColors ( true );
	listWidgetEditor->setSelectionMode( QAbstractItemView::ExtendedSelection );
	dockWidgetListaTextEditor->setWidget(listWidgetEditor);
	dockWidgetListaTextEditor->setVisible( false );

	//Dock de mensagens
	textEditMessages = new QPlainTextEdit(dockWidgetMessages);
	dockWidgetMessages->setWidget(textEditMessages);


	//Redirecionando cout e cerr para QTextEdit
	//qout = new QDebugStream(std::cout, textEditMessages);
	//qerr = new QDebugStream(std::cerr, textEditMessages);

	//Objeto para salvar as preferências do usuário
	settings = new QSettings("LENEP", "LVP");
	
	//Traduções do software
	qApp->installTranslator(&appTranslator);
	qmPath = qApp->applicationDirPath() + "/translations";
	
	//Inicializando demais atributos
	dialog = nullptr;
	dialogGT = nullptr;
	dialogES = nullptr;
	dialogImport = nullptr;
	dialogHexEditor = nullptr;
	lastOpenPath = "";
	
	readSettings();
	createActions();
	updateMenus();
	createLanguageMenu();
	
	retranslateUi(this);
	setUnifiedTitleAndToolBarOnMac(true);
	setAcceptDrops(true);
}

void Lvp::dragEnterEvent(QDragEnterEvent *event) {
	event->acceptProposedAction();
}
/*
void Lvp::dragMoveEvent(QDragMoveEvent *event) {
	event->acceptProposedAction();
}
*/
void Lvp::dropEvent(QDropEvent *event) {
	const QMimeData *mimeData = event->mimeData();
	if (mimeData->hasUrls()) {
		QList<QUrl> urlList = mimeData->urls();
		for (int i = 0; i < urlList.size(); ++i) {
			//open(urlList.at(i).path().toStdString(), false);
			open(urlList.at(i).toLocalFile().toStdString(), false);
		}
	} else {
		QMessageBox::information(this, tr("LVP"), tr("Invalid file type!"));
	}
	event->acceptProposedAction();
}

//Conexões de sinais com slots
void Lvp::createActions() {
	windowMapper = new QSignalMapper(this);
	connect( windowMapper,													SIGNAL( mapped(QWidget *) ),										this, SLOT( setActiveSubWindow(QWidget *)) );
	connect( fileWatcher,														SIGNAL( fileChanged(QString) ),									this, SLOT( treatFileChanged(QString) ) );
	connect( listWidget,														SIGNAL( itemDoubleClicked(QListWidgetItem *) ), this, SLOT( setActiveSubWindow(QListWidgetItem *)) );
	connect( listWidget3D,													SIGNAL( itemDoubleClicked(QListWidgetItem *) ), this, SLOT( setActiveSubWindow(QListWidgetItem *)) );
	connect( listWidgetChart,												SIGNAL( itemDoubleClicked(QListWidgetItem *) ), this, SLOT( setActiveSubWindow(QListWidgetItem *)) );
	connect( listWidgetEditor,											SIGNAL( itemDoubleClicked(QListWidgetItem *) ), this, SLOT( setActiveSubWindow(QListWidgetItem *)) );
	connect( mdiArea,																SIGNAL( subWindowActivated(QMdiSubWindow *) ),	this, SLOT( updateMenus()) );
	connect( mdiArea,																SIGNAL( subWindowActivated(QMdiSubWindow *) ),	this, SLOT( updateDockLista()) );
	connect( mdiArea,																SIGNAL( subWindowActivated(QMdiSubWindow *) ),	this, SLOT( updateStatusBar()) );
	connect( spinBox_x,															SIGNAL( valueChanged(int) ),										this, SLOT( exChangePlanX(int)) );
	connect( spinBox_y,															SIGNAL( valueChanged(int) ),										this, SLOT( exChangePlanY(int)) );
	connect( spinBox_z,															SIGNAL( valueChanged(int) ),										this, SLOT( exChangePlanZ(int)) );
	connect( spinBoxPlano3D,												SIGNAL( valueChanged(int) ),										this, SLOT( exChangePlan(int)) );
	connect( spinBoxPlano3D,												SIGNAL( valueChanged(int) ), horizontalSliderPlano3D, SLOT(setValue(int)) );
	connect( horizontalSliderPlano3D,								SIGNAL( valueChanged(int) ), spinBoxPlano3D,					SLOT(setValue(int)) );
	connect( radioButtonX,													SIGNAL( released() ),		this,			SLOT( exChangeAxis()) );
	connect( radioButtonY,													SIGNAL( released() ),		this,			SLOT( exChangeAxis()) );
	connect( radioButtonZ,													SIGNAL( released() ),		this,			SLOT( exChangeAxis()) );
	connect( menuWindow,														SIGNAL( aboutToShow()), this,			SLOT( updateWindowMenu()					) );
	connect( pushButtonAddCurve,										SIGNAL( clicked()   ),  this,			SLOT( addCurve()									) );
	connect( pushButtonAverage,											SIGNAL( clicked()   ),  this,			SLOT( average()										) );
	connect( pushButtonAccumulated,									SIGNAL( clicked()   ),	this,			SLOT( accumulated()               ) );
	connect( pushButtonSource,											SIGNAL( clicked()   ),  this,			SLOT( openEditor()								) );
	connect( actionAbaut,														SIGNAL( triggered() ), 	this,			SLOT( about()											) );
	connect( actionAbautQt,													SIGNAL( triggered() ),	qApp,			SLOT( aboutQt()										) );
	connect( actionCascade,													SIGNAL( triggered() ), 	mdiArea,	SLOT( cascadeSubWindows() 	      ) );
	connect( actionClose,														SIGNAL( triggered() ), 	this,			SLOT( closeActiveSubWindow()      ) );
	connect( actionCloseAll,												SIGNAL( triggered() ), 	this,			SLOT( closeAllSubWindows()				) );
	connect( actionClosing,													SIGNAL( triggered() ), 	this,			SLOT( closing()										) );
	connect( actionClosing3D,												SIGNAL( triggered() ), 	this,			SLOT( closing3D()									) );
	connect( actionConnectedObjects,								SIGNAL( triggered() ),  this,			SLOT( connectedObjects() 					) );
	connect( actionConnectivity,										SIGNAL( triggered() ),  this,			SLOT( connectivity3D()						) );
	connect( actionConfEq,													SIGNAL( triggered() ), 	this,			SLOT( confEq()										) );
	connect( actionCopy,														SIGNAL( triggered() ), 	this,			SLOT( copy()											) );
	connect( actionCorFrequency,										SIGNAL( triggered() ),	this,			SLOT( correlationFFT()						) );
	connect( actionCorFrequency3D,									SIGNAL( triggered() ),	this,			SLOT( correlationFFT3D()          ) );
	connect( actionCorSpatial,											SIGNAL( triggered() ),	this,			SLOT( correlationSpatial()        ) );
	connect( actionCorSpatial3D,										SIGNAL( triggered() ),	this,			SLOT( correlationSpatial3D()      ) );
	connect( actionCrop3D,													SIGNAL( triggered() ),  this,			SLOT( crop3DImage()								) );
	connect( actionDilation,												SIGNAL( triggered() ), 	this,			SLOT( dilation()									) );
	connect( actionDilation3D,											SIGNAL( triggered() ), 	this,			SLOT( dilation3D()                ) );
	connect( actionDTPd34,													SIGNAL( triggered() ),	this,			SLOT( dtpD34()										) );
	connect( actionDTPd34_3D,												SIGNAL( triggered() ),	this,			SLOT( dtpD34_3D()                 ) );
	connect( actionDTPd345,													SIGNAL( triggered() ),	this,			SLOT( dtpD345()										) );
	connect( actionDTPd5711,												SIGNAL( triggered() ),	this,			SLOT( dtpD5711() 	                ) );
	connect( actionDTPd5711_3D,											SIGNAL( triggered() ),	this,			SLOT( dtpD5711_3D()               ) );
	connect( actionDTPeuclidian,										SIGNAL( triggered() ),	this,			SLOT( dtpEuclidian()							) );
	connect( actionDTPeuclidian3D,									SIGNAL( triggered() ),	this,			SLOT( dtpEuclidian3D()       	    ) );
	connect( actionDTPGd345_3D,											SIGNAL( triggered() ),	this,			SLOT( dtpgD345_3D()								) );
	connect( actionDTPspatial,											SIGNAL( triggered() ),	this,			SLOT( dtpSpatial()                ) );
	connect( actionDTPspatial3D,										SIGNAL( triggered() ),	this,			SLOT( dtpSpatial3D()              ) );
	connect( actionDTSd34,													SIGNAL( triggered() ),	this,			SLOT( dtsD34()										) );
	connect( actionDTSd34_3D,												SIGNAL( triggered() ),	this,			SLOT( dtsD34_3D()      	          ) );
	connect( actionDTSd345,													SIGNAL( triggered() ),	this,			SLOT( dtsD345()										) );
	connect( actionDTSd5711,												SIGNAL( triggered() ),	this,			SLOT( dtsD5711() 	                ) );
	connect( actionDTSd5711_3D,											SIGNAL( triggered() ),	this,			SLOT( dtsD5711_3D()               ) );
	connect( actionDTSeuclidian,										SIGNAL( triggered() ),	this,			SLOT( dtsEuclidian()							) );
	connect( actionDTSeuclidian3D,									SIGNAL( triggered() ),	this,			SLOT( dtsEuclidian3D() 	          ) );
	connect( actionDTSspatial,											SIGNAL( triggered() ),	this,			SLOT( dtsSpatial()                ) );
	connect( actionDTSspatial3D,										SIGNAL( triggered() ),	this,			SLOT( dtsSpatial3D()              ) );
	connect( actionErosion,													SIGNAL( triggered() ), 	this,			SLOT( erosion()										) );
	connect( actionErosion3D,												SIGNAL( triggered() ), 	this,			SLOT( erosion3D()									) );
	connect( actionExit,														SIGNAL( triggered() ), 	qApp,			SLOT( closeAllWindows()						) );
	connect( actionFitToWindow,											SIGNAL( triggered() ), 	this,			SLOT( fitToWindow() 		          ) );
	connect( actionHighPass,												SIGNAL( triggered() ), 	this,			SLOT( highPass()									) );
	connect( actionIDF,															SIGNAL( triggered() ),  this,			SLOT( idf()												) );
	connect( actionIDF3D,														SIGNAL( triggered() ),  this,			SLOT( idf()												) );
	connect( actionImport,													SIGNAL( triggered() ),  this,			SLOT( import()										) );
	connect( actionIntrinsicPermeability,						SIGNAL( triggered() ),  this,			SLOT( intrinsicPermeability()     ) );
	connect( actionIntrinsicPermeabilityByNetwork,	SIGNAL( triggered() ),  this,			SLOT( intrinsicPermeabilityByNetwork()     ) );
	connect( actionInversion,												SIGNAL( triggered() ),  this,			SLOT( inversion()									) );
	connect( actionInversion3D,											SIGNAL( triggered() ),  this,			SLOT( inversion3D() 		          ) );
	connect( actionInverter,												SIGNAL( triggered() ),  this,			SLOT( invertPoro()								) );
	connect( actionIRA,															SIGNAL( triggered() ),  this,			SLOT( ira()												) );
	connect( actionIRA3D,														SIGNAL( triggered() ),  this,			SLOT( ira()												) );
	connect( actionLowPass,													SIGNAL( triggered() ), 	this,			SLOT( lowPass()										) );
	connect( actionMPV,															SIGNAL( triggered() ),  this,			SLOT( openMPV()                   ) );
	connect( actionNext,														SIGNAL( triggered() ), 	mdiArea,	SLOT( activateNextSubWindow()     ) );
	connect( actionNormalSize,											SIGNAL( triggered() ), 	this,			SLOT( normalSize()								) );
	connect( actionOpen,														SIGNAL( triggered() ), 	this,			SLOT( open()											) );
	connect( actionOpening,													SIGNAL( triggered() ), 	this,			SLOT( opening()										) );
	connect( actionOpening3D,												SIGNAL( triggered() ), 	this,			SLOT( opening3D()									) );
	connect( actionOptions,													SIGNAL( triggered() ),  this,			SLOT( options()										) );
	connect( actionPNV,															SIGNAL( triggered() ),  this,			SLOT( openPNV()                   ) );
	connect( actionPoresThroats,										SIGNAL( triggered() ),  this,			SLOT( segmentationPoresThroats()	) );
	connect( actionPorosity,												SIGNAL( triggered() ),	this,			SLOT( porosity()									) );
	connect( actionPrevious,												SIGNAL( triggered() ), 	mdiArea,	SLOT( activatePreviousSubWindow() ) );
	connect( actionPrint,														SIGNAL( triggered() ), 	this,			SLOT( print()											) );
	connect( actionProperties,											SIGNAL( triggered() ), 	this,			SLOT( properties()								) );
	connect( actionRelativePermeability,						SIGNAL( triggered() ),  this,			SLOT( relativePermeability()      ) );
	connect( actionRotate,													SIGNAL( triggered() ),  this,			SLOT( rotate()										) );
	connect( actionSave,														SIGNAL( triggered() ), 	this,			SLOT( save()											) );
	connect( actionSaveAs,													SIGNAL( triggered() ), 	this,			SLOT( saveAs()										) );
	connect( actionSource,													SIGNAL( triggered() ), 	this,			SLOT( openEditor()								) );
	connect( actionSuperposedSpheres,								SIGNAL( triggered() ),	this,			SLOT( reconstructionES()					) );
	connect( actionTitle,														SIGNAL( triggered() ), 	mdiArea,	SLOT( tileSubWindows()						) );
	connect( actionTruncadeGaussiana,								SIGNAL( triggered() ),	this,			SLOT( reconstructionGT()					) );
	connect( actionViewMode,												SIGNAL( triggered() ), 	this,			SLOT( changeViewMode()						) );
	connect( actionV1,															SIGNAL( triggered() ), 	this,			SLOT( skeletonV1()								) );
	connect( actionV2,															SIGNAL( triggered() ), 	this,			SLOT( skeletonV2()								) );
	connect( actionV3,															SIGNAL( triggered() ), 	this,			SLOT( skeletonV3()								) );
	connect( actionV4,															SIGNAL( triggered() ), 	this,			SLOT( skeletonV4()								) );
	connect( actionZhangSuen,												SIGNAL( triggered() ), 	this,			SLOT( skeletonV5()								) );
	connect( actionZoomIn,													SIGNAL( triggered() ), 	this,			SLOT( zoomIn()										) );
	connect( actionZoomOut,													SIGNAL( triggered() ), 	this,			SLOT( zoomOut()										) );
	connect( action3DVisualization,									SIGNAL( triggered() ),  this,			SLOT( open3DVisualization()       ) );
}

void Lvp::closeActiveSubWindow() {
	QMdiSubWindow * subwindow = mdiArea->activeSubWindow();
	QWidget * widget = subwindow->widget();
	if ( BaseDnmImageViewer *child = qobject_cast<BaseDnmImageViewer *>(widget) ) {
		if (QMdiSubWindow * subwindowtemp = findGLWidget(child->getFullFileName())) {
			mdiArea->setActiveSubWindow(subwindowtemp);
			mdiArea->closeActiveSubWindow();
			mdiArea->setActiveSubWindow(subwindow);
		}
		fileWatcher->removePath( child->getFullFileName() );
	} else if (BasePnmImageViewer * child = qobject_cast<BasePnmImageViewer *>(widget)) {
		fileWatcher->removePath( child->getFullFileName() );
	} else if (Ploter * child = qobject_cast<Ploter *>(widget)) {
		fileWatcher->removePaths( child->pathCurves );
	} else if (TextEditor * child = qobject_cast<TextEditor *>(widget)) {
		if (child->getFileExt().toLower()=="txt" || child->getFileExt().toLower()=="rsl") //só remove o path se a extensão for txt ou rsl
			fileWatcher->removePath( child->getFullFileName() );
		if (QMdiSubWindow * subwindowtemp = findGLWidget(child->getFullFileName())) {
			mdiArea->setActiveSubWindow(subwindowtemp);
			mdiArea->closeActiveSubWindow();
			mdiArea->setActiveSubWindow(subwindow);
		}
	}
	mdiArea->closeActiveSubWindow();
}

void Lvp::closeAllSubWindows() {
	QWidget * widget;
	QMdiSubWindow * subWindow;
	QList<QMdiSubWindow *> list = mdiArea->subWindowList();
	foreach(subWindow, list) {
		widget = subWindow->widget();
		if ( BaseDnmImageViewer *child = qobject_cast<BaseDnmImageViewer *>(widget) ) {
			fileWatcher->removePath( child->getFullFileName() );
		} else if (BasePnmImageViewer * child = qobject_cast<BasePnmImageViewer *>(widget)) {
			fileWatcher->removePath( child->getFullFileName() );
		} else if (Ploter * child = qobject_cast<Ploter *>(widget)) {
			fileWatcher->removePaths( child->pathCurves );
		} else if (TextEditor * child = qobject_cast<TextEditor *>(widget)) {
			if (child->getFileExt().toLower()=="txt" || child->getFileExt().toLower()=="rsl") //só remove o path se a extensão for txt ou rsl
				fileWatcher->removePath( child->getFullFileName() );
		}
	}
	mdiArea->closeAllSubWindows();
}

void Lvp::treatFileChanged( QString _file ){
	QWidget * widget;
	QMdiSubWindow * subWindow;
	QList<QMdiSubWindow *> list = mdiArea->subWindowList();
	foreach(subWindow, list) {
		widget = subWindow->widget();
		if ( BaseDnmImageViewer *child = qobject_cast<BaseDnmImageViewer *>(widget) ) {
			if ( _file == child->getFullFileName() )
				child->reloadFile();
		} else if (BasePnmImageViewer * child = qobject_cast<BasePnmImageViewer *>(widget)) {
			if ( _file == child->getFullFileName() )
				child->reloadFile();
		} else if (Ploter * child = qobject_cast<Ploter *>(widget)) {
			if ( child->pathCurves.contains(_file) )
				child->reloadFile();
		} else if (TextEditor * child = qobject_cast<TextEditor *>(widget)) {
			if ( _file == child->getFullFileName() )
				child->reloadFile();
		}
	}
}

void Lvp::updateMenus() {
	//cerr << "updateMenus()" << endl;
	bool hasImageViewer			= false;
	bool hasDbmImageViewer	= false;
	//bool hasDgmImageViewer	= false;
	bool hasPbmImageViewer	= false;
	bool hasImageViewer3D		= false;
	bool hasGLWidget				= false;
	bool hasPloter					= false;
	bool hasTextEditor			= false;
	bool hasHexEditor				= false;
	QString ext = "";
	if ( active2DImageViewer() != 0 ) {
		hasImageViewer = true;
		if ( activePbmImageViewer() != 0 )
			hasPbmImageViewer = true;
	} else if ( active3DImageViewer() != 0 ) {
		hasImageViewer3D = true;
		if ( activeDbmImageViewer() != 0 ) {
			hasDbmImageViewer = true;
			actionSave->setEnabled( activeDbmImageViewer()->isModified );
		} else if ( activeDgmImageViewer() != 0 ) {
			//hasDgmImageViewer = true;
			actionSave->setEnabled( activeDgmImageViewer()->isModified );
		}
	} else if ( activeGLWidget() != 0 ) {
		hasGLWidget = true;
	} else if ( activePloter() != 0 ) {
		hasPloter = true;
		ext = activePloter()->getFileExt();
	} else if ( activeTextEditor() != 0 ) {
		hasTextEditor = true;
		actionSave->setEnabled( hasTextEditor && activeTextEditor()->document()->isModified() );
	} else if ( activeHexEditor() != 0 ) {
		hasHexEditor = true;
		actionSave->setEnabled( hasTextEditor && activeHexEditor()->IsModified() );
	}
	actionDTSspatial->setVisible(hasImageViewer);
	actionDTSspatial3D->setVisible(hasImageViewer3D);
	actionDTSd34->setVisible(hasImageViewer);
	actionDTSd34_3D->setVisible(hasImageViewer3D);
	actionDTSd345->setVisible(hasImageViewer3D);
	actionDTSd5711->setVisible(hasImageViewer);
	actionDTSd5711_3D->setVisible(hasImageViewer3D);
	actionDTSeuclidian->setVisible(hasImageViewer);
	actionDTSeuclidian3D->setVisible(hasImageViewer3D);
	actionDTPspatial->setVisible(hasImageViewer);
	actionDTPspatial3D->setVisible(hasImageViewer3D);
	actionDTPd34->setVisible(hasImageViewer);
	actionDTPd34_3D->setVisible(hasImageViewer3D);
	actionDTPd345->setVisible(hasImageViewer3D);
	actionDTPd5711->setVisible(hasImageViewer);
	actionDTPd5711_3D->setVisible(hasImageViewer3D);
	actionDTPeuclidian->setVisible(hasImageViewer);
	actionDTPeuclidian3D->setVisible(hasImageViewer3D);
	actionDTPGd345_3D->setEnabled(hasImageViewer3D);
	actionCascade->setEnabled(mdiArea->subWindowList().size() > 0);
	actionClose->setEnabled(mdiArea->subWindowList().size() > 0);
	actionCloseAll->setEnabled(mdiArea->subWindowList().size() > 0);
	actionClosing->setVisible(hasPbmImageViewer);
	actionClosing3D->setVisible(hasDbmImageViewer);
	actionConnectivity->setEnabled(hasDbmImageViewer);
	actionConnectedObjects->setEnabled(hasPbmImageViewer);
	actionConfEq->setEnabled(hasImageViewer || hasImageViewer3D);
	actionCopy->setEnabled(false);
	actionCorFrequency->setVisible(hasImageViewer);
	actionCorFrequency3D->setVisible(hasImageViewer3D);
	actionCorSpatial->setVisible (hasImageViewer);
	actionCorSpatial3D->setVisible (hasImageViewer3D);
	actionDilation->setVisible(hasPbmImageViewer);
	actionDilation3D->setVisible(hasDbmImageViewer);
	actionErosion->setVisible(hasPbmImageViewer);
	actionErosion3D->setVisible(hasDbmImageViewer);
	actionFitToWindow->setEnabled(hasImageViewer || hasImageViewer3D);
	actionFitToWindow->setChecked(actionFitToWindow->isEnabled() && activeImageViewer()->isFitedToWindow);
	actionHighPass->setEnabled(hasImageViewer);
	actionV1->setEnabled(hasImageViewer);
	actionV2->setEnabled(hasImageViewer);
	actionV3->setEnabled(hasImageViewer);
	actionV4->setEnabled(hasImageViewer);
	actionZhangSuen->setEnabled(hasImageViewer);
	actionIDF->setEnabled(hasPbmImageViewer);
	actionIRA->setEnabled(hasPbmImageViewer);
	actionIDF3D->setEnabled(hasDbmImageViewer);
	actionIRA3D->setEnabled(hasDbmImageViewer);
	actionInversion->setEnabled(hasPbmImageViewer);
	actionInversion3D->setEnabled(hasDbmImageViewer);
	actionInverter->setEnabled(hasGLWidget && ( (activeGLWidget()->getPM3D() != nullptr) || (activeGLWidget()->getPM3Di() != nullptr)) );
	actionLowPass->setEnabled(hasImageViewer);
	actionMPV->setEnabled( hasImageViewer3D );
	actionPNV->setEnabled( hasTextEditor && activeTextEditor()->getFileExt().toLower()=="rsl" );
	actionNext->setEnabled(mdiArea->subWindowList().size() > 1);
	actionNormalSize->setEnabled(!actionFitToWindow->isChecked() && ( hasImageViewer || hasImageViewer3D ));
	actionOpening->setVisible(hasPbmImageViewer);
	actionOpening3D->setVisible(hasDbmImageViewer);
	actionPorosity->setEnabled(hasImageViewer || hasImageViewer3D);
	actionPrevious->setEnabled(mdiArea->subWindowList().size() > 1);
	actionPrint->setEnabled(hasImageViewer || hasImageViewer3D);
	actionProperties->setEnabled(hasImageViewer || hasImageViewer3D);
	actionRelativePermeability->setEnabled(hasImageViewer3D);
	actionIntrinsicPermeability->setEnabled(hasImageViewer3D);
	actionIntrinsicPermeability->setEnabled(hasImageViewer3D);
	actionRotate->setEnabled( hasImageViewer3D || hasImageViewer );
	actionSaveAs->setEnabled( hasImageViewer || hasImageViewer3D || hasPloter || hasTextEditor || hasHexEditor );
	actionSeparator->setVisible(mdiArea->subWindowList().size() > 0);
	actionSource->setEnabled( hasImageViewer || hasImageViewer3D || hasPloter );
	actionTitle->setEnabled(mdiArea->subWindowList().size() > 0);
	actionViewMode->setEnabled(mdiArea->subWindowList().size() > 0);
	actionZoomIn->setEnabled(!actionFitToWindow->isChecked() && ( hasImageViewer || hasImageViewer3D ) && (activeImageViewer()->scaleFactor < 9.0));
	actionZoomOut->setEnabled(!actionFitToWindow->isChecked() && ( hasImageViewer || hasImageViewer3D ) && (activeImageViewer()->scaleFactor > 0.1));
	action3DVisualization->setEnabled( hasImageViewer3D );
	menuCorrelation->setEnabled(hasImageViewer || hasImageViewer3D);
	menuDTP->setEnabled(hasImageViewer || hasImageViewer3D);
	menuDTS->setEnabled(hasImageViewer || hasImageViewer3D);
	menuMorphology->setEnabled(hasPbmImageViewer || hasDbmImageViewer);
	menuMulti_Plan_View->setEnabled(hasGLWidget && activeGLWidget()->Viewtype()==GLWidget::MPV);
	menuSpatial->setEnabled(hasImageViewer);
	menuSkeleton->setEnabled(hasImageViewer);
	menu3DFilters->setEnabled(hasDbmImageViewer);
	pushButtonAddCurve->setEnabled( hasPloter && ( activePloter()->pathCurves.size() < 16 ) ); // permite plotar no máximo 15 curvas.
	pushButtonAverage->setEnabled( hasPloter && ( activePloter()->pathCurves.size() > 1 ) && (ext == "cor" or ext == "COR" or ext == "dtp" or ext == "DTP" or ext == "dts" or ext == "DTS") ); // Só ativa a média se tiver plotada mais de uma curva de correlação.
	pushButtonAccumulated->setEnabled( hasPloter && ( activePloter()->pathCurves.size() == 1 ) && (ext == "dtp" or ext == "DTP" or ext == "dts" or ext == "DTS" or ext == "dtg" or ext == "DTG") );
	pushButtonSource->setEnabled(hasPloter);
	radioButtonX->setEnabled(hasImageViewer3D);
	radioButtonY->setEnabled(hasImageViewer3D);
	radioButtonZ->setEnabled(hasImageViewer3D);
	spinBoxPlano3D->setEnabled(hasImageViewer3D);
	horizontalSliderPlano3D->setEnabled(hasImageViewer3D);
	actionOptions->setEnabled(hasHexEditor);
	actionPoresThroats->setEnabled(hasDbmImageViewer);
	actionCrop3D->setEnabled(hasImageViewer3D);
	if ( hasGLWidget ) {
		GLWidget * childImage = activeGLWidget();
		//if (childImage->tonsList.size()==3)
			//actionInverter->setEnabled(false);
		if(childImage->getViewType()==GLWidget::MPV){ //se o tipo de visualização for multiplanar
			spinBox_x->setEnabled(true);
			spinBox_y->setEnabled(true);
			spinBox_z->setEnabled(true);
			horizontalSlider_x->setEnabled(true);
			horizontalSlider_y->setEnabled(true);
			horizontalSlider_z->setEnabled(true);
			action3DVisualization->setEnabled(true);
			actionMPV->setEnabled(false);
			if (childImage->getPM3D() != nullptr) {
				horizontalSlider_x->setMaximum( childImage->getPM3D()->NX() - 1 );
				horizontalSlider_y->setMaximum( childImage->getPM3D()->NY() - 1 );
				horizontalSlider_z->setMaximum( childImage->getPM3D()->NZ() - 1 );
				spinBox_x->setMaximum( childImage->getPM3D()->NX() - 1 );
				spinBox_y->setMaximum( childImage->getPM3D()->NY() - 1 );
				spinBox_z->setMaximum( childImage->getPM3D()->NZ() - 1 );
			} else if (childImage->getPM3Di() != nullptr) {
				horizontalSlider_x->setMaximum( childImage->getPM3Di()->NX() - 1 );
				horizontalSlider_y->setMaximum( childImage->getPM3Di()->NY() - 1 );
				horizontalSlider_z->setMaximum( childImage->getPM3Di()->NZ() - 1 );
				spinBox_x->setMaximum( childImage->getPM3Di()->NX() - 1 );
				spinBox_y->setMaximum( childImage->getPM3Di()->NY() - 1 );
				spinBox_z->setMaximum( childImage->getPM3Di()->NZ() - 1 );
			}
			spinBox_x->setValue( childImage->getPlanX() );
			spinBox_y->setValue( childImage->getPlanY() );
			spinBox_z->setValue( childImage->getPlanZ() );
		}else if(childImage->getViewType()==GLWidget::VIEW3D){ //se o tipo de visualização for 3D
			action3DVisualization->setEnabled(false);
			actionMPV->setEnabled(true);
		}
	}else{
		spinBox_x->setEnabled(false);
		spinBox_y->setEnabled(false);
		spinBox_z->setEnabled(false);
		horizontalSlider_x->setEnabled(false);
		horizontalSlider_y->setEnabled(false);
		horizontalSlider_z->setEnabled(false);
		horizontalSlider_x->setMaximum( 99 );
		horizontalSlider_y->setMaximum( 99 );
		horizontalSlider_z->setMaximum( 99 );
		spinBox_x->setMaximum( 99 );
		spinBox_y->setMaximum( 99 );
		spinBox_z->setMaximum( 99 );
		spinBox_x->setValue( 0 );
		spinBox_y->setValue( 0 );
		spinBox_z->setValue( 0 );
	}
	if ( hasImageViewer3D ) {								//se a imagem ativa for uma imagem 3D
		BaseDnmImageViewer * childImage = active3DImageViewer( );		//pega a imagem 3D ativa
		switch (childImage->direcao) {
			case EIXO_X:
				radioButtonX->setChecked(true);
				spinBoxPlano3D->setMaximum( childImage->nx - 1 ); //seta o valor máximo do spinbox orizontal de acordo com a dimensão z da imagem.
				horizontalSliderPlano3D->setMaximum( childImage->nx - 1 );	//seta o valor máximo do slider orizontal de acordo com a dimensão z da imagem.
				break;
			case EIXO_Y:
				radioButtonY->setChecked(true);
				spinBoxPlano3D->setMaximum( childImage->ny - 1 ); //seta o valor máximo do spinbox orizontal de acordo com a dimensão z da imagem.
				horizontalSliderPlano3D->setMaximum( childImage->ny - 1 );	//seta o valor máximo do slider orizontal de acordo com a dimensão z da imagem.
				break;
			case EIXO_Z:
				radioButtonZ->setChecked(true);
				spinBoxPlano3D->setMaximum( childImage->nz - 1 ); //seta o valor máximo do spinbox orizontal de acordo com a dimensão z da imagem.
				horizontalSliderPlano3D->setMaximum( childImage->nz - 1 );	//seta o valor máximo do slider orizontal de acordo com a dimensão z da imagem.
				break;
			default :
				radioButtonZ->setChecked(true);
				spinBoxPlano3D->setMaximum( childImage->nz - 1 ); //seta o valor máximo do spinbox orizontal de acordo com a dimensão z da imagem.
				horizontalSliderPlano3D->setMaximum( childImage->nz - 1 );	//seta o valor máximo do slider orizontal de acordo com a dimensão z da imagem.
		}
		spinBoxPlano3D->setValue( childImage->curPlan );
	} else {												//se não houver imagem 3D ativa, zera os valores
		spinBoxPlano3D->setValue( 0 );
		spinBoxPlano3D->setMaximum( 99 );
		horizontalSliderPlano3D->setMaximum( 99 );
	}
	if ( hasPloter ){
		if (activePloter()->IsAccumulated()){
			pushButtonAccumulated->setText(QString::fromUtf8("Hide Accumulated"));
		} else {
			pushButtonAccumulated->setText(QString::fromUtf8("Show Accumulated"));
		}
	} else {
		pushButtonAccumulated->setText(QString::fromUtf8("Show Accumulated"));
	}
}

void Lvp::updateWindowMenu() {
	menuWindow->clear();
	menuWindow->addAction(actionClose);
	menuWindow->addAction(actionCloseAll);
	menuWindow->addSeparator();
	menuWindow->addAction(actionTitle);
	menuWindow->addAction(actionCascade);
	menuWindow->addSeparator();
	menuWindow->addAction(actionNext);
	menuWindow->addAction(actionPrevious);
	menuWindow->addAction(actionSeparator);
	
	QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
	actionSeparator->setSeparator(!windows.isEmpty());
	actionSeparator->setVisible(!windows.isEmpty());
	
	BaseImageViewer *childImage;
	GLWidget * childGL;
	QString text;
	QAction *action;
	for (int i = 0; i < windows.size(); ++i) {
		if (qobject_cast<PbmImageViewer *>(windows.at(i)->widget()) != 0) {
			childImage = qobject_cast<PbmImageViewer *>(windows.at(i)->widget());
			if (i < 9) {
				text = tr("&%1 %2").arg(i + 1).arg(childImage->getFileName());
			} else {
				text = tr("%1 %2").arg(i + 1).arg(childImage->getFileName());
			}
			action = menuWindow->addAction(text);
			action->setCheckable(true);
			action->setChecked(childImage == activeImageViewer());
			connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
			windowMapper->setMapping(action, windows.at(i));
		} else if (qobject_cast<PgmImageViewer *>(windows.at(i)->widget()) != 0) {
			childImage = qobject_cast<PgmImageViewer *>(windows.at(i)->widget());
			if (i < 9) {
				text = tr("&%1 %2").arg(i + 1).arg(childImage->getFileName());
			} else {
				text = tr("%1 %2").arg(i + 1).arg(childImage->getFileName());
			}
			action = menuWindow->addAction(text);
			action->setCheckable(true);
			action->setChecked(childImage == activeImageViewer());
			connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
			windowMapper->setMapping(action, windows.at(i));
		} else if (qobject_cast<DbmImageViewer *>(windows.at(i)->widget()) != 0) {
			childImage = qobject_cast<DbmImageViewer *>(windows.at(i)->widget());
			if (i < 9) {
				text = tr("&%1 %2").arg(i + 1).arg(childImage->getFileName());
			} else {
				text = tr("%1 %2").arg(i + 1).arg(childImage->getFileName());
			}
			action = menuWindow->addAction(text);
			action->setCheckable(true);
			action->setChecked(childImage == activeImageViewer());
			connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
			windowMapper->setMapping(action, windows.at(i));
		} else if (qobject_cast<DgmImageViewer *>(windows.at(i)->widget()) != 0) {
			childImage = qobject_cast<DgmImageViewer *>(windows.at(i)->widget());
			if (i < 9) {
				text = tr("&%1 %2").arg(i + 1).arg(childImage->getFileName());
			} else {
				text = tr("%1 %2").arg(i + 1).arg(childImage->getFileName());
			}
			action = menuWindow->addAction(text);
			action->setCheckable(true);
			action->setChecked(childImage == activeImageViewer());
			connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
			windowMapper->setMapping(action, windows.at(i));
		} else if (qobject_cast<GLWidget *>(windows.at(i)->widget()) != 0) {
			childGL = qobject_cast<GLWidget *>(windows.at(i)->widget());
			if (i < 9) {
				text = tr("&%1 3D Visualization [%2]").arg(i + 1).arg(QFileInfo(childGL->getFullFileName()).fileName());
			} else {
				text = tr( "%1 3D Visualization [%2]").arg(i + 1).arg(QFileInfo(childGL->getFullFileName()).fileName());
			}
			action = menuWindow->addAction(text);
			action->setCheckable(true);
			action->setChecked(childGL == activeGLWidget());
			connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
			windowMapper->setMapping(action, windows.at(i));
		}
	}
}

void Lvp::updateDockLista() {
	QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
	listWidget->clear();
	listWidget3D->clear();
	listWidgetChart->clear();
	listWidgetEditor->clear();
	bool hasImageViewer   = false;
	bool hasImageViewer3D = false;
	bool hasPloter		  = false;
	bool hasTextEditor	  = false;
	bool hasGlWidget    = false;
	for (int i = 0; i < windows.size(); ++i) {
		if ( PbmImageViewer *child = qobject_cast<PbmImageViewer *>(windows.at(i)->widget()) ) {
			QListWidgetItem *item = new QListWidgetItem(child->getFileName(), listWidget);
			child->item = item;
			item->setSelected(child == activePbmImageViewer());
			hasImageViewer = true;
		} else if ( PgmImageViewer *child = qobject_cast<PgmImageViewer *>(windows.at(i)->widget()) ) {
			QListWidgetItem *item = new QListWidgetItem(child->getFileName(), listWidget);
			child->item = item;
			item->setSelected(child == activePgmImageViewer());
			hasImageViewer = true;
		} else if ( DbmImageViewer *child = qobject_cast<DbmImageViewer *>(windows.at(i)->widget()) ) {
			QListWidgetItem *item = new QListWidgetItem(child->getFileName(), listWidget3D);
			child->item = item;
			item->setSelected(child == activeDbmImageViewer());
			hasImageViewer3D = true;
		} else if ( DgmImageViewer *child = qobject_cast<DgmImageViewer *>(windows.at(i)->widget()) ) {
			QListWidgetItem *item = new QListWidgetItem(child->getFileName(), listWidget3D);
			child->item = item;
			item->setSelected(child == activeDgmImageViewer());
			hasImageViewer3D = true;
		} else if (Ploter * child = qobject_cast<Ploter *>(windows.at(i)->widget())) {
			QListWidgetItem *item = new QListWidgetItem(child->getFileName(), listWidgetChart);
			child->item = item;
			item->setSelected(child == activePloter());
			hasPloter = true;
		} else if (TextEditor * child = qobject_cast<TextEditor *>(windows.at(i)->widget())) {
			QListWidgetItem *item = new QListWidgetItem(child->getFileName(), listWidgetEditor);
			child->item = item;
			item->setSelected(child == activeTextEditor());
			hasTextEditor = true;
		} else if (GLWidget * child = qobject_cast<GLWidget *>(windows.at(i)->widget())) {
			if (child->getViewType()==GLWidget::MPV)
				hasGlWidget = true;
		}
	}
	dockWidgetLista->setVisible(hasImageViewer);
	dockWidgetLista3D->setVisible(hasImageViewer3D);
	dockWidgetListaChart->setVisible(hasPloter);
	dockWidgetListaTextEditor->setVisible(hasTextEditor);
	dockWidgetVisualisation3D->setVisible(hasGlWidget);
}

void Lvp::open( ) {
	// lista com o nome dos arquivos que serão abertos
	QStringList files = QFileDialog::getOpenFileNames(this, tr("Open File(s)"), lastOpenPath, tr("Accepted Files (*.pgm *.pbm *.dbm *.dgm *.cor *.dtp *.dts *.dtg *.rpc *.txt *.rsl);;Image Files (*.pgm *.pbm *.dbm);;Binary Images (*.pbm);;3D Binary Images (*.dbm);;Grey Scale Images (*.pgm);;3DGrey Scale Images (*.dgm);;Relative Permeability Curves (*.rpc);;Correlation Files (*.cor);;Distribution Files (*.dtp *.dts *.dtg);;Network of Sites and Links (*.rsl);;Text Files (*.txt)"));
	int numFiles = files.size(); //número de arquivos que serão abertos
	
	QProgressDialog progress("Opening files...", "&Cancel", 0, numFiles, this);
	progress.setWindowModality(Qt::WindowModal);
	
	for (int i = 0; i < numFiles; ++i) {
		progress.setValue(i);
		if (progress.wasCanceled())
			break;
		//cerr << "Abrindo " << files.at(i).toStdString() << endl;
		open( files.at(i).toStdString(), false);
	}
	
	progress.setValue(numFiles);
}
void Lvp::open(string _file, bool novo) {
	QApplication::setOverrideCursor(Qt::WaitCursor);
	QString fileName;  // nome do arquivo atual
	fileName = fileName.fromStdString(_file);
	QMdiSubWindow *existing = nullptr;
	if ( ! fileName.isEmpty() && QFile::exists ( fileName ) ) {
		fileWatcher->addPath(fileName);
		QDir lop(fileName);
		lastOpenPath = lop.canonicalPath();
		QString  qext = fileName.mid(fileName.lastIndexOf(".")+1); // pega todos os caracteres a partir do último ponto
		string ext = qext.toLower().toStdString(); 		// converte para std string
		if ( ext == "cor" or ext == "rpc" or ext == "dtp" or ext == "dts" or ext == "dtg") { 	// o arquivo será plotado
			Ploter *childPloter = nullptr;
			existing = findPloter(fileName);
			if (existing) {
				mdiArea->setActiveSubWindow(existing);
				QApplication::restoreOverrideCursor();
				return;
			}
			childPloter = createPloter();
			if ( childPloter ) {
				if ( novo ) {  // é um novo arquivo.
					childPloter->isNew = true;
				}
				if ( childPloter->loadFile(fileName) ) {
					childPloter->show();
					statusBar()->showMessage(tr("File %1 loaded!").arg(fileName), 2000);
				} else {
					statusBar()->showMessage(tr("Error"), 2000);
					//childPloter->close();
				}
			} else {
				statusBar()->showMessage(tr("Error"), 2000);
			}
		} else if (ext == "raw") {
			import(fileName);
		} else if (ext == "txt" || ext == "rsl") {
			TextEditor *childText= nullptr;
			existing = findTextEditor(fileName);
			if ( existing ) {
				mdiArea->setActiveSubWindow(existing);
				QApplication::restoreOverrideCursor();
				return;
			}
			childText = createTextEditor();
			if ( novo ) {  // é um novo arquivo.
				childText->isNew = true;
			}
			if ( childText->loadFile(fileName) ) {
				actionPrint->setEnabled(true);
				actionFitToWindow->setEnabled(false);
				childText->show();
				statusBar()->showMessage(tr("File %1 loaded!").arg(fileName), 2000);
			} else {
				statusBar()->showMessage(tr("Error"), 2000);
				childText->close();
			}
		} else {
			BaseImageViewer *child = nullptr;
			existing = findImageViewer(fileName);
			if ( existing ) {
				mdiArea->setActiveSubWindow(existing);
				QApplication::restoreOverrideCursor();
				return;
			}
			if ( ext == "dbm" ) {
				child = createDbmImageViewer();
			} else if ( ext == "dgm" ) {
				child = createDgmImageViewer();
			} else if ( ext == "pbm" ) {
				child = createPbmImageViewer();
			} else if ( ext == "pgm" ) {
				child = createPgmImageViewer();
			} else {
				QApplication::restoreOverrideCursor();
				return;
			}
			if ( novo ) {  // é um novo arquivo.
				child->isNew = true;
			}
			if ( child->loadFile(fileName) ) {
				actionPrint->setEnabled(true);
				actionFitToWindow->setEnabled(true);
				if (!actionFitToWindow->isChecked()) {
					child->isFitedToWindow = false;
					child->imageLabel->adjustSize();
				}
				child->show();
				statusBar()->showMessage(tr("File %1 loaded!").arg(fileName), 2000);
			} else {
				statusBar()->showMessage(tr("Error"), 2000);
				child->close();
			}
		}
	} else {
		statusBar()->showMessage(tr("File name error! - %1").arg(fileName), 3000);
	}
	//updateMenus(); //já esta chamando atraves do connect com subWindowActivated(QMdiSubWindow *)
	//updateDockLista();
	QApplication::restoreOverrideCursor();
}

void Lvp::open3DVisualization( ) {
	if (DbmImageViewer *mdiChild = activeDbmImageViewer()) {
		QMdiSubWindow *existing = findGLWidget(mdiChild->getFullFileName());
		if (existing) {
			GLWidget * child = qobject_cast<GLWidget *>(existing->widget());
			child->setViewType(GLWidget::VIEW3D);
			mdiArea->setActiveSubWindow(existing);
		} else {
			GLWidget *child = createGLWidget(mdiChild);
			child->setViewType(GLWidget::VIEW3D);
			child->show();
			statusBar()->showMessage(tr("File loaded"), 2000);
		}
	} else if (DgmImageViewer *mdiChild = activeDgmImageViewer()) {
		QMdiSubWindow *existing = findGLWidget(mdiChild->getFullFileName());
		if (existing) {
			GLWidget * child = qobject_cast<GLWidget *>(existing->widget());
			child->setViewType(GLWidget::VIEW3D);
			mdiArea->setActiveSubWindow(existing);
		} else {
			GLWidget *child = createGLWidget(mdiChild);
			child->setViewType(GLWidget::VIEW3D);
			child->show();
			statusBar()->showMessage(tr("File loaded"), 2000);
		}
	} else if (GLWidget *mdiChild = activeGLWidget()){
		mdiChild->setViewType(GLWidget::VIEW3D);
	}
	updateMenus();
	updateDockLista();
}

void Lvp::openMPV( ) {
	if (DbmImageViewer *mdiChild = activeDbmImageViewer()) {
		QMdiSubWindow *existing = findGLWidget(mdiChild->getFullFileName());
		if (existing) {
			GLWidget * child = qobject_cast<GLWidget *>(existing->widget());
			child->setViewType(GLWidget::MPV);
			mdiArea->setActiveSubWindow(existing);
		} else {
			GLWidget *child = createGLWidget(mdiChild);
			child->setViewType(GLWidget::MPV);
			child->show();
			statusBar()->showMessage(tr("File loaded"), 2000);
		}
	} else if (DgmImageViewer *mdiChild = activeDgmImageViewer()) {
		QMdiSubWindow *existing = findGLWidget(mdiChild->getFullFileName());
		if (existing) {
			GLWidget * child = qobject_cast<GLWidget *>(existing->widget());
			child->setViewType(GLWidget::MPV);
			mdiArea->setActiveSubWindow(existing);
		} else {
			GLWidget *child = createGLWidget(mdiChild);
			child->setViewType(GLWidget::MPV);
			child->show();
			statusBar()->showMessage(tr("File loaded"), 2000);
		}
	} else if (GLWidget *mdiChild = activeGLWidget()){
		mdiChild->setViewType(GLWidget::MPV);
	}
	updateMenus();
	updateDockLista();
}

void Lvp::openPNV( ) {
	if (TextEditor *mdiChild = activeTextEditor()) {
		QMdiSubWindow *existing = findGLWidget(mdiChild->getFullFileName());
		if (existing) {
			GLWidget * child = qobject_cast<GLWidget *>(existing->widget());
			child->setViewType(GLWidget::RPSL);
			mdiArea->setActiveSubWindow(existing);
		} else {
			GLWidget *child = createGLWidget(mdiChild);
			child->setViewType(GLWidget::RPSL);
			child->show();
			statusBar()->showMessage(tr("File loaded"), 2000);
		}
	}
	updateMenus();
	updateDockLista();
}

//void Lvp::openMPV( ) {
//	if (DbmImageViewer *mdiChild = activeDbmImageViewer()) {
//		QMdiSubWindow *existing = findGLWidget(mdiChild->getFullFileName());
//		if (existing) {
//			GLWidget * child = qobject_cast<GLWidget *>(existing->widget());
//			child->setViewType(GLWidget::MPV);
//			mdiArea->setActiveSubWindow(existing);
//		} else {
//			GLWidget *child = createGLWidget(mdiChild);
//			child->setViewType(GLWidget::MPV);
//			child->show();
//			statusBar()->showMessage(tr("File loaded"), 2000);
//		}
//	} else if (DgmImageViewer *mdiChild = activeDgmImageViewer()) {
//		QMdiSubWindow *existing = findGLWidget(mdiChild->getFullFileName());
//		if (existing) {
//			GLWidget * child = qobject_cast<GLWidget *>(existing->widget());
//			child->setViewType(GLWidget::MPV);
//			mdiArea->setActiveSubWindow(existing);
//		} else {
//			GLWidget *child = createGLWidget(mdiChild);
//			child->setViewType(GLWidget::MPV);
//			child->show();
//			statusBar()->showMessage(tr("File loaded"), 2000);
//		}
//	} else if (GLWidget *mdiChild = activeGLWidget()){
//		mdiChild->setViewType(GLWidget::MPV);
//	}
//	updateMenus();
//	updateDockLista();
//}

void Lvp::openEditor(){
	QMdiSubWindow *existing = 0;
	QString fileName;
	if( PbmImageViewer * childImage = activePbmImageViewer() ) {
		fileName = childImage->getFullFileName();
		if(childImage->pm == nullptr){
			childImage->pm = new TCMatriz2D<bool>(fileName.toStdString());
		}
		if(childImage->pm) {
			if(childImage->pm->GetFormato() == P1_X_Y_ASCII){
				existing = findTextEditor( fileName );
				goto openTextEditor;
			} else if(childImage->pm->GetFormato() == P4_X_Y_BINARY){
				existing = findHexEditor( fileName );
				goto openHexEditor;
			}
		} else {
			return;
		}
	} else if( PgmImageViewer * childImage = activePgmImageViewer() ) {
		fileName = childImage->getFullFileName();
		if(childImage->pm == nullptr){
			childImage->pm = new TCMatriz2D<int>(fileName.toStdString());
		}
		if(childImage->pm) {
			if(childImage->pm->GetFormato() == P2_X_Y_GRAY_ASCII){
				existing = findTextEditor( fileName );
				goto openTextEditor;
			} else if(childImage->pm->GetFormato() == P5_X_Y_GRAY_BINARY){
				existing = findHexEditor( fileName );
				goto openHexEditor;
			}
		} else {
			return;
		}
	} else if( DbmImageViewer * childImage = activeDbmImageViewer() ) {
		fileName = childImage->getFullFileName();
		if(childImage->pm3D == nullptr){
			childImage->pm3D = new TCImagem3D<bool>(fileName.toStdString());
		}
		if(childImage->pm3D) {
			if(childImage->pm3D->GetFormato() == D1_X_Y_Z_ASCII){
				existing = findTextEditor( fileName );
				goto openTextEditor;
			} else if(childImage->pm3D->GetFormato() == D4_X_Y_Z_BINARY){
				existing = findHexEditor( fileName );
				goto openHexEditor;
			}
		} else {
			return;
		}
	} else if( DgmImageViewer * childImage = activeDgmImageViewer() ) {
		fileName = childImage->getFullFileName();
		if(childImage->pm3D == nullptr){
			childImage->pm3D = new TCImagem3D<int>(fileName.toStdString());
		}
		if(childImage->pm3D) {
			if(childImage->pm3D->GetFormato() == D2_X_Y_Z_GRAY_ASCII){
				existing = findTextEditor( fileName );
				goto openTextEditor;
			} else if(childImage->pm3D->GetFormato() == D5_X_Y_Z_GRAY_BINARY){
				existing = findHexEditor( fileName );
				goto openHexEditor;
			}
		} else {
			return;
		}
	} else 	if (Ploter *mdiChild = activePloter()) {
		fileName = mdiChild->getFullFileName();
		existing = findTextEditor( fileName );
		goto openTextEditor;
	} else {
		return;
	}
	
openTextEditor: {
		if (existing) {
			mdiArea->setActiveSubWindow(existing);
		} else {
			if ( TextEditor *child = createTextEditor() ){
				if ( child->loadFile( fileName ) ){
					child->show();
					statusBar()->showMessage(tr("File loaded"), 2000);
				}
			}
		}
		//updateMenus();
		return;
	}
openHexEditor: {
		if (existing) {
			mdiArea->setActiveSubWindow(existing);
		} else {
			if ( HexEditor *child = createHexEditor() ){
				if ( child->loadFile(fileName) ){
					child->show();
					statusBar()->showMessage(tr("File loaded"), 2000);
				}
			}
		}
		//updateMenus();
		return;
	}
}

PbmImageViewer *Lvp::createPbmImageViewer() {
	PbmImageViewer *childim = nullptr;
	childim = new PbmImageViewer( this );
	if ( childim ) {
		mdiArea->addSubWindow(childim);
		return childim;
	}
	return nullptr;
}

PgmImageViewer *Lvp::createPgmImageViewer() {
	PgmImageViewer *childim = nullptr;
	childim = new PgmImageViewer( this );
	if ( childim ) {
		mdiArea->addSubWindow(childim);
		return childim;
	}
	return nullptr;
}

DbmImageViewer *Lvp::createDbmImageViewer() {
	DbmImageViewer *childim = nullptr;
	childim = new DbmImageViewer( this );
	if ( childim ) {
		mdiArea->addSubWindow(childim);
		connect(childim, SIGNAL(dataChanged()), this, SLOT(updateActionSave()));
		return childim;
	}
	return nullptr;
}

DgmImageViewer * Lvp::createDgmImageViewer() {
	DgmImageViewer *childim = nullptr;
	childim = new DgmImageViewer( this );
	if ( childim ) {
		mdiArea->addSubWindow(childim);
		connect(childim, SIGNAL(dataChanged()), this, SLOT(updateActionSave()));
		return childim;
	}
	return nullptr;
}

GLWidget * Lvp::createGLWidget(DbmImageViewer *_mdiChild) {
	GLWidget *childim = nullptr;
	childim = new GLWidget(_mdiChild->pm3D, _mdiChild->getFullFileName(), GLWidget::MPV, this);
	if ( childim ) {
		mdiArea->addSubWindow(childim);
		return childim;
	}
	return nullptr;
}

GLWidget * Lvp::createGLWidget(DgmImageViewer *_mdiChild) {
	GLWidget *childim = nullptr;
	childim = new GLWidget(_mdiChild->pm3D, _mdiChild->getFullFileName(), GLWidget::MPV, this);
	if ( childim ) {
		mdiArea->addSubWindow(childim);
		return childim;
	}
	return nullptr;
}

GLWidget * Lvp::createGLWidget(TextEditor *_mdiChild) {
	GLWidget *childim = nullptr;
	childim = new GLWidget(_mdiChild->getFullFileName(), GLWidget::MPV, this);
	if ( childim ) {
		mdiArea->addSubWindow(childim);
		return childim;
	}
	return nullptr;
}

Ploter * Lvp::createPloter() {
	Ploter *childPloter = nullptr;
	childPloter = new Ploter( this );
	if ( childPloter ) {
		mdiArea->addSubWindow( childPloter );
		return childPloter;
	}
	return nullptr;
}

TextEditor * Lvp::createTextEditor() {
	TextEditor *childEditor = nullptr;
	childEditor = new TextEditor( this );
	if ( childEditor ) {
		mdiArea->addSubWindow( childEditor );
		connect( childEditor->document(), SIGNAL(contentsChanged()), this, SLOT(updateActionSave()) );
		return childEditor;
	}
	return nullptr;
}

HexEditor *Lvp::createHexEditor() {
	HexEditor *childEditor = nullptr;
	childEditor = new HexEditor( this );
	if ( childEditor ) {
		mdiArea->addSubWindow( childEditor );
		connect(childEditor, SIGNAL(dataChanged()), this, SLOT(updateActionSave()));
		childEditor->createStatusBar();
		readSettings();
		return childEditor;
	}
	return nullptr;
}

void Lvp::updateActionSave(){
	if (activeTextEditor() != 0){
		actionSave->setEnabled(activeTextEditor()->document()->isModified());
	} else if (activeHexEditor() != 0){
		actionSave->setEnabled(activeHexEditor()->IsModified());
	} else if (active3DImageViewer() != 0){
		actionSave->setEnabled(active3DImageViewer()->isModified);
	}
}

void Lvp::closeEvent(QCloseEvent *event) {
	closeAllSubWindows();
	if (activeImageViewer() or activeGLWidget() or activePloter() or activeTextEditor()) {
		event->ignore();
	} else {
		writeSettings();
		event->accept();
	}
}

void Lvp::save() {
	if ( activeTextEditor() != 0 ) {
		activeTextEditor()->save();
	} else 	if ( activeHexEditor() != 0 ) {
		activeHexEditor()->save();
	} else 	if ( active3DImageViewer() != 0 ) {
		active3DImageViewer()->save();
	}
	updateActionSave();
}

void Lvp::saveAs() {
	if ( activeImageViewer() != 0 ) {
		activeImageViewer()->saveAs();
	} else if ( activePloter() != 0 ) {
		//cerr << "lastOpenPath antes: " << lastOpenPath.toStdString() << endl;
		activePloter()->saveAs( & lastOpenPath);
		//cerr << "lastOpenPath depois: " << lastOpenPath.toStdString() << endl;
	} else if ( activeTextEditor() != 0 ) {
		activeTextEditor()->saveAs();
	} else 	if ( activeHexEditor() != 0 ) {
		activeHexEditor()->saveAs();
	}
	updateDockLista();
}

void Lvp::copy() { }

void Lvp::properties() {
	BaseDnmImageViewer * mdiChild = nullptr;
	mdiChild = active3DImageViewer();
	if ( mdiChild ) {
		QApplication::setOverrideCursor(Qt::WaitCursor);
		if ( dialogProperties ) {
			dialogProperties->show();
		} else {
			dialogProperties = new Properties( this );
			if ( dialogProperties ) {
				dialogProperties->show();
			} else {
				QApplication::restoreOverrideCursor();
				QMessageBox::information(this, tr("LVP"), tr("Erro! - To create properties dialog!"));
				return;
			}
		}
		TCImagem3D<bool> * i3DB = mdiChild->getPI3DBool();
		if (i3DB) {
			dialogProperties->doubleSpinBoxSizePixel->setValue(i3DB->DimensaoPixel());
			dialogProperties->spinBoxAmplificationFactor->setValue(i3DB->FatorAmplificacao());
			dialogProperties->spinBoxNumPixelsEdge->setValue(i3DB->NumeroPixelsBorda());
		} else {
			TCImagem3D<int> * i3DI = mdiChild->getPI3DInt();
			if (i3DI) {
				dialogProperties->doubleSpinBoxSizePixel->setValue(i3DI->DimensaoPixel());
				dialogProperties->spinBoxAmplificationFactor->setValue(i3DI->FatorAmplificacao());
				dialogProperties->spinBoxNumPixelsEdge->setValue(i3DI->NumeroPixelsBorda());
			}
		}
	} else {
		QMessageBox::information(this, tr("LVP"), tr("Erro! - To get active 3D image!"));
	}
	QApplication::restoreOverrideCursor();
}

void Lvp::exProperties() {
	QApplication::setOverrideCursor(Qt::WaitCursor);
	BaseDnmImageViewer * mdiChild = nullptr;
	mdiChild = active3DImageViewer();
	if ( mdiChild ) {
		TCImagem3D<bool> * i3DB = mdiChild->getPI3DBool();
		if (i3DB) {
			if ( i3DB->DimensaoPixel() != dialogProperties->doubleSpinBoxSizePixel->value() ||
					 i3DB->FatorAmplificacao() != (unsigned int) dialogProperties->spinBoxAmplificationFactor->value() ||
					 i3DB->NumeroPixelsBorda() != (unsigned int) dialogProperties->spinBoxNumPixelsEdge->value() )
			{ mdiChild->documentWasModified(); }
			i3DB->DimensaoPixel( dialogProperties->doubleSpinBoxSizePixel->value() );
			i3DB->FatorAmplificacao( dialogProperties->spinBoxAmplificationFactor->value() );
			i3DB->NumeroPixelsBorda( dialogProperties->spinBoxNumPixelsEdge->value() );
		} else {
			TCImagem3D<int> * i3DI = mdiChild->getPI3DInt();
			if (i3DI) {
				if ( i3DI->DimensaoPixel() != dialogProperties->doubleSpinBoxSizePixel->value() ||
						 i3DI->FatorAmplificacao() != (unsigned int) dialogProperties->spinBoxAmplificationFactor->value() ||
						 i3DI->NumeroPixelsBorda() != (unsigned int) dialogProperties->spinBoxNumPixelsEdge->value() )
				{ mdiChild->documentWasModified(); }

				i3DI->DimensaoPixel( dialogProperties->doubleSpinBoxSizePixel->value() );
				i3DI->FatorAmplificacao( dialogProperties->spinBoxAmplificationFactor->value() );
				i3DI->NumeroPixelsBorda( dialogProperties->spinBoxNumPixelsEdge->value() );
			}
		}

	} else {
		QMessageBox::information(this, tr("LVP"), tr("Erro! - To get active 3D image!"));
	}
	delete dialogProperties;
	dialogProperties = nullptr;
	QApplication::restoreOverrideCursor();
}

void Lvp::fitToWindow() {
	bool in = false;
	BaseImageViewer *mdiChild;
	foreach (mdiChild, selectedAllImagesList()) {
		if (actionFitToWindow->isChecked()) {
			if (!mdiChild->isFitedToWindow) {
				mdiChild->setWidgetResizable(true);
				mdiChild->isFitedToWindow = true;
			}
		} else {
			if (mdiChild->isFitedToWindow) {
				mdiChild->setWidgetResizable(false);
				mdiChild->isFitedToWindow = false;
				mdiChild->normalSize();
			}
		}
		in = true;
	}
	if (!in && (mdiChild = activeImageViewer())) {
		if (actionFitToWindow->isChecked()) {
			if (!mdiChild->isFitedToWindow) {
				mdiChild->setWidgetResizable(true);
				mdiChild->isFitedToWindow = true;
			}
		} else {
			if (mdiChild->isFitedToWindow) {
				mdiChild->setWidgetResizable(false);
				mdiChild->isFitedToWindow = false;
				mdiChild->normalSize();
			}
		}
	}
	updateMenus();
}

void Lvp::changeViewMode() {
	if (mdiArea->viewMode() == QMdiArea::TabbedView) {
		mdiArea->setViewMode( QMdiArea::SubWindowView );
		settings->setValue("viewMode", QMdiArea::SubWindowView);
	} else {
		mdiArea->setViewMode( QMdiArea::TabbedView );
		settings->setValue("viewMode", QMdiArea::TabbedView);
	}
}

void Lvp::print() {
	activeImageViewer()->print();
}

void Lvp::zoomIn() {
	bool in = false;
	BaseImageViewer *mdiChild;
	foreach (mdiChild, selectedAllImagesList()) {
		if (mdiChild->scaleFactor < 9.0) {
			mdiChild->zoomIn();
		}
		in = true;
	}
	if (!in && (mdiChild = activeImageViewer())) {
		if (mdiChild->scaleFactor < 9.0) {
			mdiChild->zoomIn();
		}
	}
	updateMenus();
}

void Lvp::zoomOut() {
	bool in = false;
	BaseImageViewer *mdiChild;
	foreach (mdiChild, selectedAllImagesList()) {
		if (mdiChild->scaleFactor > 0.1) {
			mdiChild->zoomOut();
		}
		in = true;
	}
	if (!in && (mdiChild = activeImageViewer())) {
		if (mdiChild->scaleFactor > 0.1) {
			mdiChild->zoomOut();
		}
	}
	updateMenus();
	//     activeImageViewer()->zoomOut();
	//     actionZoomIn->setEnabled(activeImageViewer()->scaleFactor < 3.0);
	//     actionZoomOut->setEnabled(activeImageViewer()->scaleFactor > 0.333);
}

void Lvp::normalSize() {
	bool in = false;
	BaseImageViewer *mdiChild;
	foreach (mdiChild, selectedAllImagesList()) {
		mdiChild->normalSize();
		actionZoomIn->setEnabled(true);
		actionZoomOut->setEnabled(true);
		in = true;
	}
	if (!in && (mdiChild = activeImageViewer())) {
		mdiChild->normalSize();
		actionZoomIn->setEnabled(true);
		actionZoomOut->setEnabled(true);
	}
}

void Lvp::inversion() {
	QApplication::setOverrideCursor(Qt::WaitCursor);
	TCMatriz2D<bool> * pmInv = nullptr;
	int numFiles = selectedPbmImagesList().size();
	int cont = 0;
	QProgressDialog progress("Inverting images...", "&Cancel", 0, numFiles, this);
	progress.setWindowModality(Qt::WindowModal);
	foreach (PbmImageViewer *mdiChild, selectedPbmImagesList()) {
		progress.setValue(cont);
		if (progress.wasCanceled())
			break;
		QString qstr = mdiChild->getFullFileName();
		cerr << "Nome do arquivo em Lvp::inversion() " << qstr.toStdString() << endl;
		if ( mdiChild->pm == nullptr )
			pmInv = new TCMatriz2D<bool>( qstr.toStdString() );
		else
			pmInv = new TCMatriz2D<bool>( *mdiChild->pm );
		if( pmInv ) {
			pmInv->Inverter();
			static int seqNumberInversion = 1;
			qstr = tr(".inversion%1.%2").arg(QString::number(seqNumberInversion++)).arg(mdiChild->getFileExt());
			pmInv->Write( qstr.toStdString() );
			cerr << "Tentando abrir: " << (mdiChild->getFilePath() + qstr).toStdString() << endl;
			open( (mdiChild->getFilePath() + qstr).toStdString() );
			cont++;
			pmInv = nullptr;
		} else {
			QMessageBox::information(this, tr("LVP"), tr("Error trying invert image!"));
		}
	}
	progress.setValue(numFiles);
	QApplication::restoreOverrideCursor();
}

void Lvp::inversion3D() {
	QApplication::setOverrideCursor(Qt::WaitCursor);
	TCImagem3D<bool> * pmInv = nullptr;
	int numFiles = selectedDbmImagesList().size();
	int cont = 0;
	QProgressDialog progress("Inverting images...", "&Cancel", 0, numFiles, this);
	progress.setWindowModality(Qt::WindowModal);
	foreach (DbmImageViewer *mdiChild, selectedDbmImagesList()) {
		progress.setValue(cont);
		if (progress.wasCanceled())
			break;
		QString qstr = mdiChild->getFullFileName();
		if ( mdiChild->pm3D == nullptr )
			pmInv = new TCImagem3D<bool>( qstr.toStdString() );
		else
			pmInv = new TCImagem3D<bool>( *mdiChild->pm3D );
		if( pmInv ) {
			pmInv->Inverter();
			static int seqNumberInversion3D = 1;
			qstr = tr(".inversion3D%1.%2").arg(QString::number(seqNumberInversion3D++)).arg(mdiChild->getFileExt());
			pmInv->Write( qstr.toStdString() );
			open( (mdiChild->getFilePath() + qstr).toStdString() );
			cont++;
		} else {
			QMessageBox::information(this, tr("LVP"), tr("Error trying invert 3D image!"));
		}
	}
	progress.setValue(numFiles);
	QApplication::restoreOverrideCursor();
}

void Lvp::lowPass() {
	QList<PbmImageViewer *> pbmImagesList = selectedPbmImagesList(); //lista de ponteiros para imagens pbm selecionadas.
	QList<PgmImageViewer *> pgmImagesList = selectedPgmImagesList(); //lista de ponteiros para imagens pgm selecionadas.
	bool ok;
	int tamMask = 5;
	do {
		if ( (tamMask % 2) == 0 ) {
			QMessageBox::information(this, tr("LVP"), tr("The mask size must be an odd number"));
		}
		tamMask = QInputDialog::getInt(this, tr(":. Low Pass"), tr("Enter the Mask size:"), 5, 1, 99, 2, &ok);
	} while ( (tamMask % 2) == 0 );
	if (ok) {
		int numFiles = pbmImagesList.size(); //número de arquivos que serão abertos
		numFiles += pgmImagesList.size(); //acumula número de arquivos que serão abertos
		int cont = 0;
		QProgressDialog progress("Applying filter...", "&Cancel", 0, numFiles, this);
		progress.setWindowModality(Qt::WindowModal);
		foreach (PbmImageViewer *mdiChild, pbmImagesList) {
			progress.setValue(cont);
			if (progress.wasCanceled())
				break;
			QString qstr = mdiChild->getFullFileName();
			TCMatriz2D<bool> * pm = new TCMatriz2D<bool>(qstr.toStdString());
			TCFiltro<bool> * filtro = new TCFEPassaBaixa<bool>(pm,tamMask);
			filtro->Go(pm,tamMask);
			qstr = tr(".lp-%1").arg( mdiChild->getFileName() );
			qstr = validateFileName( mdiChild->getFilePath() + qstr );
			pm->Path("");
			pm->Write(qstr.toStdString());
			open( qstr.toStdString() );
			delete pm;
			delete filtro;
			cont++;
		}
		foreach (PgmImageViewer *mdiChild, pgmImagesList) {
			progress.setValue(cont);
			if (progress.wasCanceled())
				break;
			QString qstr = mdiChild->getFullFileName();
			TCMatriz2D<int> * pm = new TCMatriz2D<int>(qstr.toStdString());
			TCFiltro<int> * filtro = new TCFEPassaBaixa<int>(pm,tamMask);
			filtro->Go(pm,tamMask);
			qstr = tr(".lp-%1").arg( mdiChild->getFileName() );
			qstr = validateFileName( mdiChild->getFilePath() + qstr );
			pm->Path("");
			pm->Write(qstr.toStdString());
			open( qstr.toStdString() );
			delete pm;
			delete filtro;
			cont++;
		}
		progress.setValue(numFiles);
	}
}

void Lvp::highPass() {
	QList<PbmImageViewer *> pbmImagesList = selectedPbmImagesList(); //lista de ponteiros para imagens pbm selecionadas.
	QList<PgmImageViewer *> pgmImagesList = selectedPgmImagesList(); //lista de ponteiros para imagens pgm selecionadas.
	bool ok;
	int tamMask = 5;
	do {
		if ( (tamMask % 2) == 0 ) {
			QMessageBox::information(this, tr("LVP"), tr("The mask size must be an odd number"));
		}
		tamMask = QInputDialog::getInt(this, tr(":. High Pass"), tr("Enter the Mask size:"), 5, 1, 99, 2, &ok);
	} while ( (tamMask % 2) == 0 );
	if (ok) {
		int numFiles = pbmImagesList.size(); //número de arquivos que serão abertos
		numFiles += pgmImagesList.size(); //acumula número de arquivos que serão abertos
		int cont = 0;
		QProgressDialog progress("Applying filter...", "&Cancel", 0, numFiles, this);
		progress.setWindowModality(Qt::WindowModal);
		foreach (PbmImageViewer *mdiChild, pbmImagesList) {
			progress.setValue(cont);
			if (progress.wasCanceled())
				break;
			QString qstr = mdiChild->getFullFileName();
			TCMatriz2D<bool> * pm = new TCMatriz2D<bool>(qstr.toStdString());
			TCFiltro<bool> * filtro = new TCFEPassaAlta<bool>(pm,tamMask);
			filtro->Go(pm,tamMask);
			qstr = tr(".hp-%1").arg( mdiChild->getFileName() );
			qstr = validateFileName( mdiChild->getFilePath() + qstr );
			pm->Path("");
			pm->Write(qstr.toStdString());
			open( qstr.toStdString() );
			delete pm;
			delete filtro;
			cont++;
		}
		foreach (PgmImageViewer *mdiChild, pgmImagesList) {
			progress.setValue(cont);
			if (progress.wasCanceled())
				break;
			QString qstr = mdiChild->getFullFileName();
			TCMatriz2D<int> * pm = new TCMatriz2D<int>(qstr.toStdString());
			TCFiltro<int> * filtro = new TCFEPassaAlta<int>(pm,tamMask);
			filtro->Go(pm,tamMask);
			qstr = tr(".hp-%1").arg( mdiChild->getFileName() );
			qstr = validateFileName( mdiChild->getFilePath() + qstr );
			pm->Path("");
			pm->Write(qstr.toStdString());
			open( qstr.toStdString() );
			delete pm;
			delete filtro;
			cont++;
		}
		progress.setValue(numFiles);
	}
}

void Lvp::erosion() {
	mathematicalMorphology( mtErosion );
}

void Lvp::dilation() {
	mathematicalMorphology( mtDilation );
}

void Lvp::opening() {
	mathematicalMorphology( mtOpening );
}

void Lvp::closing() {
	mathematicalMorphology( mtClosing );
}

void Lvp::mathematicalMorphology( MorphType mtype ) {
	TCMatriz2D<bool> * pm = nullptr;
	TCFEMorfologiaMatematica<bool> * filtro = nullptr;
	QString qstr;
	//CBCDiscreta *maskd = dynamic_cast < CBCDiscreta * > ( mask );
	QList<PbmImageViewer *> imagesList = selectedPbmImagesList(); //lista de ponteiros para imagens selecionadas.
	bool ok;
	int tamStruElem = QInputDialog::getInt(this, tr(":. LVP"), tr("Enter the Structuring Element size:"), 1, 0, 99, 1, &ok);
	if (ok) {
		QMessageBox msgBox(this);
		msgBox.setWindowTitle(tr("LVP - Mathematical Morphology"));
		msgBox.setText(tr("Pore is:"));
		QPushButton *cancelButton = msgBox.addButton(QMessageBox::Cancel);
		msgBox.addButton(tr("&Write (0)"), QMessageBox::ActionRole);
		QPushButton *blackButton = msgBox.addButton(tr("&Black (1)"), QMessageBox::ActionRole);
		msgBox.setDefaultButton(blackButton);
		msgBox.exec();
		if ( msgBox.clickedButton() != cancelButton ) {
			QStringList tipos; // cria lista de items
			tipos << tr("Spatial") << tr("IDF-d34") << tr("IDF-d5711") << tr("Euclidean"); //seta os items
			QString tipo = QInputDialog::getItem(this, tr(":. LVP"), tr("Kind of metric:"), tipos, 0, false, &ok); //apresenta dialogo para escolha do item
			if (ok && !tipo.isEmpty()){
				QApplication::setOverrideCursor(Qt::WaitCursor);
				int indice, fundo;
				if (msgBox.clickedButton() == blackButton) {
					indice = 1; fundo = 0;
				} else {
					indice = 0; fundo = 1;
				}
				int numFiles = imagesList.size(); //número de arquivos que serão abertos
				int cont = 0;
				QProgressDialog progress("Applying filter...", "&Cancel", 0, numFiles, this);
				progress.setWindowModality(Qt::WindowModal);
				
				foreach (PbmImageViewer *mdiChild, imagesList) {
					progress.setValue(cont++);
					if (progress.wasCanceled())
						break;
					qstr = mdiChild->getFullFileName();
					pm = new TCMatriz2D<bool>(qstr.toStdString());
					if (tipos.at(0)==tipo){ //Espacial
						filtro = new TCFEMorfologiaMatematica<bool>(pm,tamStruElem, indice, fundo);
					} else {
						if (tipos.at(1)==tipo){ //IDFd34
							filtro = new TCFEMMIDFd34<bool>(pm, indice, fundo);
						} else if (tipos.at(2)==tipo){ //IDFd5711
							filtro = new TCFEMMIDFd5711<bool>(pm, indice, fundo);
						} else { //Euclidiana
							filtro = new TCFEMMIDFEuclidiana<bool>(pm, indice, fundo);
						}
						filtro->Go(pm,tamStruElem);
					}
					switch ( mtype ) {
						case mtErosion://===================================EROSÃO===================================
							filtro->Erosao(pm,tamStruElem);
							static int seqNumberErosion = 1;
							qstr = tr(".erosion%1.%2").arg(QString::number(seqNumberErosion++)).arg(mdiChild->getFileExt());
							break;
						case mtDilation://=================================DILATAÇÃO=================================
							filtro->Dilatacao(pm,tamStruElem);
							static int seqNumberDilation = 1;
							qstr = tr(".dilation%1.%2").arg(QString::number(seqNumberDilation++)).arg(mdiChild->getFileExt());
							break;
						case mtOpening://==================================ABERTURA==================================
							filtro->Abertura(pm,tamStruElem);
							static int seqNumberOpenning = 1;
							qstr = tr(".opening%1.%2").arg(QString::number(seqNumberOpenning++)).arg(mdiChild->getFileExt());
							break;
						case mtClosing://=================================FECHAMENTO=================================
							filtro->Fechamento(pm,tamStruElem);
							static int seqNumberClosing = 1;
							qstr = tr(".closing%1.%2").arg(QString::number(seqNumberClosing++)).arg(mdiChild->getFileExt());
							break;
					}
					pm->Write(qstr.toStdString());
					open( (mdiChild->getFilePath() + qstr).toStdString() );
					delete pm;
					pm = nullptr;
					delete filtro;
					filtro = nullptr;
				}
				progress.setValue(numFiles);
				QApplication::restoreOverrideCursor();
			}
		}
	}
}

void Lvp::erosion3D () {
	mathematicalMorphology3D( mtErosion );
}

void Lvp::dilation3D() {
	mathematicalMorphology3D( mtDilation );
}

void Lvp::opening3D() {
	mathematicalMorphology3D( mtOpening );
}

void Lvp::closing3D() {
	mathematicalMorphology3D( mtClosing );
}

void Lvp::mathematicalMorphology3D( MorphType mtype ){
	TCMatriz3D<bool> * pm = nullptr;
	TCFEMorfologiaMatematica3D<bool> * filtro = nullptr;
	QString qstr;
	//CBCDiscreta *maskd = dynamic_cast < CBCDiscreta * > ( mask );
	QList<DbmImageViewer *> imagesList = selectedDbmImagesList(); //lista de ponteiros para imagens selecionadas.
	bool ok;
	int tamStruElem = QInputDialog::getInt(this, tr(":. LVP"), tr("Enter the Structuring Element size:"), 1, 0, 99, 1, &ok);
	if (ok) {
		QMessageBox msgBox(this);
		msgBox.setWindowTitle(tr("LVP - Mathematical Morphology"));
		msgBox.setText(tr("Pore is:"));
		QPushButton *cancelButton = msgBox.addButton(QMessageBox::Cancel);
		msgBox.addButton(tr("&Write (0)"), QMessageBox::ActionRole);
		QPushButton *blackButton = msgBox.addButton(tr("&Black (1)"), QMessageBox::ActionRole);
		msgBox.setDefaultButton(blackButton);
		msgBox.exec();
		if ( msgBox.clickedButton() != cancelButton ) {
			QStringList tipos = ( QStringList() << tr("Spatial") << tr("IDF-d345") << tr("Euclidean") ); //seta os tipos
			QString tipo = QInputDialog::getItem(this, tr(":. LVP"), tr("Kind of metric:"), tipos, 0, false, &ok); //apresenta dialogo para escolha do item
			if (ok && !tipo.isEmpty()){
				QApplication::setOverrideCursor(Qt::WaitCursor);
				int indice, fundo;
				if (msgBox.clickedButton() == blackButton) {
					indice = 1; fundo = 0;
				} else {
					indice = 0; fundo = 1;
				}
				
				int numFiles = imagesList.size(); //número de arquivos que serão abertos
				int cont = 0;
				QProgressDialog progress("Applying 3D filter...", "&Cancel", 0, numFiles, this);
				progress.setWindowModality(Qt::WindowModal);
				
				foreach (DbmImageViewer *mdiChild, imagesList) {
					progress.setValue(cont++);
					if (progress.wasCanceled())
						break;
					qstr = mdiChild->getFullFileName();
					pm = new TCMatriz3D<bool>(qstr.toStdString());
					if (tipos.at(0)==tipo) { //Espacial
						filtro = new TCFEMorfologiaMatematica3D<bool>(pm,tamStruElem, indice, fundo);
					} else {
						if (tipos.at(1)==tipo) { //IDFd345
							filtro = new TCFEMMIDFd3453D<bool>(pm, indice, fundo);
						} else { //Euclidiana
							filtro = new TCFEMMIDFEuclidiana3D<bool>(pm, indice, fundo);
						}
						filtro->Go (pm,tamStruElem);
					}
					switch ( mtype ) {
						case mtErosion://===================================EROSÃO===================================
							filtro->Erosao(pm,tamStruElem);
							static int seqNumberErosion3D = 1;
							qstr = tr(".erosion3D-%1.%2").arg(QString::number(seqNumberErosion3D++)).arg(mdiChild->getFileExt());
							break;
						case mtDilation://=================================DILATAÇÃO=================================
							filtro->Dilatacao(pm,tamStruElem);
							static int seqNumberDilation3D = 1;
							qstr = tr(".dilation3D-%1.%2").arg(QString::number(seqNumberDilation3D++)).arg(mdiChild->getFileExt());
							break;
						case mtOpening://==================================ABERTURA==================================
							filtro->Abertura(pm,tamStruElem);
							static int seqNumberOpenning3D = 1;
							qstr = tr(".opening3D-%1.%2").arg(QString::number(seqNumberOpenning3D++)).arg(mdiChild->getFileExt());
							break;
						case mtClosing://=================================FECHAMENTO=================================
							filtro->Fechamento(pm,tamStruElem);
							static int seqNumberClosing3D = 1;
							qstr = tr(".closing3D-%1.%2").arg(QString::number(seqNumberClosing3D++)).arg(mdiChild->getFileExt());
							break;
					}
					pm->Write(qstr.toStdString());
					open( (mdiChild->getFilePath() + qstr).toStdString() );
					delete pm;
					pm = nullptr;
					delete filtro;
					filtro = nullptr;
				}
				progress.setValue(numFiles);
				QApplication::restoreOverrideCursor();
			}
		}
	}
}

void Lvp::skeletonV1() {
	skeleton(1);
}

void Lvp::skeletonV2() {
	skeleton(2);
}

void Lvp::skeletonV3() {
	skeleton(3);
}

void Lvp::skeletonV4() {
	skeleton(4);
}

void Lvp::skeletonV5() {
	skeleton(5);
}

void Lvp::skeleton(int type) {
	QList<PbmImageViewer *> imagesList = selectedPbmImagesList(); //lista de ponteiros para imagens selecionadas.
	bool ok;
	int tamMask = QInputDialog::getInt(this, tr(":. Skeleton"), tr("Enter the mask size:"), 1, 0, 99, 1, &ok);
	if (ok) {
		int numFiles = imagesList.size(); //número de arquivos que serão abertos
		int cont = 0;
		QProgressDialog progress("Applying filter...", "&Cancel", 0, numFiles, this);
		progress.setWindowModality(Qt::WindowModal);
		foreach (PbmImageViewer *mdiChild, imagesList) {
			progress.setValue(cont);
			if (progress.wasCanceled())
				break;
			QString qstr = mdiChild->getFullFileName();
			TCMatriz2D<int> * pm = new TCMatriz2D<int>(qstr.toStdString());
			CFEEsqueleto * filtro = nullptr;
			QString nomeArquivo;
			switch (type) {
				case 1:
					filtro = new CFEEsqueletoV1(pm,tamMask);
					nomeArquivo="skeleton-V1";
					break;
				case 2:
					filtro = new CFEEsqueletoV2(pm,tamMask);
					nomeArquivo="skeleton-V2";
					break;
				case 3:
					filtro = new CFEEsqueletoV3(pm,tamMask);
					nomeArquivo="skeleton-V3";
					break;
				case 4:
					filtro = new CFEEsqueletoV4(pm,tamMask);
					nomeArquivo="skeleton-V4";
					break;
				case 5:
					filtro = new CFEEZhangSuen(pm,tamMask);
					nomeArquivo="skeleton-ZhangSuen";
					break;
				default:
					filtro = new CFEEsqueletoV4(pm,tamMask);
					nomeArquivo="skeleton-V4";
			}
			filtro->Go(pm,tamMask);
			static int seqNumberSkeleton = 1;
			qstr = tr(".%1-%2.%3").arg(nomeArquivo).arg(QString::number(seqNumberSkeleton++)).arg(mdiChild->getFileExt());
			pm->Write(qstr.toStdString());
			open( (mdiChild->getFilePath() + qstr).toStdString() );
			delete pm;
			delete filtro;
			cont++;
		}
	}
}

void Lvp::idf() {
	//QMessageBox::information(this, tr("LVP"), tr("Funcionalidade não implementada!"));
	bool ok;
	static int seqNumberIDF = 1;
	//cria diálogo para informar quem é poro
	QMessageBox msgBox(this);
	msgBox.setWindowTitle(tr("LVP - IDF"));
	msgBox.setText(tr("Pore is:"));
	msgBox.addButton(QMessageBox::Cancel);
	QPushButton *writeButton = msgBox.addButton(tr("&Write (0)"), QMessageBox::ActionRole);
	QPushButton *blackButton = msgBox.addButton(tr("&Black (1)"), QMessageBox::ActionRole);
	msgBox.setDefaultButton(blackButton);
	int indice, fundo;
	if ( PbmImageViewer *mdiChild = activePbmImageViewer() ) {
		QString qstr = mdiChild->getFullFileName();
		string stdstr = qstr.toStdString();
		msgBox.exec();
		if (msgBox.clickedButton() == blackButton) {
			indice = 1; fundo = 0;
		} else if (msgBox.clickedButton() == writeButton) {
			indice = 0; fundo = 1;
		} else {
			QApplication::restoreOverrideCursor();
			return;
		}
		QStringList tipos = ( QStringList() << tr("IDF-d34") << tr("IDF-d5711") << tr("Euclidean") ); //seta os tipos
		QString tipo = QInputDialog::getItem(this, tr(":. IDF"), tr("Kind of metric:"), tipos, 0, false, &ok); //apresenta dialogo para escolha do item
		if (ok && !tipo.isEmpty()){
			QApplication::setOverrideCursor(Qt::WaitCursor);
			if ( mdiChild->pm == nullptr )
				mdiChild->pm = new TCMatriz2D<bool>(stdstr);
			TCFEMMIDF<bool> *idf2D = nullptr;
			if (tipos.at(0)==tipo) { //d34
				idf2D = new TCFEMMIDFd34<bool>(mdiChild->pm, indice, fundo);
			} else if (tipos.at(1)==tipo) { //d5711
				idf2D = new TCFEMMIDFd5711<bool>(mdiChild->pm, indice, fundo);
			} else if (tipos.at(2)==tipo) { //Euclidiana
				idf2D = new TCFEMMIDFEuclidiana<bool>(mdiChild->pm, indice, fundo);
			}
			idf2D->Go(mdiChild->pm);
			qstr = tr(".idf%1.pgm").arg(QString::number(seqNumberIDF++));
			stdstr = qstr.toStdString();
			idf2D->SetFormato(P5_X_Y_GRAY_BINARY);
			idf2D->NumCores(idf2D->MaiorValor());
			idf2D->Path(mdiChild->getFilePath().toStdString());
			idf2D->Write(stdstr);
			open( (mdiChild->getFilePath() + qstr).toStdString() );
			delete idf2D;
		}
	} else if ( DbmImageViewer *mdiChild = activeDbmImageViewer() ) {
		QString qstr = mdiChild->getFullFileName();
		string stdstr = qstr.toStdString();
		msgBox.exec();
		if (msgBox.clickedButton() == blackButton) {
			indice = 1; fundo = 0;
		} else if (msgBox.clickedButton() == writeButton) {
			indice = 0; fundo = 1;
		} else {
			QApplication::restoreOverrideCursor();
			return;
		}
		QApplication::setOverrideCursor(Qt::WaitCursor);
		if ( mdiChild->pm3D == nullptr )
			mdiChild->pm3D = new TCImagem3D<bool>(stdstr);
		TCFEMMIDFd3453D<bool> *idf3D = nullptr;
		TCMatriz3D<bool> * obj3D = dynamic_cast<TCMatriz3D<bool> *>(mdiChild->pm3D);
		idf3D = new TCFEMMIDFd3453D<bool>(obj3D, indice, fundo);
		idf3D->Go(obj3D);
		qstr = tr(".idf%1.dgm").arg(QString::number(seqNumberIDF++));
		stdstr = qstr.toStdString();
		idf3D->SetFormato(D5_X_Y_Z_GRAY_BINARY);
		idf3D->NumCores(idf3D->MaiorValor());
		idf3D->Path(mdiChild->getFilePath().toStdString());
		idf3D->Write(stdstr);
		qstr = mdiChild->getFilePath() + qstr;
		open(qstr.toStdString());
		delete idf3D;
	}
	QApplication::restoreOverrideCursor();
}

void Lvp::ira() {
	static int seqNumberIRA = 1;
	//cria diálogo para informar quem é poro
	QMessageBox msgBox(this);
	msgBox.setWindowTitle(tr("LVP - IRA"));
	msgBox.setText(tr("Pore is:"));
	msgBox.addButton(QMessageBox::Cancel);
	QPushButton *writeButton = msgBox.addButton(tr("&Write (0)"), QMessageBox::ActionRole);
	QPushButton *blackButton = msgBox.addButton(tr("&Black (1)"), QMessageBox::ActionRole);
	msgBox.setDefaultButton(blackButton);
	int indice, fundo;
	if ( PbmImageViewer *mdiChild = activePbmImageViewer() ) {
		QString qstr = mdiChild->getFullFileName();
		string stdstr = qstr.toStdString();
		msgBox.exec();
		if (msgBox.clickedButton() == blackButton) {
			indice = 1; fundo = 0;
		} else if (msgBox.clickedButton() == writeButton) {
			indice = 0; fundo = 1;
		} else {
			QApplication::restoreOverrideCursor();
			return;
		}
		QApplication::setOverrideCursor(Qt::WaitCursor);
		if ( mdiChild->pm == nullptr )
			mdiChild->pm = new TCMatriz2D<bool>(stdstr);
		TCFEMMIRA<bool> *ira2D = nullptr;
		TCMatriz2D<int> *pmira2D = nullptr;
		ira2D = new TCFEMMIRA<bool>(mdiChild->pm, indice, fundo);
		pmira2D = ira2D->Go();
		qstr = tr(".ira%1.pgm").arg(QString::number(seqNumberIRA++));
		stdstr = qstr.toStdString();
		pmira2D->SetFormato(P2_X_Y_GRAY_ASCII);
		pmira2D->NumCores(pmira2D->MaiorValor());
		pmira2D->salvarAlinhado = true;
		pmira2D->Path(mdiChild->getFilePath().toStdString());
		pmira2D->Write(stdstr);
		open( (mdiChild->getFilePath() + qstr).toStdString() );
		delete ira2D;
		delete pmira2D;
	} else if ( DbmImageViewer *mdiChild = activeDbmImageViewer() ) {
		QString qstr = mdiChild->getFullFileName();
		string stdstr = qstr.toStdString();
		msgBox.exec();
		if (msgBox.clickedButton() == blackButton) {
			indice = 1; fundo = 0;
		} else if (msgBox.clickedButton() == writeButton) {
			indice = 0; fundo = 1;
		} else {
			QApplication::restoreOverrideCursor();
			return;
		}
		QApplication::setOverrideCursor(Qt::WaitCursor);
		if ( mdiChild->pm3D == nullptr )
			mdiChild->pm3D = new TCImagem3D<bool>(stdstr);
		TCFEMMIRA3D<bool> *ira3D = nullptr;
		TCMatriz3D<int> *pmira3D = nullptr;
		TCMatriz3D<bool> * obj3D = dynamic_cast<TCMatriz3D<bool> *>(mdiChild->pm3D);
		ira3D = new TCFEMMIRA3D<bool>(obj3D, indice, fundo);
		pmira3D = ira3D->Go();
		qstr = tr(".ira%1.dgm").arg(QString::number(seqNumberIRA++));
		stdstr = qstr.toStdString();
		pmira3D->SetFormato(D2_X_Y_Z_GRAY_ASCII);
		pmira3D->NumCores(pmira3D->MaiorValor());
		pmira3D->Path(mdiChild->getFilePath().toStdString());
		pmira3D->Write(stdstr);
		open( (mdiChild->getFilePath() + qstr).toStdString() );
		delete ira3D;
	}
	QApplication::restoreOverrideCursor();
}

void Lvp::connectivity3D() {
	//QMessageBox::information(this, tr("LVP"), tr("Funcionalidade não implementada!"));
	TCImagem3D<bool> * pm3Dcon = nullptr;
	DbmImageViewer * child3D = nullptr;
	child3D = activeDbmImageViewer();
	if ( ! child3D ) {
		QApplication::restoreOverrideCursor();
		QMessageBox::information(this, tr("LVP"), tr("Error while trying to retrieve 3D image!"));
		return;
	}
	TCFEConectividade3D<bool> * objCon = nullptr;
	if ( child3D->pm3D ) {
		QMessageBox msgBox(this);
		msgBox.setWindowTitle(tr("LVP - 3D Connectivity"));
		msgBox.setText(tr("Pore is:"));
		msgBox.addButton(QMessageBox::Cancel);
		QPushButton *writeButton = msgBox.addButton(tr("&Write (0)"), QMessageBox::ActionRole);
		QPushButton *blackButton = msgBox.addButton(tr("&Black (1)"), QMessageBox::ActionRole);
		msgBox.setDefaultButton(blackButton);
		msgBox.exec();
		
		int indice, fundo;
		if (msgBox.clickedButton() == blackButton) {
			indice = 1; fundo = 0;
		} else if (msgBox.clickedButton() == writeButton) {
			indice = 0; fundo = 1;
		} else {
			return;
		}
		objCon = new TCFEConectividade3D<bool>( child3D->pm3D, indice, fundo );
		QApplication::setOverrideCursor(Qt::WaitCursor);
		if ( objCon ) {
			pm3Dcon = objCon->GetImagemConectada( child3D->pm3D );
			if (pm3Dcon) {
				static int seqNumberConnectivity = 1;
				QString qstr = tr(".connected%1.%2").arg(QString::number(seqNumberConnectivity++)).arg(child3D->getFileExt());
				if ( pm3Dcon->Write( qstr.toStdString() ) )
					open( pm3Dcon->Path() + qstr.toStdString() );
			} else {
				QApplication::restoreOverrideCursor();
				QMessageBox::information(this, tr("LVP"), tr("The image does not have connectivity on the Y axis!"));
			}
		} else {
			QApplication::restoreOverrideCursor();
			QMessageBox::information(this, tr("LVP"), tr("Error trying to determine the 3D connectivity!"));
		}
		QApplication::restoreOverrideCursor();
	} else {
		QMessageBox::information(this, tr("LVP"), tr("Error trying to determine the 3D connectivity!"));
	}
}

void Lvp::connectedObjects(){
	static int seqNumberConObj = 1;
	if ( PbmImageViewer *mdiChild = activePbmImageViewer() ) {
		QString qstr = mdiChild->getFullFileName();
		string stdstr = qstr.toStdString();
		QMessageBox msgBox(this);
		msgBox.setWindowTitle(tr("LVP - Segmentation"));
		msgBox.setText(tr("Pore is:"));
		msgBox.addButton(QMessageBox::Cancel);
		QPushButton *writeButton = msgBox.addButton(tr("&Write (0)"), QMessageBox::ActionRole);
		QPushButton *blackButton = msgBox.addButton(tr("&Black (1)"), QMessageBox::ActionRole);
		msgBox.setDefaultButton(blackButton);
		msgBox.exec();
		int indice, fundo;
		if (msgBox.clickedButton() == blackButton) {
			indice = 1; fundo = 0;
		} else if (msgBox.clickedButton() == writeButton) {
			indice = 0; fundo = 1;
		} else {
			QApplication::restoreOverrideCursor();
			return;
		}
		QApplication::setOverrideCursor(Qt::WaitCursor);
		if ( mdiChild->pm == nullptr )
			mdiChild->pm = new TCMatriz2D<bool>(stdstr);
		TCRotulador2D<bool> *obj = new TCRotulador2D<bool>(mdiChild->pm, indice, fundo);
		obj->Go(mdiChild->pm);
		qstr = tr(".objCon%1.pgm").arg(QString::number(seqNumberConObj++));
		stdstr = qstr.toStdString();
		obj->SetFormato(P5_X_Y_GRAY_BINARY);
		obj->NumCores(obj->NumeroObjetos());
		obj->Path(mdiChild->getFilePath().toStdString());
		obj->Write(stdstr);
		open( (mdiChild->getFilePath() + qstr).toStdString() );
		delete obj;
	}/* else if ( DbmImageViewer *mdiChild = activeDbmImageViewer() ) {
		QString qstr = mdiChild->getFullFileName();
		string stdstr = qstr.toStdString();
		QApplication::setOverrideCursor(Qt::WaitCursor);
		if ( mdiChild->pm3D == nullptr )
			mdiChild->pm3D = new TCImagem3D<bool>(stdstr);
		TCFEMMIDFd3453D<bool> *idf3D = nullptr;
		TCMatriz3D<bool> * obj3D = dynamic_cast<TCMatriz3D<bool> *>(mdiChild->pm3D);
		idf3D = new TCFEMMIDFd3453D<bool>(obj3D);
		idf3D->Go(obj3D);
		qstr = tr(".idf%1.dgm").arg(QString::number(seqNumberIDF++));
		stdstr = qstr.toStdString();
		idf3D->SetFormato(D5_X_Y_Z_GRAY_BINARY);
		idf3D->NumCores(idf3D->MaiorValor());
		idf3D->Path(mdiChild->getFilePath().toStdString());
		idf3D->Write(stdstr);
		qstr = mdiChild->getFilePath() + qstr;
		open(qstr.toStdString());
		delete idf3D;
	}*/
	QApplication::restoreOverrideCursor();
}

void Lvp::addCurve() {
	QStringList filespath;
	Ploter * plot = nullptr;
	plot = activePloter();
	if ( plot ) {
		QString ext = plot->getFileExt();
		if (ext == "cor" or ext == "COR")
			filespath = QFileDialog::getOpenFileNames(this, tr("Open File(s)"), lastOpenPath, tr("Correlation Files (*.cor)"));
		else if (ext == "dtp" or ext == "DTP" or ext == "dts" or ext == "DTS" or ext == "dtg" or ext == "DTG")
			filespath = QFileDialog::getOpenFileNames(this, tr("Open File(s)"), lastOpenPath, tr("Distribution Files (*.dtp *.dts *.dtg)"));
		else if (ext == "rpc" or ext == "RPC")
			filespath = QFileDialog::getOpenFileNames(this, tr("Open File(s)"), lastOpenPath, tr("Relative Permeability Curves (*.rpc)"));
	}
	if ( ! filespath.isEmpty() ) {
		QDir lop( filespath.at(0) );
		lastOpenPath = lop.canonicalPath();
		plot->addCurves( filespath );
		fileWatcher->addPaths(filespath);
	}
}

void Lvp::accumulated() {
	Ploter * plot = nullptr;
	plot = activePloter();
	if ( plot ) {
		plot->accumulated();
		if (plot->IsAccumulated()){
			pushButtonAccumulated->setText(QString::fromUtf8("Hide Accumulated"));
		} else {
			pushButtonAccumulated->setText(QString::fromUtf8("Show Accumulated"));
		}
	}
}

void Lvp::average() {
	Ploter * plot = nullptr;
	plot = activePloter();
	if ( plot ) {
		QString qstr;
		QString ext = plot->getFileExt();
		
		set<string> fnames;
		for (int it = 0; it < plot->pathCurves.size(); ++it){
			fnames.insert((plot->pathCurves[it]).toStdString());
		}
		if ( ext == "cor" or ext == "COR" ){
			CCorrelacaoFFT obj( 512 );
			if(obj.Media(fnames)){
				static int seqNumberAverage = 1;
				qstr = tr(".average%1").arg(QString::number(seqNumberAverage++));
				//obj.path = plot->getFullFileName().left(plot->getFullFileName().lastIndexOf("/")+1).toStdString();
				obj.Write(qstr.toStdString());
				qstr.append(".cor");
				statusBar()->showMessage(tr("%1 Ok!").arg(qstr), 10000);
				open(qstr.toStdString());
			} else {
				QMessageBox::information(this, tr("LVP"), tr("Error - Was not possible to calculate the average!"));
			}
		} else if ( ext == "dtp" or ext == "DTP" ) {
			CDistribuicao *objDist = new CDistribuicao();
			if( objDist->Media(fnames, CDistribuicao::dtp) ){
				static int seqNumberAverage = 1;
				qstr = tr(".average-%1").arg(QString::number(seqNumberAverage++));
				objDist->path = plot->getFilePath ().toStdString();
				if ( objDist->Write(qstr.toStdString()) ) {
					qstr.append(".dtp");
					statusBar()->showMessage(tr("%1 Ok!").arg(qstr), 10000);
					open(qstr.toStdString());
				} else {
					QMessageBox::information(this, tr("LVP"), tr("Error! - Was not possible to create the average!"));
				}
			} else {
				QMessageBox::information(this, tr("LVP"), tr("Error! - Was not possible to calculate the average!"));
			}
			delete objDist;
		} else if ( ext == "dts" or ext == "DTS" ) {
			CDistribuicao *objDist = new CDistribuicao();
			if( objDist->Media(fnames, CDistribuicao::dts) ){
				static int seqNumberAverage = 1;
				qstr = tr(".average-%1").arg(QString::number(seqNumberAverage++));
				objDist->path = plot->getFilePath ().toStdString();
				if ( objDist->Write(qstr.toStdString()) ) {
					qstr.append(".dts");
					statusBar()->showMessage(tr("%1 Ok!").arg(qstr), 10000);
					open(qstr.toStdString());
				} else {
					QMessageBox::information(this, tr("LVP"), tr("Error! - Was not possible to create the average!"));
				}
			} else {
				QMessageBox::information(this, tr("LVP"), tr("Error! - Was not possible to calculate the average!"));
			}
			delete objDist;
		}
	} else {
		QMessageBox::information(this, tr("LVP"), tr("Error to calculate the average!"));
	}
}

void Lvp::invertPoro() {
	GLWidget * glwidget = nullptr;
	glwidget = activeGLWidget();
	if ( glwidget ) {
		if ( glwidget->getPM3D()!=nullptr ) {
			glwidget->invertPore();
		} else if ( glwidget->getPM3Di()!=nullptr ) {
			//glwidget->dialog
			glwidget->dialog->show();
			/*
			bool ok;
			int current = glwidget->tonsList.indexOf( glwidget->getPore() );
			QString poreValue = QInputDialog::getItem(this, tr(":. 3D Visualization"), tr("Select a value:"), glwidget->qstrTonsList, current, false, &ok);
			if ( ok && ! poreValue.isEmpty() )
				glwidget->setPore( poreValue.toInt() );
			*/
		}
	}
}

void Lvp::reconstructionGT() {
	QString filepath;
	Ploter * plot = nullptr;
	plot = activePloter();
	if ( plot && plot->getFileExt() == "cor") {
		filepath = plot->getFullFileName();
	} else {
		filepath = QFileDialog::getOpenFileName(this, tr("Open File"), lastOpenPath, tr("Correlation Files (*.cor)"));
	}
	if ( ! filepath.isNull() ) {
		QApplication::setOverrideCursor(Qt::WaitCursor);
		if ( dialogGT ) {
			dialogGT->show();
			dialogGT->lineEditFile->setText(filepath);
		} else {
			dialogGT = new Reconstruction(filepath, this);
			if ( dialogGT ) {
				dialogGT->show();
			} else {
				QApplication::restoreOverrideCursor();
				QMessageBox::information(this, tr("LVP"), tr("Erro! - To create reconstruction dialog!"));
			}
		}
		QDir lop( filepath );
		lastOpenPath = lop.canonicalPath();
	}
	QApplication::restoreOverrideCursor();
}

void Lvp::exReconstructionGT() {
	QApplication::setOverrideCursor(Qt::WaitCursor);
	int af = dialogGT->spinBoxAF->value();
	int np = dialogGT->spinBoxNP->value();
	double sp = dialogGT->doubleSpinBoxSP->value() * 1.0e-6; // 0.000001  -  Size Pixel - da imagem 2D original.
	int gt, nx, ny, nz;
	dialogGT->radioButtonGT1->isChecked() ? gt = 1 : gt = 2;
	switch (dialogGT->comboBoxXYZ->currentIndex()) {
		case 0: nx = ny = nz =  50; break;
		case 1: nx = ny = nz = 100; break;
		case 2: nx = ny = nz = 150; break;
		case 3: nx = ny = nz = 200; break;
		case 4: nx = ny = nz = 250; break;
		case 5: nx = ny = nz = 300; break;
		case 6: nx = ny = nz = 350; break;
		case 7: nx = ny = nz = 400; break;
		case 8: nx = ny = nz = 450; break;
		case 9: nx = ny = nz = 500; break;
		default: nx = ny = nz = 100;
	}
	//está reconstruindo a partir da lista de graficos selecionados.
	foreach (Ploter *mdiChild, selectedPloterList()) {
		if (mdiChild->getFileExt() == "cor") {
			CReconstrucaoBueno * ptrRec = new CReconstrucaoBueno(mdiChild->getFullFileName().toStdString(), af, np, sp, nx, ny, nz, gt);
			if ( ptrRec->Go( ) ) {
				open( ptrRec->getNomeArquivo() );
			} else {
				QApplication::restoreOverrideCursor();
				QMessageBox::information(this, tr("LVP"), tr("Erro! - To create 3D Image File: \"%1\"").arg( QString::fromStdString( ptrRec->getNomeArquivo() ) ) );
			}
			delete ptrRec;
		}
	}
	dialogGT->close();
	delete dialogGT;
	dialogGT = nullptr;
	QApplication::restoreOverrideCursor();
}

void Lvp::reconstructionES() {
	QString filepath;
	Ploter * plot = nullptr;
	plot = activePloter();
	//if ( plot and  ( QMessageBox::Ok == QMessageBox::question( this, tr("LVP"), tr("Do you want create the reconstruction by active correlation [Ok] or by another on [Cancel]?"), QMessageBox::Ok | QMessageBox::Cancel ) ) ) {
	if ( plot && plot->getFileExt() == "dts") {
		filepath = plot->getFullFileName();
	} else {
		filepath = QFileDialog::getOpenFileName(this, tr("Open File"), lastOpenPath, tr("Distribution Files (*.dts)"));
	}
	if ( ! filepath.isNull() ) {
		QApplication::setOverrideCursor(Qt::WaitCursor);
		if ( dialogES ) {
			dialogES->show();
			dialogES->lineEditFile->setText(filepath);
			dialogES->readInfo(); //Lê o valor referente a área de sólidos no arquivo .dts
		} else {
			dialogES = new ReconstructionES(filepath, this);
			if ( dialogES ) {
				dialogES->show();
			} else {
				QApplication::restoreOverrideCursor();
				QMessageBox::information(this, tr("LVP"), tr("Erro! - To create ES reconstruction dialog!"));
			}
		}
		QDir lop( filepath );
		lastOpenPath = lop.canonicalPath();
	}
	QApplication::restoreOverrideCursor();
}

void Lvp::exReconstructionES() {
	QApplication::setOverrideCursor(Qt::WaitCursor);
	QString filepath = dialogES->lineEditFile->text();
	double phi = dialogES->doubleSpinBoxPhi->value();
	double of = dialogES->doubleSpinBoxOF->value();
	double bf = dialogES->doubleSpinBoxBF->value(); // Size Pixel - da imagem 2D original.
	double sp = dialogES->doubleSpinBoxSP->value();
	int af = dialogES->spinBoxAF->value();
	int nxyz;
	switch (dialogES->comboBoxXYZ->currentIndex()) {
		case 0: nxyz = 100; break;
		case 1: nxyz = 150; break;
		case 2: nxyz = 200; break;
		case 3: nxyz = 250; break;
		case 4: nxyz = 300; break;
		case 5: nxyz = 350; break;
		case 6: nxyz = 400; break;
		case 7: nxyz = 450; break;
		case 8: nxyz = 500; break;
		default: nxyz = 100;
	}
	CReconstrucaoEsferas * ptrRec = nullptr;
	cerr << "nxyz: " << nxyz << "\nof: " << of << "\nbf: " << bf << endl;
	cerr << "phi: " << phi << "\nsp: " << sp << "\naf: " << af << endl;
	ptrRec = new CReconstrucaoEsferas( phi/100, nxyz, of, bf, af, sp );
	dialogES->close();
	delete dialogES;
	dialogES = nullptr;
	if ( ptrRec ) {
		if (ptrRec->Go( filepath.toStdString() ) ){
			open( ptrRec->getNomeArquivo() );
		} else {
			QMessageBox::information( this, tr("LVP"), tr("Erro! - To create 3D Reconstruction") );
		}
	} else {
		QApplication::restoreOverrideCursor();
		QMessageBox::information( this, tr("LVP"), tr("Erro! - To create 3D Reconstruction") );
	}
	delete ptrRec;
	QApplication::restoreOverrideCursor();
}

void Lvp::intrinsicPermeabilityByNetwork() {
	DbmImageViewer * child3D = nullptr;
	unsigned int nx=100;
	if ( (child3D = activeDbmImageViewer()) ) {
		if ( ! child3D->pm3D ) {
			child3D->pm3D = new TCImagem3D<bool>( child3D->getFullFileName().toStdString() );
		}
		if ( child3D->pm3D ) {
			CPermeabilidadeIntrinsecaByRede * objPerIn = nullptr;
			objPerIn = new CPermeabilidadeIntrinsecaByRede();
			int limiteIteracoes = 100000;
			double limiteErro = 1.0e-06;
			if ( objPerIn ) {
				bool ok;
				limiteIteracoes = QInputDialog::getInt(this, tr("Intrinsic Permeability"), tr("Enter the limit iterations number:"), 100000, 1000, 10000000, 1, &ok);
				if (ok) {
					QStringList itens = ( QStringList() << tr("1.0e-10") << tr("1.0e-09") << tr("1.0e-08") << tr("1.0e-07") << tr("1.0e-06") << tr("1.0e-05") << tr("1.0e-04") << tr("1.0e-03") << tr("1.0e-02") << tr("1.0e-01") );
					QString item = QInputDialog::getItem(this, tr("Intrinsic Permeability"), tr("Enter the limit error number:"), itens, 4, false, &ok);
					if (ok) {
						limiteErro = item.toDouble(&ok);
						nx = QInputDialog::getInt(this, tr("Intrinsic Permeability"), tr("Enter the network size:"), 100, 50, 1000, 10, &ok);
					}
				}
				if (ok) {
					QApplication::setOverrideCursor(Qt::WaitCursor);
					objPerIn->CriarObjetos(nx,nx,nx);
					objPerIn->SetarPropriedadesSolver(limiteErro,limiteIteracoes);
					double permeabilidade = objPerIn->Go(child3D->pm3D,nx,nx,nx,nx/2,2,1,1,EModelo::ONZE,1,0);
					QApplication::restoreOverrideCursor();
					QMessageBox::information(this, tr("LVP"), tr("Intrinsic Permeability = %1 mD").arg(permeabilidade));
				}
			}
		} else {
			QMessageBox::information(this, tr("LVP"), tr("Error while trying to retrieve 3D image!"));
		}
	} else {
		QMessageBox::information(this, tr("LVP"), tr("Error while trying to retrieve 3D image!"));
		return;
	}
}

void Lvp::intrinsicPermeability() {
	//QMessageBox::information(this, tr("LVP"), tr("Funcionalidade não implementada!"));
	//Antes de mudar a classe CPermeabilidadeIntrinseca para template precisa
	//implmentar template em Grafo...
	//Antes de implementar template no GCS, resolvi criar nova matriz<int> e copiar os dados da matiz<bool>
	DbmImageViewer * child3D = nullptr;
	if ( (child3D = activeDbmImageViewer()) ) {
		/*
		if ( ! child3D->pm3D ) {
			child3D->pm3D = new TCImagem3D<bool>( child3D->getFullFileName().toStdString() );
		}
		if ( child3D->pm3D ) {
		*/
		// A matriz precisa ser do tipo int para funcionar com o grafo.
		TCImagem3D<int> * pm3D = new TCImagem3D<int>( child3D->getFullFileName().toStdString() );
		if ( pm3D ) {
			CSimPermeabilidadeIntrinseca * objPerIn = nullptr;
			objPerIn = new CSimPermeabilidadeIntrinseca();
			unsigned short int modelo = 3;
			int limiteIteracoes = 100000;
			double limiteErro = 1.0e-06;
			if ( objPerIn ) {
				bool ok;
				limiteIteracoes = QInputDialog::getInt(this, tr("Intrinsic Permeability"), tr("Enter the limit iterations number:"), 100000, 1000, 10000000, 1, &ok);
				if (ok) {
					QStringList itens = ( QStringList() << tr("1.0e-10") << tr("1.0e-09") << tr("1.0e-08") << tr("1.0e-07") << tr("1.0e-06") << tr("1.0e-05") << tr("1.0e-04") << tr("1.0e-03") << tr("1.0e-02") << tr("1.0e-01") );
					QString item = QInputDialog::getItem(this, tr("Intrinsic Permeability"), tr("Enter the limit error number:"), itens, 4, false, &ok);
					if (ok) {
						limiteErro = item.toDouble(&ok);
						cerr << "limiteErro: " << limiteErro << endl;
						modelo = QInputDialog::getInt(this, tr("Intrinsic Permeability"), tr("Enter the Graph model:"), 4, 1, 5, 1, &ok);
					}
				}
				if (ok) {
					QApplication::setOverrideCursor(Qt::WaitCursor);
					objPerIn->CriarObjetos( );
					objPerIn->SetarPropriedadesSolver(limiteErro,limiteIteracoes);
					double permeabilidade = objPerIn->Go ( pm3D, modelo );
					QApplication::restoreOverrideCursor();
					QMessageBox::information(this, tr("LVP"), tr("Intrinsic Permeability = %1 mD").arg(permeabilidade));
				}
			}
		} else {
			QMessageBox::information(this, tr("LVP"), tr("Error while trying to retrieve 3D image!"));
		}
	} else {
		QMessageBox::information(this, tr("LVP"), tr("Error while trying to retrieve 3D image!"));
		return;
	}
}

void Lvp::relativePermeability() {
	PbmImageViewer * child3D = nullptr;
	if ( (child3D = activePbmImageViewer()) ) {
		QString qstr = tr("%1%2.rpc").arg( child3D->getFilePath( ), child3D->getFileNameNoExt() );
		ofstream fout( qstr.toStdString().c_str() );
		
		CSimPermeabilidadeRelativa * objPerRel = nullptr;
		objPerRel = new CSimPermeabilidadeRelativa( fout );
		if ( objPerRel ) {
			bool ok;
			//int limiteIteracoes = QInputDialog::getInt(this, tr("Relative Permeability"), tr("Enter the limit iterations number:"), 10000, 1000, 10000000, 1, &ok);
			objPerRel->limiteIteracoes = QInputDialog::getInt(this, tr("Relative Permeability"), tr("Enter the limit iterations number:"), 100000, 1000, 10000000, 1, &ok);
			if (ok) {
				QStringList itens = ( QStringList() << tr("1.0e-10") << tr("1.0e-09") << tr("1.0e-08") << tr("1.0e-07") << tr("1.0e-06") << tr("1.0e-05") << tr("1.0e-04") << tr("1.0e-03") << tr("1.0e-02") << tr("1.0e-01") );
				QString item = QInputDialog::getItem(this, tr("Relative Permeability"), tr("Enter the limit error number:"), itens, 4, false, &ok);
				if (ok) {
					objPerRel->limiteErro = item.toDouble(&ok);
				}
				if (ok) {
					int retorno = QMessageBox::question( this, tr("LVP"), tr("Do you want to save images A and B?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel );
					switch (retorno) {
						case QMessageBox::Yes:
							objPerRel->salvarImagensAB = true;
							break;
						case QMessageBox::No:
							objPerRel->salvarImagensAB = false;
							break;
						case QMessageBox::Cancel:
							ok = false;
							break;
						default:
							ok = false;
					}
				}
				if (ok) {
					int retorno = QMessageBox::question( this, tr("LVP"), tr("Do you want to correct the opening image?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel );
					switch (retorno) {
						case QMessageBox::Yes:
							objPerRel->corrigirAbertura = true;
							break;
						case QMessageBox::No:
							objPerRel->corrigirAbertura = false;
							break;
						case QMessageBox::Cancel:
							ok = false;
							break;
						default:
							ok = false;
					}
				}
			}
			if (ok) {
				QApplication::setOverrideCursor(Qt::WaitCursor);
				if ( objPerRel->Go ( child3D->getFullFileName().toStdString() ) ) {
					open( qstr.toStdString().c_str() );
					QApplication::restoreOverrideCursor();
				} else {
					QApplication::restoreOverrideCursor();
					QMessageBox::information(this, tr("LVP"), tr("Error trying to determine the Relative Permeability!"));
				}
			}
			delete objPerRel;
			fout.close();
		} else {
			QMessageBox::information(this, tr("LVP"), tr("Error creating Relative Permeability Object!"));
			return;
		}
	} else {
		QMessageBox::information(this, tr("LVP"), tr("Error while trying to retrieve 3D image!"));
	}
}

void Lvp::segmentationPoresThroats() {
	DbmImageViewer* child = activeDbmImageViewer();
	if ( child != 0 ){
		if ( child->pm3D != 0 ){
			dialogPoresThroats = new PoresThroats( this, child );
			dialogPoresThroats->show();
		} else {
			QMessageBox::information(this, tr("LVP"), tr("Error: Image viewer not active!"));
		}
	} else {
		QMessageBox::information(this, tr("LVP"), tr("Error: Image viewer not active!"));
	}
}

void Lvp::exSegmentationPoresThroats(){
	static int seqNumberSPT = 1;
	QString filepath = tr(".segmented%1.dgm").arg(QString::number(seqNumberSPT++));
	int indice, fundo;

	if (dialogPoresThroats->radioButtonBlack->isChecked()) {
		indice = 1;
		fundo = 0;
	} else {
		indice = 0;
		fundo = 1;
	}

	QApplication::setOverrideCursor(Qt::WaitCursor);

	if (dialogPoresThroats->comboBoxModel->currentText() == "Pores Throats By IRA Model 0" ) {
		CSegPorosGargantas3D filtro = CSegPorosGargantas3D(dialogPoresThroats->child->pm3D, indice, fundo );
		filtro.RaioMaximoElementoEstruturante(dialogPoresThroats->spinBoxRmax->value());
		filtro.FatorReducaoRaioElemEst(dialogPoresThroats->spinBoxRreduction->value());
		filtro.IncrementoRaioElementoEstruturante(dialogPoresThroats->spinBoxRinc->value());
		filtro.SalvarResultadosParciais(dialogPoresThroats->checkBoxSPR->isChecked());
		filtro.GerarDetalhesObjetos(dialogPoresThroats->checkBoxGOD->isChecked());
		filtro.Go(0);
		filtro.Write(filepath.toStdString());
		open( filepath.toStdString() );
		if (filtro.GerarDetalhesObjetos()) {
			filepath = dialogPoresThroats->child->getFilePath();
			filepath+= dialogPoresThroats->child->getFileNameNoExt() + ".rsl";
			filtro.SalvarListaObjetos(filepath.toStdString());
			open( filepath.toStdString() );
		}
	} else {
		EModelo model = ONZE;;
		if (dialogPoresThroats->comboBoxModel->currentText() == "Openning Dilatation Model 11" ) {
			model = ONZE;
		} else if (dialogPoresThroats->comboBoxModel->currentText() == "Openning Dilatation Model 10" ) {
			model = DEZ;
		} else if (dialogPoresThroats->comboBoxModel->currentText() == "Openning Dilatation Model 9" ) {
			model = NOVE;
		} else if (dialogPoresThroats->comboBoxModel->currentText() == "Openning Dilatation Model 8" ) {
			model = OITO;
		} else if (dialogPoresThroats->comboBoxModel->currentText() == "Openning Dilatation Model 7" ) {
			model = SETE;
		}
		CAberturaDilatacao3D filtro = CAberturaDilatacao3D(dialogPoresThroats->child->pm3D, indice, fundo );
		filtro.RaioMaximoElementoEstruturante(dialogPoresThroats->spinBoxRmax->value());
		filtro.RaioEEDilatacao(dialogPoresThroats->spinBoxRdilatation->value());
		filtro.FatorReducaoRaioElemEst(dialogPoresThroats->spinBoxRreduction->value());
		filtro.IncrementoRaioElementoEstruturante(dialogPoresThroats->spinBoxRinc->value());
		filtro.SalvarResultadosParciais(dialogPoresThroats->checkBoxSPR->isChecked());
		filtro.GerarDetalhesObjetos(dialogPoresThroats->checkBoxGOD->isChecked());
		filtro.Go(model);
		filtro.Write(filepath.toStdString());
		open( filepath.toStdString() );
		if (filtro.GerarDetalhesObjetos()) {
			filepath = dialogPoresThroats->child->getFilePath();
			filepath+= dialogPoresThroats->child->getFileNameNoExt() + ".rsl";
			filtro.SalvarListaObjetos(filepath.toStdString());
			open( filepath.toStdString() );
		}
	}

	QApplication::restoreOverrideCursor();

	dialogPoresThroats->close();
	delete dialogPoresThroats;
	dialogPoresThroats = nullptr;
}

void Lvp::rotate() {
	QString filepath;
	BaseImageViewer * child = nullptr;
	child = activeImageViewer();
	if ( child ) {
		filepath = child->getFullFileName();
	} else {
		QMessageBox::information(this, tr("LVP"), tr("Error trying to retrieve image!"));
		return;
	}
	if ( ! filepath.isNull() ) {
		if ( activePbmImageViewer() != 0 ){
			TCMatriz2D<bool> * pm = nullptr;
			pm = new TCMatriz2D<bool>(filepath.toStdString());
			if ( pm ) {
				QApplication::setOverrideCursor(Qt::WaitCursor);
				if ( pm->Rotacionar90 ( ) ) {
					static int seqNumberRotate = 1;
					filepath = tr(".rotated%1.%2").arg(QString::number(seqNumberRotate++)).arg(child->getFileExt());
					pm->Write(filepath.toStdString());
					filepath = child->getFilePath() + filepath;
					open( filepath.toStdString() );
					QApplication::restoreOverrideCursor();
				} else {
					QApplication::restoreOverrideCursor();
					QMessageBox::information(this, tr("LVP"), tr("Error trying to rotate the 2D image!"));
				}
				delete pm;
			} else {
				QMessageBox::information(this, tr("LVP"), tr("Error trying to create 2D image!"));
				return;
			}
		} else if ( activePgmImageViewer() != 0 ){
			TCMatriz2D<int> * pm = nullptr;
			pm = new TCMatriz2D<int>(filepath.toStdString());
			if ( pm ) {
				QApplication::setOverrideCursor(Qt::WaitCursor);
				if ( pm->Rotacionar90 ( ) ) {
					static int seqNumberRotate = 1;
					filepath = tr(".rotated%1.%2").arg(QString::number(seqNumberRotate++)).arg(child->getFileExt());
					pm->Write(filepath.toStdString());
					filepath = child->getFilePath() + filepath;
					open( filepath.toStdString() );
					QApplication::restoreOverrideCursor();
				} else {
					QApplication::restoreOverrideCursor();
					QMessageBox::information(this, tr("LVP"), tr("Error trying to rotate the 2D image!"));
				}
				delete pm;
			} else {
				QMessageBox::information(this, tr("LVP"), tr("Error trying to create 2D image!"));
				return;
			}
		} else if ( activeDbmImageViewer() != 0 ){
			TCMatriz3D<bool> * pm = nullptr;
			pm = new TCMatriz3D<bool>(filepath.toStdString());
			if ( pm ) {
				QMessageBox msgBox(this);
				msgBox.setWindowTitle(tr("LVP - Rotate"));
				msgBox.setText(tr("Rotate on:"));
				msgBox.addButton(QMessageBox::Cancel);
				QPushButton *xButton = msgBox.addButton(tr("&X axis"), QMessageBox::ActionRole);
				QPushButton *yButton = msgBox.addButton(tr("&Y axis"), QMessageBox::ActionRole);
				QPushButton *zButton = msgBox.addButton(tr("&Z axis"), QMessageBox::ActionRole);
				msgBox.setDefaultButton(xButton);
				msgBox.exec();

				E_eixo axis;
				if (msgBox.clickedButton() == xButton) {
					axis = EIXO_X;
				} else if (msgBox.clickedButton() == yButton) {
					axis = EIXO_Y;
				} else if (msgBox.clickedButton() == zButton) {
					axis = EIXO_Z;
				} else {
					delete pm;
					return;
				}

				QApplication::setOverrideCursor(Qt::WaitCursor);
				if ( pm->Rotacionar90 ( axis ) ) {
					static int seqNumberRotate = 1;
					filepath = tr(".rotated%1.%2").arg(QString::number(seqNumberRotate++)).arg(child->getFileExt());
					pm->Write(filepath.toStdString());
					filepath = child->getFilePath() + filepath;
					open( filepath.toStdString() );
					QApplication::restoreOverrideCursor();
				} else {
					QApplication::restoreOverrideCursor();
					QMessageBox::information(this, tr("LVP"), tr("Error trying to rotate the 3D image!"));
				}
				delete pm;
			} else {
				QMessageBox::information(this, tr("LVP"), tr("Error trying to create 3D image!"));
				return;
			}
		} else {
			QMessageBox::information(this, tr("LVP"), tr("Error: Image viewer not active!"));
			return;
		}
	} else if ( activeDgmImageViewer() != 0 ){
		TCMatriz3D<int> * pm = nullptr;
		pm = new TCMatriz3D<int>(filepath.toStdString());
		if ( pm ) {
			QMessageBox msgBox(this);
			msgBox.setWindowTitle(tr("LVP - Rotate"));
			msgBox.setText(tr("Rotate on:"));
			msgBox.addButton(QMessageBox::Cancel);
			QPushButton *xButton = msgBox.addButton(tr("&X axis"), QMessageBox::ActionRole);
			QPushButton *yButton = msgBox.addButton(tr("&Y axis"), QMessageBox::ActionRole);
			QPushButton *zButton = msgBox.addButton(tr("&Z axis"), QMessageBox::ActionRole);
			msgBox.setDefaultButton(xButton);
			msgBox.exec();

			E_eixo axis;
			if (msgBox.clickedButton() == xButton) {
				axis = EIXO_X;
			} else if (msgBox.clickedButton() == yButton) {
				axis = EIXO_Y;
			} else if (msgBox.clickedButton() == zButton) {
				axis = EIXO_Z;
			} else {
				delete pm;
				return;
			}

			QApplication::setOverrideCursor(Qt::WaitCursor);
			if ( pm->Rotacionar90 ( axis ) ) {
				static int seqNumberRotate = 1;
				filepath = tr(".rotated%1.%2").arg(QString::number(seqNumberRotate++)).arg(child->getFileExt());
				pm->Write(filepath.toStdString());
				filepath = child->getFilePath() + filepath;
				open( filepath.toStdString() );
				QApplication::restoreOverrideCursor();
			} else {
				QApplication::restoreOverrideCursor();
				QMessageBox::information(this, tr("LVP"), tr("Error trying to rotate the 3D image!"));
			}
			delete pm;
		} else {
			QMessageBox::information(this, tr("LVP"), tr("Error trying to create 3D image!"));
			return;
		}
	} else {
		QMessageBox::information(this, tr("LVP"), tr("Error retrieving image!"));
	}
}

void Lvp::import() {
	// lista com o nome dos arquivos que serão abertos
	QString file = QFileDialog::getOpenFileName(this, tr("Import RAW File"), lastOpenPath, tr("RAW Files (*.raw *.3fr *.ari *.arw *.srf *.sr2 *.bay *.crw *.kdc *.nrw *.orf *.rw2 *.rwz *.srw *.x3f)"));
	if ( ! file.isNull() ) {
		import(file);
	}
}

void Lvp::import(QString file){
	QApplication::setOverrideCursor(Qt::WaitCursor);
	if ( ! file.isNull() ) {
		if ( dialogImport ) {
			dialogImport->show();
		} else {
			dialogImport = new Import(this, file);
			if ( dialogImport ) {
				dialogImport->show();
			} else {
				QApplication::restoreOverrideCursor();
				QMessageBox::information(this, tr("LVP"), tr("Erro! - To create Import dialog!"));
			}
		}
		QDir lop( file );
		lastOpenPath = lop.canonicalPath();
	}
	QApplication::restoreOverrideCursor();
}

void Lvp::exImport() {
	QApplication::setOverrideCursor(Qt::WaitCursor);
	EImageType tipo;
	static int seqNumberImport = 1;
	QString qstr;
	switch (dialogImport->comboBoxImageType->currentIndex()) {
		case 0:
			tipo = D4_X_Y_Z_BINARY;
			qstr = tr(".imported%1.dbm").arg(QString::number(seqNumberImport++));
			break;
		case 1:
			tipo = D5_X_Y_Z_GRAY_BINARY;
			qstr = tr(".imported%1.dgm").arg(QString::number(seqNumberImport++));
			break;
		case 2:
			tipo = D6_X_Y_Z_COLOR_BINARY;
			qstr = tr(".imported%1.dpm").arg(QString::number(seqNumberImport++));
			break;
		default:
			tipo = D4_X_Y_Z_BINARY;
			qstr = tr(".imported%1.dbm").arg(QString::number(seqNumberImport++));
	}
	TCImagem3D<int> * pm3D = new TCImagem3D<int>( dialogImport->fullFileName.toStdString(), dialogImport->spinBoxWidth->value(), dialogImport->spinBoxHeight->value(), dialogImport->spinBoxNOI->value(), tipo );
	pm3D->Path(lastOpenPath.toStdString());
	dialogImport->close();
	if ( pm3D->Write(qstr.toStdString()) ) {
		qstr.prepend(lastOpenPath); //concatena lastOpenPath antes de qstr
		open(qstr.toStdString());
	} else {
		QMessageBox::information(this, tr("LVP"), tr("Erro! - To save imported image!"));
	}
	delete pm3D;
	delete dialogImport;
	dialogImport = nullptr;
	QApplication::restoreOverrideCursor();
}

void Lvp::crop3DImage(){
	BaseDnmImageViewer *mdiChild = active3DImageViewer();
	dialogCrop = new Crop3D(this);
	dialogCrop->spinBoxStartX->setMaximum(mdiChild->nx-1);
	dialogCrop->spinBoxStartY->setMaximum(mdiChild->ny-1);
	dialogCrop->spinBoxStartZ->setMaximum(mdiChild->nz-1);
	dialogCrop->spinBoxEndX->setMaximum(mdiChild->nx);
	dialogCrop->spinBoxEndY->setMaximum(mdiChild->ny);
	dialogCrop->spinBoxEndZ->setMaximum(mdiChild->nz);
	dialogCrop->spinBoxEndX->setValue(mdiChild->nx);
	dialogCrop->spinBoxEndY->setValue(mdiChild->ny);
	dialogCrop->spinBoxEndZ->setValue(mdiChild->nz);
	dialogCrop->show();
}

void Lvp::exCrop3DImage() {
	QApplication::setOverrideCursor(Qt::WaitCursor);
	static int seqNumberCrop3D = 1;
	string str;
	if ( DbmImageViewer *mdiChild = activeDbmImageViewer() ) {
		cerr << "aqui" << endl;
		TCMatriz3D<bool> * pm3D = mdiChild->pm3D->Crop(dialogCrop->spinBoxStartX->value(), dialogCrop->spinBoxEndX->value(), dialogCrop->spinBoxStartY->value(), dialogCrop->spinBoxEndY->value(), dialogCrop->spinBoxStartZ->value(), dialogCrop->spinBoxEndZ->value());

		str = tr(".croped%1.dbm").arg(QString::number(seqNumberCrop3D++)).toStdString();
		pm3D->Write( str );
		open( str );

		dialogCrop->close();
		delete dialogCrop;
		dialogCrop = nullptr;
		delete pm3D;
	} else if ( DgmImageViewer *mdiChild = activeDgmImageViewer() ) {
		TCMatriz3D<int> * pm3D = mdiChild->pm3D->Crop(dialogCrop->spinBoxStartX->value(), dialogCrop->spinBoxEndX->value(), dialogCrop->spinBoxStartY->value(), dialogCrop->spinBoxEndY->value(), dialogCrop->spinBoxStartZ->value(), dialogCrop->spinBoxEndZ->value());

		str = tr(".croped%1.dgm").arg(QString::number(seqNumberCrop3D++)).toStdString();
		pm3D->Write( str );
		open( str );

		dialogCrop->close();
		delete dialogCrop;
		dialogCrop = nullptr;
		delete pm3D;
	} else {
		cerr << "não entrou..." << endl;
	}
	QApplication::restoreOverrideCursor();
}

void Lvp::confEq() {
	BaseImageViewer *mdiChild = activeImageViewer();
	dialog = new ConfEq(this, mdiChild);
	dialog->show();
}

void Lvp::exConfEq() {
	QApplication::setOverrideCursor(Qt::WaitCursor);
	//	QList<QString> listqstr; //lista de imagens criadas.
	//	foreach (ImageViewer *mdiChild, selectedImagesList()) {
	if (BaseImageViewer *mdiChild = active2DImageViewer()) {
		TCMatriz2D<int> * pm2D = new TCMatriz2D<int>( mdiChild->getFullFileName().toStdString() );

		CConfiguracoesEquilibrio2D *confeq = nullptr;
		confeq = new CConfiguracoesEquilibrio2D ( );

		confeq->SalvarResultadosParciaisDisco(dialog->checkBoxSalvar->checkState() == 2); //checkState: 0 = unchecked, 1 = partially, 2 = checked
		confeq->fluidoA->Compressibilidade(dialog->radioButtonCompressivel->isChecked()); // 0 or false = incompressível, 1 or true = compressível
		confeq->fluidoB->Molhabilidade(dialog->radioButtonMolhante->isChecked()); // 0 or false = não molhante, 1 or true = molhante

		dialog->close();
		delete dialog;
		dialog = nullptr;
		confeq->Go( pm2D );
		delete confeq;
		delete pm2D;
	}
	/*foreach (QString qstr2, listqstr){
	string stdstr2 = qstr2.toStdString();
	statusBar()->showMessage(tr("%1 Ok!").arg(qstr2), 10000);
	open(stdstr2);
	}*/
	QApplication::restoreOverrideCursor();
}

void Lvp::exConfEq3D() {
	QApplication::setOverrideCursor(Qt::WaitCursor);
	if (BaseImageViewer *mdiChild = active3DImageViewer()) {
		CConfiguracoesEquilibrio3D *confeq = nullptr;
		confeq = new CConfiguracoesEquilibrio3D ( );
		confeq->SalvarResultadosParciaisDisco(dialog->checkBoxSalvar->checkState() == 2); 	//checkState: 0 = unchecked, 1 = partially, 2 = checked
		confeq->fluidoA->Compressibilidade( dialog->radioButtonCompressivel->isChecked() ); 	// 0 or false = incompressível, 1 or true = compressível
		confeq->fluidoB->Compressibilidade( ! dialog->radioButtonCompressivel->isChecked() );// o inverso da compressibilidade do fluidoA
		confeq->fluidoA->Molhabilidade( ! dialog->radioButtonMolhante->isChecked() ); 		// o inverso da molhabilidade do fluidoB
		confeq->fluidoB->Molhabilidade( dialog->radioButtonMolhante->isChecked() ); 		// 0 or false = não molhante, 1 or true = molhante
		dialog->close();
		delete dialog;
		dialog = nullptr;
		TCMatriz3D<int> * objM3D = new TCMatriz3D<int>( mdiChild->getFullFileName().toStdString() );
		confeq->Go( objM3D );
		QApplication::restoreOverrideCursor();
		if ( QMessageBox::Yes == QMessageBox::question( this, tr("LVP"), tr("Do you want invert the flow?"),  QMessageBox::Yes | QMessageBox::No ) ) {
			QApplication::setOverrideCursor(Qt::WaitCursor);
			confeq->InverterFluxo( objM3D );
			confeq->Go( objM3D );
		}
		delete confeq;
	}
	QApplication::restoreOverrideCursor();
}

void Lvp::exChangePlanX( int _plan ) {
	if (GLWidget *mdiChild = activeGLWidget()) {
		mdiChild->setPlanX( _plan );
	}
}

void Lvp::exChangePlanY( int _plan ) {
	if (GLWidget *mdiChild = activeGLWidget()) {
		mdiChild->setPlanY( _plan );
	}
}

void Lvp::exChangePlanZ( int _plan ) {
	if (GLWidget *mdiChild = activeGLWidget()) {
		mdiChild->setPlanZ( _plan );
	}
}

void Lvp::exChangePlan( int _plan ) {
	if (DbmImageViewer *mdiChild = activeDbmImageViewer()) {
		E_eixo axis= EIXO_X;
		if (radioButtonY->isChecked())
			axis = EIXO_Y;
		else if (radioButtonZ->isChecked())
			axis = EIXO_Z;
		if ( ! mdiChild->ChangePlan( _plan, axis ) )
			cerr << "erro: exChangePlan" << endl;
	} else if (DgmImageViewer *mdiChild = activeDgmImageViewer()) {
		E_eixo axis= EIXO_X;
		if (radioButtonY->isChecked())
			axis = EIXO_Y;
		else if (radioButtonZ->isChecked())
			axis = EIXO_Z;
		if ( ! mdiChild->ChangePlan( _plan, axis ) )
			cerr << "erro: exChangePlan" << endl;
	}
}

void Lvp::exChangeAxis( ) {
	if ( DbmImageViewer *mdiChild = activeDbmImageViewer() ) {
		E_eixo axis = EIXO_X;
		if (radioButtonY->isChecked())
			axis = EIXO_Y;
		else if (radioButtonZ->isChecked())
			axis = EIXO_Z;

		if ( mdiChild->ChangePlan( spinBoxPlano3D->value(), axis ) ) {
			updateMenus();
		} else {
			cerr << "erro: exChangePlanAxis" << endl;
		}
	} else 	if ( DgmImageViewer *mdiChild = activeDgmImageViewer() ) {
		E_eixo axis = EIXO_X;
		if (radioButtonY->isChecked())
			axis = EIXO_Y;
		else if (radioButtonZ->isChecked())
			axis = EIXO_Z;

		if ( mdiChild->ChangePlan( spinBoxPlano3D->value(), axis ) ) {
			updateMenus();
		} else {
			cerr << "erro: exChangePlanAxis" << endl;
		}
	}
}

void Lvp::correlationFFT3D () {
	correlation3D (CCorrelacao3D::Frequencial);
}

void Lvp::correlationSpatial3D () {
	correlation3D (CCorrelacao3D::Espacial);
}

void Lvp::correlation3D ( CCorrelacao3D::Tipos tipo ){
	int indice;
	bool ok;
	QList<DbmImageViewer *> imagesList = selectedDbmImagesList(); //lista de ponteiros para imagens selecionadas.

	QMessageBox msgBox(this);
	msgBox.setWindowTitle(tr("LVP - 3D Correlation"));
	msgBox.setText(tr("Pore is:"));
	msgBox.addButton(QMessageBox::Cancel);
	QPushButton *writeButton = msgBox.addButton(tr("&Write (0)"), QMessageBox::ActionRole);
	QPushButton *blackButton = msgBox.addButton(tr("&Black (1)"), QMessageBox::ActionRole);
	msgBox.setDefaultButton(blackButton);
	msgBox.exec();

	if (msgBox.clickedButton() == blackButton) {
		indice = 1;
	} else if (msgBox.clickedButton() == writeButton) {
		indice = 0;
	} else {
		return;
	}
	int numPlanos = QInputDialog::getInt(this, tr(":. 3D Correlation"), tr("Enter the number of plans to be considered.\nLeave 0 to consider 10% of plans of the image:"), 0, 0, 300, 1, &ok);
	if ( ok ){
		int numFiles = imagesList.size(); //número de arquivos que serão abertos
		int cont = 0;
		QProgressDialog progress("Calculating 3D correlation...", "&Cancel", 0, numFiles, this);
		progress.setWindowModality(Qt::WindowModal);
		QApplication::setOverrideCursor(Qt::WaitCursor);
		CCorrelacao3D *cor = nullptr;
		foreach (DbmImageViewer *mdiChild, imagesList) {
			progress.setValue(cont);
			if (progress.wasCanceled())
				break;
			if ( mdiChild->pm3D == nullptr ) {
				mdiChild->pm3D = new TCImagem3D<bool>( mdiChild->getFullFileName().toStdString() );
			}
			if ( mdiChild->pm3D == nullptr ) {
				cerr << "Não foi possível alocar TCMatriz3D em Lvp::correlation3D()";
				QApplication::restoreOverrideCursor();
				return;
			}
			cor = new CCorrelacao3D( );
			if ( ! cor ) {
				cerr << "Não foi possível alocar CCorrelacao3D em Lvp::correlation3D()";
				QApplication::restoreOverrideCursor();
				return;
			}
			if ( cor->Go( mdiChild->pm3D, tipo, indice, numPlanos ) ) { // Executa funcao autocorrelacao Go.
				string stdstr = mdiChild->getFileName().toStdString();
				switch (tipo) {
					case CCorrelacao3D::Espacial:
						stdstr = "." + stdstr + "-Spatial-3D";
						break;
					case CCorrelacao3D::Frequencial:
						stdstr = "." + stdstr + "-FFT-3D";
						break;
				}
				cor->Write(stdstr);
				stdstr += ".cor";
				open(stdstr);
			} else {
				QMessageBox::information(this, tr("LVP"), tr("Erro! - FFT Correlation class returned false!"));
			}
			delete cor;
			cor = nullptr;
			cont++;
		}
		progress.setValue(numFiles);
		QApplication::restoreOverrideCursor();
	}
}

void Lvp::correlationFFT() {
	int indice;
	QList<PbmImageViewer *> imagesList = selectedPbmImagesList(); //lista de ponteiros para imagens selecionadas.
	QMessageBox msgBox(this);
	msgBox.setWindowTitle(tr("LVP - Correlation FFT"));
	msgBox.setText(tr("Pore is:"));
	msgBox.addButton(QMessageBox::Cancel);
	QPushButton *writeButton = msgBox.addButton(tr("&Write (0)"), QMessageBox::ActionRole);
	QPushButton *blackButton = msgBox.addButton(tr("&Black (1)"), QMessageBox::ActionRole);
	msgBox.setDefaultButton(blackButton);
	msgBox.exec();

	if (msgBox.clickedButton() == blackButton) {
		indice = 1;
	} else if (msgBox.clickedButton() == writeButton) {
		indice = 0;
	} else {
		return;
	}
	int numFiles = imagesList.size(); //número de arquivos que serão abertos
	int cont = 0;
	QProgressDialog progress("Calculating FFT correlation...", "&Cancel", 0, numFiles, this);
	progress.setWindowModality(Qt::WindowModal);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	CCorrelacaoFFT *cor = nullptr;
	foreach (PbmImageViewer *mdiChild, imagesList) {
		progress.setValue(cont);
		if (progress.wasCanceled())
			break;
		if ( mdiChild->pm == nullptr ) {
			string stdstr = mdiChild->getFullFileName().toStdString();	// converte nome do arquivo para string std.
			mdiChild->pm = new TCMatriz2D<bool>(stdstr);
		}
		if ( mdiChild->pm == nullptr ) {
			cerr << "Não foi possível alocar TCMatriz2D<int> em Lvp::correlationFFT()";
			QApplication::restoreOverrideCursor();
			return;
		}
		cor = new CCorrelacaoFFT ();
		if ( ! cor ) {
			cerr << "Não foi possível alocar CCorrelacaoFFT em Lvp::correlationFFT()";
			QApplication::restoreOverrideCursor();
			return;
		}
		if ( cor->Go( mdiChild->pm, indice ) ) { // Executa funcao autocorrelacao Go.
			string stdstr = mdiChild->getFileName().toStdString();
			stdstr = "." + stdstr + "-FFT";
			cor->Write(stdstr);
			stdstr += ".cor";
			open(stdstr);
		} else {
			QMessageBox::information(this, tr("LVP"), tr("Erro! - FFT Correlation class returned false!"));
		}
		delete cor;
		cor = nullptr;
		cont++;
	}
	progress.setValue(numFiles);
	QApplication::restoreOverrideCursor();
}

void Lvp::correlationSpatial() {
	int indice;
	QList<PbmImageViewer *> imagesList = selectedPbmImagesList(); //lista de ponteiros para imagens selecionadas.

	QMessageBox msgBox(this);
	msgBox.setWindowTitle(tr("LVP - Correlation Spatial"));
	msgBox.setText(tr("Pore is:"));
	msgBox.addButton(QMessageBox::Cancel);
	QPushButton *writeButton = msgBox.addButton(tr("&Write (0)"), QMessageBox::ActionRole);
	QPushButton *blackButton = msgBox.addButton(tr("&Black (1)"), QMessageBox::ActionRole);
	msgBox.setDefaultButton(blackButton);
	msgBox.exec();

	if (msgBox.clickedButton() == blackButton) {
		indice = 1;
	} else if (msgBox.clickedButton() == writeButton) {
		indice = 0;
	} else {
		return;
	}

	int numFiles = imagesList.size(); //número de arquivos que serão abertos
	int cont = 0;
	QProgressDialog progress("Calculating FFT correlation...", "&Cancel", 0, numFiles, this);
	progress.setWindowModality(Qt::WindowModal);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	CCorrelacaoEspacial *cor = nullptr;
	foreach (PbmImageViewer *mdiChild, imagesList) {
		progress.setValue(cont);
		if (progress.wasCanceled())
			break;
		if ( mdiChild->pm == nullptr ) {
			string stdstr = mdiChild->getFullFileName().toStdString();	// converte nome do arquivo para string std.
			mdiChild->pm = new TCMatriz2D<bool>(stdstr);
		}
		if ( mdiChild->pm == nullptr ) {
			cerr << "Não foi possível alocar TCMatriz2D<int> em Lvp::correlationSpatial()";
			QApplication::restoreOverrideCursor();
			return;
		}
		cor = new CCorrelacaoEspacial ();
		if ( ! cor ) {
			cerr << "Não foi possível alocar CCorrelacaoFFT em Lvp::correlationSpatial()";
			QApplication::restoreOverrideCursor();
			return;
		}
		if ( cor->Go ( mdiChild->pm, indice ) ) {// Executa funcao autocorrelacao
			string stdstr = mdiChild->getFileName().toStdString();
			stdstr = "." + stdstr + "-Spatial";
			cor->Write(stdstr);
			stdstr += ".cor";
			open(stdstr);
		} else {
			QApplication::restoreOverrideCursor();
			QMessageBox::information(this, tr("LVP"), tr("Erro! - Spatial Correlation class returned false!"));
		}
		delete cor;
		cor = nullptr;
		cont++;
	}
	progress.setValue(numFiles);
	QApplication::restoreOverrideCursor();
}

void Lvp::dtpSpatial () {
	distribution( CBaseDistribuicao::dtp, m2DSpatial );
}

void Lvp::dtsSpatial () {
	distribution( CBaseDistribuicao::dts, m2DSpatial );
}

void Lvp::dtpD34 () {
	distribution( CBaseDistribuicao::dtp, m2Dd34 );
}

void Lvp::dtsD34 () {
	distribution( CBaseDistribuicao::dts, m2Dd34 );
}

void Lvp::dtpD5711 () {
	distribution( CBaseDistribuicao::dtp, m2Dd5711 );
}

void Lvp::dtsD5711 () {
	distribution( CBaseDistribuicao::dts, m2Dd5711 );
}

void Lvp::dtpEuclidian () {
	distribution( CBaseDistribuicao::dtp, m2DEuclidian );
}

void Lvp::dtsEuclidian () {
	distribution( CBaseDistribuicao::dts, m2DEuclidian );
}

void Lvp::distribution (CBaseDistribuicao::Tipos tipo, Metrics2D m2d) {
	QList<PbmImageViewer *> imagesList = selectedPbmImagesList(); //lista de ponteiros para imagens selecionadas.
	if ( imagesList.isEmpty() )
		return;
	string stdstr;
	string ext;
	string sufixo;
	CDistribuicao *dist = nullptr;

	QMessageBox msgBox(this);
	msgBox.addButton(QMessageBox::Cancel);
	QPushButton *writeButton = msgBox.addButton(tr("&Write (0)"), QMessageBox::ActionRole);
	QPushButton *blackButton = msgBox.addButton(tr("&Black (1)"), QMessageBox::ActionRole);

	if (tipo == CBaseDistribuicao::dts) {
		ext = ".dts";
		msgBox.setWindowTitle(tr("LVP - Solids Size Distribution"));
		msgBox.setText(tr("Solid is:"));
		msgBox.setDefaultButton(writeButton);
	} else {
		ext = ".dtp";
		msgBox.setWindowTitle(tr("LVP - Pores Size Distribution"));
		msgBox.setText(tr("Pore is:"));
		msgBox.setDefaultButton(blackButton);
	}
	msgBox.exec();

	int indice, fundo;
	if (msgBox.clickedButton() == blackButton) {
		indice = 1; fundo = 0;
	} else if (msgBox.clickedButton() == writeButton) {
		indice = 0; fundo = 1;
	} else {
		return;
	}
	CDistribuicao::Metrica metrica = CDistribuicao::d34; //default
	switch ( m2d ) {
		case m2DSpatial: // Espacial
			metrica = CDistribuicao::espacial;
			sufixo = "spatial";
			break;
		case m2Dd34: //d34
			metrica = CDistribuicao::d34;
			sufixo = "d34";
			break;
		case m2Dd5711: //d5711
			metrica = CDistribuicao::d5711;
			sufixo = "d5711";
			break;
		case m2DEuclidian: //euclidiana
			metrica = CDistribuicao::euclidiana;
			sufixo = "euclidian";
			break;
	}
	int numFiles = imagesList.size(); //número de arquivos que serão abertos
	int cont = 0;
	QProgressDialog progress("Calculating distribution...", "&Cancel", 0, numFiles, this);
	progress.setWindowModality(Qt::WindowModal);
	foreach (PbmImageViewer *mdiChild, imagesList) {
		progress.setValue(cont);
		if (progress.wasCanceled())
			break;
		if ( ! mdiChild->pm ) {
			mdiChild->pm = new TCMatriz2D<bool>( mdiChild->getFullFileName().toStdString() );
		}
		if ( ! mdiChild->pm ) {
			cerr << "Não foi possível alocar TCMatriz2D<int> em Lvp::distribution()" << endl;
			QApplication::restoreOverrideCursor();
			return;
		}
		dist = new CDistribuicao ( mdiChild->pm );
		if ( ! dist ) {
			cerr << "Não foi possível alocar CDistribuicao em Lvp::distribution()" << endl;
			QApplication::restoreOverrideCursor();
			return;
		}
		// Executa funcao distribuição
		if ( dist->Go( tipo, metrica, indice, fundo) ) {
			stdstr = tr(".%1-%2").arg(mdiChild->getFileNameNoExt(), QString::fromStdString(sufixo)).toStdString ();
			stdstr = validateFileName(stdstr + ext);
			if ( dist->Write(stdstr.substr(0, stdstr.size()-4)) ) {
				open( stdstr );
			} else {
				QMessageBox::information(this, tr("LVP"), tr("Erro! - Can not save the file \"%1\"!").arg(QString::fromStdString(stdstr)));
			}
		} else {
			QApplication::restoreOverrideCursor();
			QMessageBox::information(this, tr("LVP"), tr("Erro! - Distribution class returned false!"));
			delete dist;
			return;
		}
		delete dist;
		dist = nullptr;
		cont++;
	}
	progress.setValue(numFiles);
	QApplication::restoreOverrideCursor();
}

void Lvp::dtpSpatial3D () {
	distribution3D ( CBaseDistribuicao::dtp, m3DSpatial );
}

void Lvp::dtsSpatial3D () {
	distribution3D ( CBaseDistribuicao::dts, m3DSpatial );
}

void Lvp::dtpD34_3D() {
	distribution3D ( CBaseDistribuicao::dtp, m3Dd34 );
}

void Lvp::dtsD34_3D() {
	distribution3D ( CBaseDistribuicao::dts, m3Dd34 );
}

void Lvp::dtpD345 () {
	distribution3D ( CBaseDistribuicao::dtp, m3Dd345 );
}

void Lvp::dtsD345 () {
	distribution3D ( CBaseDistribuicao::dts, m3Dd345 );
}

void Lvp::dtpD5711_3D() {
	distribution3D ( CBaseDistribuicao::dtp, m3Dd5711 );
}

void Lvp::dtsD5711_3D() {
	distribution3D ( CBaseDistribuicao::dts, m3Dd5711 );
}

void Lvp::dtpEuclidian3D () {
	distribution3D ( CBaseDistribuicao::dtp, m3DEuclidian );
}

void Lvp::dtsEuclidian3D () {
	distribution3D ( CBaseDistribuicao::dts, m3DEuclidian );
}

void Lvp::distribution3D (CBaseDistribuicao::Tipos tipo, Metrics3D m3d) {
	//tipo=CBaseDistribuicao::dts; m3d=m3DSpatial; //evitar warming. apagar depois...
	//QMessageBox::information(this, tr("LVP"), tr("Funcionalidade não implementada!"));
	QList<DbmImageViewer *> imagesList = selectedDbmImagesList (); //lista de ponteiros para imagens selecionadas.
	if ( imagesList.isEmpty() )
		return;
	string stdstr;
	string ext;
	string sufixo;
	CDistribuicao3D *dist = nullptr;

	QMessageBox msgBox(this);
	msgBox.addButton(QMessageBox::Cancel);
	QPushButton *writeButton = msgBox.addButton(tr("&Write (0)"), QMessageBox::ActionRole);
	QPushButton *blackButton = msgBox.addButton(tr("&Black (1)"), QMessageBox::ActionRole);
	msgBox.setDefaultButton(blackButton);

	bool ok;
	int numPlanos = 0;

	if (tipo == CBaseDistribuicao::dts) {
		ext = ".dts";
		msgBox.setWindowTitle(tr("LVP - 3D Solids Size Distribution"));
		msgBox.setText(tr("Solid is:"));
	} else {
		ext = ".dtp";
		msgBox.setWindowTitle(tr("LVP - 3D Pores Size Distribution"));
		msgBox.setText(tr("Pore is:"));
	}
	msgBox.exec();

	int indice, fundo;
	if (msgBox.clickedButton() == blackButton) {
		indice = 1; fundo = 0;
	} else if (msgBox.clickedButton() == writeButton) {
		indice = 0; fundo = 1;
	} else {
		return;
	}

	CDistribuicao3D::Metrica3D metrica = CDistribuicao3D::d345;
	CDistribuicao::Metrica metrica2D = CDistribuicao::d34;
	stringstream suf;
	switch ( m3d ) {
		case m3DSpatial: // Espacial 3D
			metrica = CDistribuicao3D::espacial;
			sufixo = "spatial3D";
			break;
		case m3Dd34: //d34
			metrica2D = CDistribuicao::d34;
			numPlanos = QInputDialog::getInt(this, tr(":. 3D Distribution"), tr("Enter the number of plans to be considered.\nLeave 0 to consider 10% of plans of the image:"), 0, 0, 1000, 1, &ok);
			if ( ! ok ) return;
			suf << "d34_np-" << numPlanos;
			sufixo = suf.str();
			break;
		case m3Dd345: //d345
			metrica = CDistribuicao3D::d345;
			sufixo = "d345";
			break;
		case m3Dd5711: //d5711
			metrica2D = CDistribuicao::d5711;
			numPlanos = QInputDialog::getInt(this, tr(":. 3D Distribution"), tr("Enter the number of plans to be considered.\nLeave 0 to consider 10% of plans of the image:"), 0, 0, 1000, 1, &ok);
			if ( ! ok ) return;
			suf << "d5711_np-" << numPlanos;
			sufixo = suf.str();
			break;
		case m3DEuclidian: //euclidiana 3D
			metrica = CDistribuicao3D::euclidiana;
			sufixo = "euclidian3D";
			break;
	}
	int numFiles = imagesList.size(); //número de arquivos que serão abertos
	int cont = 0;
	QProgressDialog progress("Calculating 3D distribution...", "&Cancel", 0, numFiles, this);
	progress.setWindowModality(Qt::WindowModal);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	foreach (DbmImageViewer *mdiChild, imagesList) {
		progress.setValue(cont);
		if (progress.wasCanceled())
			break;
		if ( ! mdiChild->pm3D ) {
			mdiChild->pm3D = new TCImagem3D<bool>( mdiChild->getFullFileName().toStdString() );
		}
		if ( ! mdiChild->pm3D ) {
			cerr << "Não foi possível alocar TCMatriz3D em Lvp::distribution3D()" << endl;
			QApplication::restoreOverrideCursor();
			return;
		}
		dist = new CDistribuicao3D ( mdiChild->pm3D );
		if ( ! dist ) {
			cerr << "Não foi possível alocar CDistribuicao3D em Lvp::distribution3D()" << endl;
			QApplication::restoreOverrideCursor();
			return;
		}
		// Executa função distribuição
		if (m3d == m3Dd34 || m3d == m3Dd5711) {
			ok = dist->Go( tipo, metrica2D, indice, fundo, numPlanos);
		} else {
			ok = dist->Go( tipo, metrica, indice, fundo);
		}
		if ( ok ) {
			stdstr = tr(".%1_%2").arg(mdiChild->getFileNameNoExt(), QString::fromStdString(sufixo)).toStdString ();
			stdstr = validateFileName(stdstr + ext);
			if ( dist->Write(stdstr.substr(0, stdstr.size()-4)) ) {
				open( stdstr );
			} else {
				QMessageBox::information(this, tr("LVP"), tr("Erro! - Can not save the file \"%1\"!").arg(QString::fromStdString(stdstr)));
			}
		} else {
			QApplication::restoreOverrideCursor();
			QMessageBox::information(this, tr("LVP"), tr("Erro! - Distribution3D class returned false!"));
			delete dist;
			return;
		}
		delete dist;
		dist = nullptr;
		cont++;
	}
	progress.setValue(numFiles);
	QApplication::restoreOverrideCursor();
}

void Lvp::dtpgD345_3D(){
	bool ok1, ok2, ok3, ok4;
	std::pair<CDistribuicao3D *, CDistribuicao3D *> dist;
	DbmImageViewer * child = activeDbmImageViewer();
	CDistribuicaoTamanhoPorosGargantas * filtro = nullptr;
	if ( child != nullptr ){
		if ( child->pm3D ) {
			int indice=1;
			int fundo=0;
			QMessageBox msgBox(this);
			msgBox.setWindowTitle(tr("LVP - 3D pores and throats size distribution"));
			msgBox.setText(tr("Pore is:"));
			msgBox.addButton(QMessageBox::Cancel);
			QPushButton *writeButton = msgBox.addButton(tr("&Write (0)"), QMessageBox::ActionRole);
			QPushButton *blackButton = msgBox.addButton(tr("&Black (1)"), QMessageBox::ActionRole);
			msgBox.setDefaultButton(blackButton);
			msgBox.exec();
			if (msgBox.clickedButton() == blackButton) {
				indice = 1;
				fundo = 0;
			} else if (msgBox.clickedButton() == writeButton) {
				indice = 0;
				fundo = 1;
			} else {
				return;
			}
			int raioMaximo = QInputDialog::getInt(this, tr(":. Segmentation"), tr("Enter the maximum radius of the structuring element:"), 50, 1, 99, 1, &ok1);
			int raioDilatacao = QInputDialog::getInt(this, tr(":. Segmentation"), tr("Enter the radius for the dilatation:"), 1, 1, 10, 1, &ok2);
			int fatorReducao = QInputDialog::getInt(this, tr(":. Segmentation"), tr("Enter the reduction factor of the structuring element radius:"), 1, 1, raioMaximo, 1, &ok3);
			int incrementoRaio = QInputDialog::getInt(this, tr(":. Segmentation"), tr("Enter the increment value for the structuring element radius:"), 1, 1, raioMaximo, 1, &ok4);
			if (ok1 and ok2 and ok3 and ok4) {
				QApplication::setOverrideCursor(Qt::WaitCursor);
				EModelo modelo = ONZE;
				filtro = new CDistribuicaoTamanhoPorosGargantas(child->pm3D, raioMaximo, raioDilatacao, fatorReducao, incrementoRaio, modelo, indice, fundo );
				dist = filtro->Go(CDistribuicao3D::d345);
				QApplication::restoreOverrideCursor();
			}
		} else {
			QMessageBox::information(this, tr("LVP"), tr("Error: 3D image is nullptr!"));
			return;
		}
	} else {
		DgmImageViewer* childg = activeDgmImageViewer();
		if ( childg != nullptr ) {
			if (childg->pm3D != nullptr) {
				QApplication::setOverrideCursor(Qt::WaitCursor);
				filtro = new CDistribuicaoTamanhoPorosGargantas( childg->pm3D );
				dist = filtro->Go(CDistribuicao3D::d345);
				QApplication::restoreOverrideCursor();
			} else {
				QMessageBox::information(this, tr("LVP"), tr("Error: 3D image is nullptr!"));
				return;
			}
		}else {
			QMessageBox::information(this, tr("LVP"), tr("Error: Image viewer not active!"));
			return;
		}
	}
	if (dist.first != nullptr && dist.second != nullptr) {
		static int seqNumberDTPG = 0;
		seqNumberDTPG++;
		QString filepath = tr(".distribution%1").arg(QString::number(seqNumberDTPG));
		filepath = validateFileName(filepath);
		ok1 = dist.first->Write(filepath.toStdString());
		ok2 = dist.second->Write(filepath.toStdString());
		if ( ok1 and ok2 ) {
			open( (filepath + ".dtp").toStdString() );
			open( (filepath + ".dtg").toStdString() );
		} else {
			QMessageBox::information(this, tr("LVP"), tr("Erro! - Can not save the distribution file!"));
		}
	} else {
		QMessageBox::information(this, tr("LVP"), tr("Erro! - CDistribuicaoTamanhoPorosGargantas class returned nullptr!"));
	}
	if(filtro)
		delete filtro;
	if(dist.first)
		delete dist.first;
	if(dist.second)
		delete dist.second;
	QApplication::restoreOverrideCursor();
}

void Lvp::porosity() {
	double porosityValue = 0.0;
	double sumPorority = 0.0;
	int cont = 0;
	int poro, indice;
	QString msg = "";
	QString msgTmp = "";

	QMessageBox msgBox(this);
	msgBox.setWindowTitle(tr("LVP - Porosity"));
	msgBox.setText(tr("Pore is:"));
	msgBox.addButton(QMessageBox::Cancel);
	QPushButton *writeButton = msgBox.addButton(tr("&Write (0)"), QMessageBox::ActionRole);
	QPushButton *blackButton = msgBox.addButton(tr("&Black (1)"), QMessageBox::ActionRole);
	msgBox.setDefaultButton(blackButton);

	if ( activePbmImageViewer() != 0 ) { // imagem 2D
		QList<PbmImageViewer *> imagesList = selectedPbmImagesList(); //lista de ponteiros para imagens selecionadas.
		msgBox.exec();
		if (msgBox.clickedButton() == blackButton) {
			indice = 1;
		} else if (msgBox.clickedButton() == writeButton) {
			indice = 0;
		} else {
			return;
		}
		foreach (PbmImageViewer *mdiChild, imagesList) {
			QString qstr = mdiChild->getFullFileName();  // busca nome completo do arquivo.
			if ( mdiChild->pm == nullptr )
				mdiChild->pm = new TCMatriz2D<bool>(qstr.toStdString());
			poro = 0;
			int nx = mdiChild->pm->NX();
			int ny = mdiChild->pm->NY();
			for (int x = 0; x <  nx; x++)
				for (int y = 0; y < ny; y++)
					if ( mdiChild->pm->data2D[x][y] == indice )
						poro++;
			porosityValue = ( ( double(poro) / double(nx * ny) ) * 100 );
			sumPorority += porosityValue;
			cont ++;
			msgTmp = tr("Porosity of the image \"%1\"\t= %2\%\n").arg(mdiChild->getFileName()).arg(porosityValue);
			msg += msgTmp;
		}
	} else if ( activeDbmImageViewer() != 0 ) { // imagem 3D
		QList<DbmImageViewer *> imagesList = selectedDbmImagesList(); //lista de ponteiros para imagens 3D selecionadas.
		msgBox.exec();
		if (msgBox.clickedButton() == blackButton) {
			indice = 1;
		} else if (msgBox.clickedButton() == writeButton) {
			indice = 0;
		} else {
			return;
		}
		foreach (DbmImageViewer *mdiChild, imagesList) {
			QString qstr = mdiChild->getFullFileName();  // busca nome completo do arquivo.
			if ( mdiChild->pm3D == nullptr )
				mdiChild->pm3D = new TCImagem3D<bool>(qstr.toStdString());
			poro = 0;
			int nx = mdiChild->pm3D->NX();
			int ny = mdiChild->pm3D->NY();
			int nz = mdiChild->pm3D->NZ();
			for (int x = 0; x <  nx; x++)
				for (int y = 0; y < ny; y++)
					for (int z = 0; z < nz; z++)
						if ( mdiChild->pm3D->data3D[x][y][z] == indice )
							poro++;
			porosityValue = ( ( double(poro) / double(nx * ny * nz) ) * 100 );
			sumPorority += porosityValue;
			cont ++;
			msgTmp = tr("Porosity of the image \"%1\"\t= %2\%\n").arg(mdiChild->getFileName()).arg(porosityValue);
			msg += msgTmp;
		}
		if ( cont > 1 ) {
			msgTmp = tr("\nAverage = %1\%\n").arg( sumPorority / cont );
			msg += msgTmp;
		}
		QMessageBox msgPorosity;
		msgPorosity.setText( msg );
		msgPorosity.setIcon(QMessageBox::Information);
		msgPorosity.exec();
	}
}

// Retorna a lista de imagens selecionadas em Images List
QList<BasePnmImageViewer *> Lvp::selected2DImagesList() {
	QList<BasePnmImageViewer *> list; //lista de imagens selecionadas.
	BasePnmImageViewer *mdiChild;
	foreach (QMdiSubWindow *window, mdiArea->subWindowList()) {
		if ( qobject_cast<BasePnmImageViewer *>(window->widget()) != 0 ) {
			mdiChild = qobject_cast<BasePnmImageViewer *>(window->widget());
			if (mdiChild->item->isSelected()) {
				list.append(mdiChild);
			}
		}
	}
	return list;
}

// Retorna a lista de imagens pbm selecionadas em Images List
QList<PbmImageViewer *> Lvp::selectedPbmImagesList() {
	QList<PbmImageViewer *> list; //lista de imagens selecionadas.
	PbmImageViewer *mdiChild;
	foreach (QMdiSubWindow *window, mdiArea->subWindowList()) {
		if ( qobject_cast<PbmImageViewer *>(window->widget()) != 0 ) {
			mdiChild = qobject_cast<PbmImageViewer *>(window->widget());
			if (mdiChild->item->isSelected()) {
				list.append(mdiChild);
			}
		}
	}
	return list;
}

// Retorna a lista de imagens pgm selecionadas em Images List
QList<PgmImageViewer *> Lvp::selectedPgmImagesList() {
	QList<PgmImageViewer *> list; //lista de imagens selecionadas.
	PgmImageViewer *mdiChild;
	foreach (QMdiSubWindow *window, mdiArea->subWindowList()) {
		if ( qobject_cast<PgmImageViewer *>(window->widget()) != 0 ) {
			mdiChild = qobject_cast<PgmImageViewer *>(window->widget());
			if (mdiChild->item->isSelected()) {
				list.append(mdiChild);
			}
		}
	}
	return list;
}

// Retorna a lista de imagens dbm selecionadas em Images List
QList<DbmImageViewer *> Lvp::selectedDbmImagesList() {
	QList<DbmImageViewer *> list; //lista de imagens selecionadas.
	DbmImageViewer *mdiChild;
	foreach (QMdiSubWindow *window, mdiArea->subWindowList()) {
		if ( qobject_cast<DbmImageViewer *>(window->widget()) != 0 ) {
			mdiChild = qobject_cast<DbmImageViewer *>(window->widget());
			if (mdiChild->item->isSelected()) {
				list.append(mdiChild);
			}
		}
	}
	return list;
}

// Retorna a lista de imagens dgm selecionadas em Images List
QList<DgmImageViewer *> Lvp::selectedDgmImagesList() {
	QList<DgmImageViewer *> list; //lista de imagens selecionadas.
	DgmImageViewer *mdiChild;
	foreach (QMdiSubWindow *window, mdiArea->subWindowList()) {
		if ( qobject_cast<DgmImageViewer *>(window->widget()) != 0 ) {
			mdiChild = qobject_cast<DgmImageViewer *>(window->widget());
			if (mdiChild->item->isSelected()) {
				list.append(mdiChild);
			}
		}
	}
	return list;
}


// Retorna a lista de imagens selecionadas em 3D Images List
QList<BaseDnmImageViewer *> Lvp::selected3DImagesList() {
	QList<BaseDnmImageViewer *> list; //lista de imagens selecionadas.
	BaseDnmImageViewer *mdiChild;
	foreach (QMdiSubWindow *window, mdiArea->subWindowList()) {
		if ( qobject_cast<BaseDnmImageViewer *>(window->widget()) != 0 ) {
			mdiChild = qobject_cast<BaseDnmImageViewer *>(window->widget());
			if (mdiChild->item->isSelected()) {
				list.append(mdiChild);
			}
		}
	}
	return list;
}

// Retorna a lista graficos selecionadas em Charts List
QList<Ploter *> Lvp::selectedPloterList() {
	QList<Ploter *> list; //lista de imagens selecionadas.
	Ploter *mdiChild;
	foreach (QMdiSubWindow *window, mdiArea->subWindowList()) {
		if ( qobject_cast<Ploter *>(window->widget()) != 0 ) {
			mdiChild = qobject_cast<Ploter *>(window->widget());
			if (mdiChild->item->isSelected()) {
				list.append(mdiChild);
			}
		}
	}
	return list;
}

// Retorna a lista de imagens selecionadas em todas Images List
QList<BaseImageViewer *> Lvp::selectedAllImagesList() {
	QList<BaseImageViewer *> list; //lista de imagens selecionadas.
	BaseImageViewer *mdiChild;
	foreach (QMdiSubWindow *window, mdiArea->subWindowList()) {
		if ( qobject_cast<PbmImageViewer *>(window->widget()) != 0 ) {
			mdiChild = qobject_cast<PbmImageViewer *>(window->widget());
			if (mdiChild->item->isSelected()) {
				list.append(mdiChild);
			}
		} else if ( qobject_cast<PgmImageViewer *>(window->widget()) != 0 ) {
			mdiChild = qobject_cast<PgmImageViewer *>(window->widget());
			if (mdiChild->item->isSelected()) {
				list.append(mdiChild);
			}
		} else if ( qobject_cast<DbmImageViewer *>(window->widget()) != 0 ) {
			mdiChild = qobject_cast<DbmImageViewer *>(window->widget());
			if (mdiChild->item->isSelected()) {
				list.append(mdiChild);
			}
		}
	}
	return list;
}

BaseImageViewer * Lvp::activeImageViewer() {
	if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow()) {
		if ( qobject_cast<DbmImageViewer *>(activeSubWindow->widget()) != 0 )
			return qobject_cast<DbmImageViewer *>(activeSubWindow->widget());
		else if ( qobject_cast<DgmImageViewer *>(activeSubWindow->widget()) != 0 )
			return qobject_cast<DgmImageViewer *>(activeSubWindow->widget());
		else if ( qobject_cast<PbmImageViewer *>(activeSubWindow->widget()) != 0 )
			return qobject_cast<PbmImageViewer *>(activeSubWindow->widget());
		else if ( qobject_cast<PgmImageViewer *>(activeSubWindow->widget()) != 0 )
			return qobject_cast<PgmImageViewer *>(activeSubWindow->widget());
	}
	return 0;
}

BasePnmImageViewer * Lvp::active2DImageViewer() {
	if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow()) {
		if ( qobject_cast<PbmImageViewer *>(activeSubWindow->widget()) != 0 )
			return qobject_cast<PbmImageViewer *>(activeSubWindow->widget());
		else if ( qobject_cast<PgmImageViewer *>(activeSubWindow->widget()) != 0 )
			return qobject_cast<PgmImageViewer *>(activeSubWindow->widget());
	}
	return 0;
}

BaseDnmImageViewer * Lvp::active3DImageViewer() {
	if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow()) {
		if ( qobject_cast<DbmImageViewer *>(activeSubWindow->widget()) != 0 )
			return qobject_cast<DbmImageViewer *>(activeSubWindow->widget());
		else if ( qobject_cast<DgmImageViewer *>(activeSubWindow->widget()) != 0 )
			return qobject_cast<DgmImageViewer *>(activeSubWindow->widget());
	}
	return 0;
}

DgmImageViewer * Lvp::activeDgmImageViewer() {
	if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow()) {
		if ( qobject_cast<DgmImageViewer *>(activeSubWindow->widget()) != 0 )
			return qobject_cast<DgmImageViewer *>(activeSubWindow->widget());
	}
	return 0;
}

DbmImageViewer *Lvp::activeDbmImageViewer() {
	if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow()) {
		if ( qobject_cast<DbmImageViewer *>(activeSubWindow->widget()) != 0 )
			return qobject_cast<DbmImageViewer *>(activeSubWindow->widget());
	}
	return 0;
}

PbmImageViewer * Lvp::activePbmImageViewer() {
	if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow()) {
		if ( qobject_cast<PbmImageViewer *>(activeSubWindow->widget()) != 0 )
			return qobject_cast<PbmImageViewer *>(activeSubWindow->widget());
	}
	return 0;
}

PgmImageViewer * Lvp::activePgmImageViewer() {
	if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow()) {
		if ( qobject_cast<PgmImageViewer *>(activeSubWindow->widget()) != 0 )
			return qobject_cast<PgmImageViewer *>(activeSubWindow->widget());
	}
	return 0;
}

GLWidget * Lvp::activeGLWidget() {
	if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow()) {
		if ( qobject_cast<GLWidget *>(activeSubWindow->widget()) != 0 )
			return qobject_cast<GLWidget *>(activeSubWindow->widget());
	}
	return 0;
}

Ploter * Lvp::activePloter() {
	if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow()) {
		if ( qobject_cast<Ploter *>(activeSubWindow->widget()) != 0 )
			return qobject_cast<Ploter *>(activeSubWindow->widget());
	}
	return 0;
}

TextEditor * Lvp::activeTextEditor() {
	if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow()) {
		if ( qobject_cast<TextEditor *>(activeSubWindow->widget()) != 0 )
			return qobject_cast<TextEditor *>(activeSubWindow->widget());
	}
	return 0;
}

HexEditor * Lvp::activeHexEditor() {
	if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow()) {
		if ( qobject_cast<HexEditor *>(activeSubWindow->widget()) != 0 )
			return qobject_cast<HexEditor *>(activeSubWindow->widget());
	}
	return 0;
}

void Lvp::setActiveSubWindow(QWidget *window) {
	if (!window)
		return;
	mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
	updateMenus();
	updateDockLista();
}

void Lvp::setActiveSubWindow(QListWidgetItem *item) {
	if (!item)
		return;
	foreach (QMdiSubWindow * window, mdiArea->subWindowList()) {
		if ( PbmImageViewer * mdiChild = qobject_cast<PbmImageViewer *>(window->widget()) ) {
			if (item == mdiChild->item) {
				mdiArea->setActiveSubWindow(window);
				break;
			}
		} else if ( PgmImageViewer * mdiChild = qobject_cast<PgmImageViewer *>(window->widget()) ) {
			if (item == mdiChild->item) {
				mdiArea->setActiveSubWindow(window);
				break;
			}
		} else if ( DbmImageViewer * mdiChild = qobject_cast<DbmImageViewer *>(window->widget()) ) {
			if (item == mdiChild->item) {
				mdiArea->setActiveSubWindow(window);
				break;
			}
		} else if ( DgmImageViewer * mdiChild = qobject_cast<DgmImageViewer *>(window->widget()) ) {
			if (item == mdiChild->item) {
				mdiArea->setActiveSubWindow(window);
				break;
			}
		} else if ( Ploter * mdiChild = qobject_cast<Ploter *>(window->widget()) ) {
			if (item == mdiChild->item) {
				mdiArea->setActiveSubWindow(window);
				break;
			}
		} else if ( TextEditor * mdiChild = qobject_cast<TextEditor *>(window->widget()) ) {
			if (item == mdiChild->item) {
				mdiArea->setActiveSubWindow(window);
				break;
			}
		} else {
			updateMenus();
			updateDockLista();
		}
	}
}

QMdiSubWindow *Lvp::findImageViewer(const QString &_fileName) {
	QString canonicalFilePath = QFileInfo(_fileName).canonicalFilePath();
	BaseImageViewer *mdiChild;
	foreach (QMdiSubWindow *window, mdiArea->subWindowList()) {
		mdiChild = nullptr;
		if (qobject_cast<PbmImageViewer *>(window->widget()) != 0)
			mdiChild = qobject_cast<PbmImageViewer *>(window->widget());
		else if (qobject_cast<PgmImageViewer *>(window->widget()) != 0)
			mdiChild = qobject_cast<PgmImageViewer *>(window->widget());
		else if (qobject_cast<DbmImageViewer *>(window->widget()) != 0)
			mdiChild = qobject_cast<DbmImageViewer *>(window->widget());
		else if (qobject_cast<DgmImageViewer *>(window->widget()) != 0)
			mdiChild = qobject_cast<DgmImageViewer *>(window->widget());
		if (mdiChild)
			if (mdiChild->getFullFileName() == canonicalFilePath)
				return window;
	}
	return 0;
}

QMdiSubWindow *Lvp::findGLWidget(const QString &_fileName) {
	QString canonicalFilePath = QFileInfo(_fileName).canonicalFilePath();
	GLWidget *mdiChild;
	foreach (QMdiSubWindow *window, mdiArea->subWindowList()) {
		mdiChild = nullptr;
		if (qobject_cast<GLWidget *>(window->widget()) != 0)
			mdiChild = qobject_cast<GLWidget *>(window->widget());
		if (mdiChild)
			if (mdiChild->getFullFileName() == canonicalFilePath)
				return window;
	}
	return 0;
}

QMdiSubWindow *Lvp::findPloter(const QString & _fileName) {
	//QString canonicalFilePath = QFileInfo(_fileName).canonicalFilePath();
	Ploter *mdiChild;
	foreach (QMdiSubWindow *window, mdiArea->subWindowList()) {
		mdiChild = nullptr;
		if (qobject_cast<Ploter *>(window->widget()) != 0)
			mdiChild = qobject_cast<Ploter *>(window->widget());
		if (mdiChild)
			//if (mdiChild->getFullFileName() == canonicalFilePath)
			if (mdiChild->getFullFileName() == _fileName)
				return window;
	}
	return 0;
}

QMdiSubWindow *Lvp::findTextEditor(const QString & _fileName) {
	//QString canonicalFilePath = QFileInfo(_fileName).canonicalFilePath();
	TextEditor *mdiChild;
	foreach (QMdiSubWindow *window, mdiArea->subWindowList()) {
		mdiChild = nullptr;
		if (qobject_cast<TextEditor *>(window->widget()) != 0)
			mdiChild = qobject_cast<TextEditor *>(window->widget());
		if (mdiChild)
			//if (mdiChild->getFullFileName() == canonicalFilePath)
			if (mdiChild->getFullFileName() == _fileName)
				return window;
	}
	return 0;
}

QMdiSubWindow *Lvp::findHexEditor(const QString & _fileName) {
	QString canonicalFilePath = QFileInfo(_fileName).canonicalFilePath();
	HexEditor *mdiChild;
	foreach (QMdiSubWindow *window, mdiArea->subWindowList()) {
		mdiChild = nullptr;
		if (qobject_cast<HexEditor *>(window->widget()) != 0)
			mdiChild = qobject_cast<HexEditor *>(window->widget());
		if (mdiChild)
			if (mdiChild->CurFile() == canonicalFilePath)
				return window;
	}
	return 0;
}

string Lvp::validateFileName( const string _strname ){
	return validateFileName( QString::fromStdString(_strname) ).toStdString();
}

QString Lvp::validateFileName( const QString _name){
	QString name = _name;
	int cont = 1;
	if (QFile::exists(name)){
		name = name.insert(name.lastIndexOf("."), tr("-(1)"));
		while (QFile::exists(name)){
			cont++;
			name = name.replace(QRegExp("(\\-\\([0-9]{1,4}\\)\\.)"), tr("-(%1).").arg(cont));
		}
	}
	return name;
}

//preenche o menu Language de acordo com o conteúdo do diretório translations
void Lvp::createLanguageMenu() {
	QActionGroup *languageActionGroup = new QActionGroup(this);
	connect(languageActionGroup, SIGNAL(triggered(QAction *)), this, SLOT(switchLanguage(QAction *)));
	QDir dir(qmPath);
	QStringList fileNames = dir.entryList(QStringList("lvp_*.qm"));
	for (int i = 0; i < fileNames.size(); ++i) {
		QString locale = fileNames[i];
		locale.remove(0, locale.indexOf('_') + 1);
		locale.truncate(locale.lastIndexOf('.'));
		QAction *action = new QAction(tr("&%1 %2").arg(i + 1).arg(locale), this);
		action->setCheckable(true);
		action->setData(locale);
		menuLanguage->addAction(action);
		languageActionGroup->addAction(action);
		if (locale == settings->value("language", QString("English")).toString()) {
			action->setChecked(true);
		}
	}
}

void Lvp::switchLanguage(QAction *action)
{
	QString locale = action->data().toString();
	appTranslator.load("lvp_" + locale, qmPath);
	retranslateUi(this);
	settings->setValue("language", locale);
}

void Lvp::writeSettings() { //Grava a posição e tamanho atuais da aplicação
	settings->setValue("pos", pos());
	settings->setValue("size", size());
	settings->setValue("maximized", isMaximized());
	settings->setValue("messagesVisible", dockWidgetMessages->isVisible());
	settings->setValue("lastOpenPath", lastOpenPath);
}

void Lvp::readSettings() { //Carrega a última posição e tamanho usadas para a aplicação.
	QPoint pos = settings->value("pos", QPoint(200, 200)).toPoint();
	move(pos);

	QSize size = settings->value("size", QSize(400, 400)).toSize();
	resize(size);

	bool maximized = settings->value("maximized", bool(false)).toBool();
	if ( maximized ) showMaximized ();

	dockWidgetMessages->setVisible( settings->value("messagesVisible", bool(false)).toBool() );

	lastOpenPath = settings->value("lastOpenPath", QString(qApp->applicationDirPath())).toString();

	QString locale = settings->value("language", QString("English")).toString();
	appTranslator.load("lvp_" + locale, qmPath);
	int viewMode = settings->value("viewMode", 0).toInt();
	if ( viewMode ) {
		mdiArea->setViewMode( QMdiArea::TabbedView );
	} else {
		mdiArea->setViewMode( QMdiArea::SubWindowView );
	}

	foreach (QMdiSubWindow *window, mdiArea->subWindowList()) {
		if (qobject_cast<HexEditor *>(window->widget()) != 0) {
			HexEditor *hexEdit = qobject_cast<HexEditor *>(window->widget());
			hexEdit->setAddressArea(settings->value("AddressArea").toBool());
			hexEdit->setAsciiArea(settings->value("AsciiArea").toBool());
			hexEdit->setHighlighting(settings->value("Highlighting").toBool());
			hexEdit->setOverwriteMode(settings->value("OverwriteMode").toBool());
			hexEdit->setReadOnly(settings->value("ReadOnly").toBool());
			hexEdit->setHighlightingColor(settings->value("HighlightingColor").value<QColor>());
			hexEdit->setAddressAreaColor(settings->value("AddressAreaColor").value<QColor>());
			hexEdit->setSelectionColor(settings->value("SelectionColor").value<QColor>());
			hexEdit->setFont(settings->value("WidgetFont").value<QFont>());
			hexEdit->setAddressWidth(settings->value("AddressAreaWidth").toInt());
		}
	}
}

void Lvp::about() {
	QMessageBox::about(this, tr("About LVP"), tr("The <b>LVP</b> is a Petrophysical Virtual Laboratory."));
}

void Lvp::updateStatusBar() {
	foreach (QMdiSubWindow *window, mdiArea->subWindowList()) {
		if (qobject_cast<HexEditor *>(window->widget()) != 0) {
			HexEditor *mdiChild = qobject_cast<HexEditor *>(window->widget());
			mdiChild->destroyStatusBar();
		} else if (qobject_cast<BasePnmImageViewer *>(window->widget()) != 0) {
			BasePnmImageViewer *mdiChild = qobject_cast<BasePnmImageViewer *>(window->widget());
			mdiChild->destroyStatusBar();
		} else if (qobject_cast<BaseDnmImageViewer *>(window->widget()) != 0) {
			BaseDnmImageViewer *mdiChild = qobject_cast<BaseDnmImageViewer *>(window->widget());
			mdiChild->destroyStatusBar();
		}
	}
	if ( activeHexEditor() != 0 ) {
		activeHexEditor()->createStatusBar();
	} else if ( activeImageViewer() != 0 ) {
		activeImageViewer()->createStatusBar();
	}
}

void Lvp::options(){
	if ( ! dialogHexEditor ) {
		dialogHexEditor = new OptionsDialog( this );
		connect(dialogHexEditor, SIGNAL(accepted()), this, SLOT(optionsAccepted()));
	}
	dialogHexEditor->show();
}

void Lvp::optionsAccepted() {
	writeSettings();
	readSettings();
}
