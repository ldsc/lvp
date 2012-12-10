#ifndef LVP_H
#define LVP_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QPrinter>
#include <QDir>
#include <QTranslator>
#include <QtGui/QRadioButton>
#include <QtGui/QGroupBox>
//#include <QVariant>
#include "ui_lvpinterface.h"
#include "confEq.h"
#include "reconstruction.h"
#include "reconstructionES.h"
#include "import.h"
#include "baseImageViewer.h"
#include "dbmImageViewer.h"
#include "dgmImageViewer.h"
#include "pbmImageviewer.h"
#include "pgmImageviewer.h"
#include "glwidget.h"
#include "ploter.h"
#include "textEditor.h"
#include "hexEditor.h"
#include <Correlacao/CCorrelacao3D.h>
#include <Distribuicao/CBaseDistribuicao.h>

QT_BEGIN_NAMESPACE
class QMdiSubWindow;
class QSignalMapper;
class QMdiArea;
class QListWidget;
class QListWidgetItem;
/*
class QAction;
class QMenu;
class QUndoStack;
*/
QT_END_NAMESPACE

/**
\mainpage
O software LVP possibilita, através da análise de imagens de rochas
reservatório de petróleo, a caracterização e determinação de propriedades
petrofísicas.

As funcionalidades disponibilizadas no LVP estão implementadas na
biblioteca computacional para análise de imagens de meios porosos
lib_ldsc, desenvolvida com base em trabalhos científcos de diversos
autores.

Escrito em C++, utiliza a framework Qt (Qt4) para facilitar sua usabilidade
através de uma interface gráfica avançada, moderna e amigável.

O software LVP é distribuído sob a licença GPL
(General Public License - http://www.gnu.org/licenses/gpl.html).
É software livre, podendo ser copiado, modificado e distribuído por
toda a comunidade profissional e acadêmica.

Está disponível nos idiomas Inglês e Português do Brasil, podendo ser
facilmente traduzido para outros idiomas.

---

The LVP software enables, through image analysis of petroleum reservoir
rocks, the characterization and determination of petrophysical properties.

---

\version 2.0.0
\image html splashScream.png
*/
/*! Lvp é a classe principal do software LVP. */
class Lvp : public QMainWindow, public Ui::MainWindow
{
		Q_OBJECT
	public:
		//Construtor
		Lvp();
		//Destrutor
		virtual ~Lvp() {
			delete settings;
			if (dialog)				delete dialog;
			if (dialogGT)			delete dialogGT;
			if (dialogES)			delete dialogES;
			if (dialogImport)	delete dialogImport;
		}

		// Atributos
	public:
		// Tipos de operações da Morfologia Matemática
		enum MorphType {
			mtErosion,
			mtDilation,
			mtOpening,
			mtClosing
		};

		//Tipos de Metricas para imagens 2D
		enum Metrics2D {
			m2DSpatial,
			m2Dd34,
			m2Dd5711,
			m2DEuclidian
		};

		//Tipos de Metricas para imagens 3D
		enum Metrics3D {
			m3DSpatial,
			m3Dd34,
			m3Dd345,
			m3Dd5711,
			m3DEuclidian
		};

		// Último diretório acessado
		QString lastOpenPath;
		QPushButton *pushButtonAccumulated;

	private:
		QMdiArea *mdiArea;
		QListWidget *listWidget;
		QListWidget *listWidget3D;
		QListWidget *listWidgetChart;
		QListWidget *listWidgetEditor;
		QPushButton *pushButtonAddCurve;
		QPushButton *pushButtonAverage;
		QPushButton *pushButtonSource;
		//QVBoxLayout *verticalLayout;
		QGridLayout *gridLayout;
		QGridLayout *gridLayoutChart;
		QLabel *labelPlano3D;
		QSpinBox *spinBoxPlano3D;
		QSlider *horizontalSliderPlano3D;

		QGroupBox *groupBox;
		QRadioButton *radioButtonX;
		QRadioButton *radioButtonY;
		QRadioButton *radioButtonZ;
		QLabel *labelAxis;

		QSignalMapper *windowMapper;

		ConfEq * dialog;
		Reconstruction   * dialogGT;
		ReconstructionES * dialogES;
		Import * dialogImport;

		QTranslator appTranslator;
		QString qmPath;
		QSettings * settings;

#ifndef QT_NO_PRINTER
		QPrinter printer;
#endif

		//Métodos
	public slots:
		void open(std::string _file, bool novo = true);

	private slots:
		void about();
		void accumulated();
		void addCurve();
		void average();
		void changeViewMode();
		void closing();
		void closing3D();
		void connectedObjects();
		void confEq();
		void connectivity3D();
		void copy();
		void correlationFFT();
		void correlationFFT3D();
		void correlationSpatial();
		void correlationSpatial3D();
		void dilation();
		void dilation3D();
		void dtsSpatial();
		void dtsSpatial3D();
		void dtsD34();
		void dtsD34_3D();
		void dtsD345();
		void dtsD5711();
		void dtsD5711_3D();
		void dtsEuclidian();
		void dtsEuclidian3D();
		void dtpSpatial();
		void dtpSpatial3D();
		void dtpD34();
		void dtpD34_3D();
		void dtpD345();
		void dtpD5711();
		void dtpD5711_3D();
		void dtpEuclidian();
		void dtpEuclidian3D();
		void updateActionSave();
		void erosion();
		void erosion3D();
		void exChangeAxis();
		void exChangePlan( int _plan );
		void exChangePlanX( int _plan );
		void exChangePlanY( int _plan );
		void exChangePlanZ( int _plan );
		void exConfEq();
		void exConfEq3D();
		void exImport();
		void exReconstructionGT();
		void exReconstructionES();
		void fitToWindow();
		void highPass();
		void idf();
		void intrinsicPermeability();
		void import();
		void inversion();
		void inversion3D();
		void invertPoro();
		void lowPass();
		void normalSize();
		void open();
		void opening();
		void opening3D();
		void open3DVisualization();
		void openMPV( );
		void openEditor();
		void porosity();
		void print();
		void reconstructionES();
		void reconstructionGT();
		void relativePermeability();
		void rotate();
		void save();
		void saveAs();
		void setActiveSubWindow(QWidget *window);
		void setActiveSubWindow(QListWidgetItem *item);
		void skeleton(int type);
		void skeletonV1();
		void skeletonV2();
		void skeletonV3();
		void skeletonV4();
		void skeletonV5();
		void switchLanguage(QAction *action);
		void updateDockLista();
		void updateMenus();
		void updateWindowMenu();
		void updateStatusBar();
		void zoomIn();
		void zoomOut();
		GLWidget * createGLWidget(DbmImageViewer *_mdiChild);
		GLWidget * createGLWidget(DgmImageViewer *_mdiChild);
		PbmImageViewer * createPbmImageViewer();
		PgmImageViewer * createPgmImageViewer();
		DbmImageViewer * createDbmImageViewer();
		DgmImageViewer * createDgmImageViewer();
		Ploter * createPloter();
		TextEditor * createTextEditor();
		HexEditor * createHexEditor();

	protected:
		void closeEvent(QCloseEvent *event);
		void correlation3D( CCorrelacao3D::Tipos tipo );
		void mathematicalMorphology( MorphType mtype );
		void mathematicalMorphology3D( MorphType mtype );
		void distribution( CBaseDistribuicao::Tipos tipo, Metrics2D m2d );
		void distribution3D( CBaseDistribuicao::Tipos tipo, Metrics3D m3d );

	private:
		void createActions();
		void readSettings();
		void writeSettings();
		void createLanguageMenu();
		BaseImageViewer *activeImageViewer();
		BaseDnmImageViewer *active3DImageViewer();
		BasePnmImageViewer *active2DImageViewer();
		DbmImageViewer *activeDbmImageViewer();
		DgmImageViewer *activeDgmImageViewer();
		PbmImageViewer *activePbmImageViewer();
		PgmImageViewer *activePgmImageViewer();
		GLWidget *activeGLWidget();
		Ploter *activePloter();
		TextEditor *activeTextEditor();
		HexEditor *activeHexEditor();
		QList<BasePnmImageViewer *> selected2DImagesList();
		QList<BaseDnmImageViewer *> selected3DImagesList();
		QList<PbmImageViewer *> selectedPbmImagesList();
		QList<PgmImageViewer *> selectedPgmImagesList();
		QList<DbmImageViewer *> selectedDbmImagesList();
		QList<DgmImageViewer *> selectedDgmImagesList();
		QList<Ploter *> selectedPloterList();
		QList<BaseImageViewer *> selectedAllImagesList();
		QMdiSubWindow *findImageViewer(const QString &_fileName);
		QMdiSubWindow *findGLWidget(const QString &_fileName);
		QMdiSubWindow *findPloter(const QString &_fileName);
		QMdiSubWindow *findTextEditor(const QString & _fileName);
		QMdiSubWindow *findHexEditor(const QString & _fileName);
		//verifica se existe arquivo com o nome informado. Caso exista retorna o nome de um arquivo inexistente.
		string validateFileName( const string _strname );
		QString validateFileName( const QString _name);

		//From HexEditor
//		QLabel *lbAddress, *lbAddressName;
//		QLabel *lbOverwriteMode, *lbOverwriteModeName;
//		QLabel *lbSize, *lbSizeName;
};

#endif
