#ifndef LVP_H
#define LVP_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QPrinter>
#include <QDir>
#include <QFileSystemWatcher>
//#include <QProcess>

#include <QTranslator>
#include <QRadioButton>
#include <QGroupBox>
#include <QApplication>
#include <QCoreApplication>
#include "ui_lvpinterface.h"
#include "confEq.h"
#include "reconstruction.h"
#include "reconstructionES.h"
#include "import.h"
#include "crop3D.h"
#include "baseImageViewer.h"
#include "dbmImageViewer.h"
#include "dgmImageViewer.h"
#include "pbmImageviewer.h"
#include "pgmImageviewer.h"
#include "glwidget.h"
#include "intrinsicPermeabilityByNetwork.h"
#include "percolationNetwork.h"
#include "poresThroats.h"
#include "ploter.h"
#include "properties.h"
#include "textEditor.h"
#include "hexEditor.h"
#include "optionsdialog.h"
#include "poresThroats.h"
#include "qdebugstream.h"
#include <AnaliseImagem/Caracterizacao/Correlacao/CCorrelacao3D.h>
#include <AnaliseImagem/Caracterizacao/Distribuicao/CBaseDistribuicao.h>

QT_BEGIN_NAMESPACE
class QMdiSubWindow;
class QSignalMapper;
class QMdiArea;
class QListWidget;
class QListWidgetItem;
class QMimeData;
//class QAction;
//class QMenu;
//class QUndoStack;
QT_END_NAMESPACE

/**
\mainpage
---

O software LVP possibilita, através da análise de imagens de rochas reservatório de petróleo, a caracterização e determinação de propriedades petrofísicas.
As funcionalidades disponibilizadas no LVP estão implementadas na biblioteca computacional para análise de imagens de meios porosos lib_ldsc, desenvolvida com base em trabalhos científicos de diversos autores.
Escrito em C++, utiliza a framework Qt (Qt4) para facilitar sua usabilidade através de uma interface gráfica avançada, moderna e amigável.
O software LVP é distribuído sob a licença GPL (General Public License - http://www.gnu.org/licenses/gpl.html).
É software livre, podendo ser copiado, modificado e distribuído por toda a comunidade profissional e acadêmica.
Está disponível nos idiomas Inglês e Português do Brasil, podendo ser facilmente traduzido para outros idiomas.

---

The LVP software enables, through image analysis of petroleum reservoir rocks, the characterization and determination of petrophysical properties.
The features available on the LVP software are implemented on lib_ldsc, a computational library for image analysis of porous media, developed based on scientific works of several authors.
Written in C++, using the Qt framework (Qt4) to facilitate its usability through a graphical, modern and friendly user interface.
The LVP software is distributed under the GPL license (General Public License - http://www.gnu.org/licenses/gpl.html).
It is free software and can be copied, modified and distributed by all professional and academic community.
It is available in English and Portuguese and can be easily translated into other languages​​.

---

\version 0.2.0
\image html splashScream.png
*/

/*! Lvp é a classe principal do Software LVP. */
class Lvp : public QMainWindow, public Ui::MainWindow
{
		Q_OBJECT
	public:
		/// Construtor
		Lvp();

		/// Destrutor
		virtual ~Lvp() {
			//delete
			delete settings;
			if (qout) delete qout;
			if (qerr)	delete qerr;
			if (dialog) delete dialog;
			if (dialogGT) delete dialogGT;
			if (dialogES)	delete dialogES;
			if (dialogImport) delete dialogImport;
			if (dialogPoresThroats) delete dialogPoresThroats;
			if (dialogPercolationNetwork) delete dialogPercolationNetwork;
			if (dialogIntrinsicPermeabilityByNetwork) delete dialogIntrinsicPermeabilityByNetwork;
		}
		// Enums
	private:
		/// Tipos de operações da Morfologia Matemática
		enum MorphType {
			mtErosion,
			mtDilation,
			mtOpening,
			mtClosing
		};

		/// Tipos de Metricas para  imagens 2D
		enum Metrics2D {
			m2DSpatial,
			m2Dd34,
			m2Dd5711,
			m2DEuclidian
		};

		/// Tipos de Metricas para imagens 3D
		enum Metrics3D {
			m3DSpatial,
			m3Dd34,
			m3Dd345,
			m3Dd5711,
			m3DEuclidian
		};

		// Atributos
	public:
		/// Último diretório acessado
		QString lastOpenPath;

	private:
		QMdiArea *mdiArea = nullptr;
		QListWidget *listWidget = nullptr;
		QListWidget *listWidget3D = nullptr;
		QListWidget *listWidgetChart = nullptr;
		QListWidget *listWidgetEditor = nullptr;
		QPushButton *pushButtonAddCurve = nullptr;
		QPushButton *pushButtonAverage = nullptr;
		QPushButton *pushButtonSource = nullptr;
		QPushButton *pushButtonAccumulated = nullptr;
		//redirect cout and cerr
		QPlainTextEdit *textEditMessages = nullptr;
		QDebugStream *qout = nullptr;
		QDebugStream *qerr = nullptr;

		//QVBoxLayout *verticalLayout;
		QGridLayout *gridLayout = nullptr;
		QGridLayout *gridLayoutChart = nullptr;
		QLabel *labelPlano3D = nullptr;
		QSpinBox *spinBoxPlano3D = nullptr;
		QSlider *horizontalSliderPlano3D = nullptr;

		QGroupBox *groupBox = nullptr;
		QRadioButton *radioButtonX = nullptr;
		QRadioButton *radioButtonY = nullptr;
		QRadioButton *radioButtonZ = nullptr;
		QLabel *labelAxis = nullptr;

		QSignalMapper *windowMapper = nullptr;
		QFileSystemWatcher *fileWatcher = nullptr; ///< The QFileSystemWatcher class provides an interface for monitoring files and directories for modifications

		PoresThroats * dialogPoresThroats = nullptr;
		IntrinsicPermeabilityByNetwork * dialogIntrinsicPermeabilityByNetwork = nullptr;
		PercolationNetwork * dialogPercolationNetwork = nullptr;
		OptionsDialog * dialogHexEditor = nullptr;
		Reconstruction   * dialogGT = nullptr;
		ReconstructionES * dialogES = nullptr;
		Properties * dialogProperties = nullptr;
		Import * dialogImport = nullptr;
		Crop3D * dialogCrop = nullptr;
		ConfEq * dialog = nullptr;

		QTranslator appTranslator;
		QString qmPath;
		QSettings * settings = nullptr;

#ifndef QT_NO_PRINTER
		QPrinter printer;
#endif

		//Métodos
	public slots:
		/// Método para abertura de arquivos suportados pelo LVP
		void open(std::string _file, bool novo = true);
		/// Método para importar arquivos RAW informando o nome do arquivo.
		void import(QString file);

	private slots:
		void about();///< Exibe a tela Sobre (about)
		void accumulated();///< Acumula os valores de um gráfico e exibe a curva de valores acumulados
		void addCurve();///< Adiciona curva ao gráfico
		void average();///< Calcula a média das curvas exibidas em um gráfico
		void changeViewMode();///< Muda o modo de visualização de uma imagem 3D
		void closeActiveSubWindow();///< Fecha a aba ativa
		void closeAllSubWindows();///< Fecha todas as abas
		void closing();///< Aplica a operação morfológica de fechamento em imagens 2D
		void closing3D();///< Aplica a operação morfológica de fechamento em imagens 3D
		void connectedObjects();///< Filtra e exibe os objetos interconectados em uma imagem 3D
		void confEq();///< Define as configurações de equilíbrio em imagens binárias
		void connectivity3D();///< Verifica se uma imagem 3D possui conectividade entre duas fronteiras paralelas
		void copy();
		void correlationFFT();///< Executa correlação em imagens 2D binárias pela Trasformada de Fourier
		void correlationFFT3D();///< Executa correlação em imagens 3D binárias pela Trasformada de Fourier
		void correlationSpatial();///< Executa correlação espacial em imagens 2D binárias
		void correlationSpatial3D();///< Executa correlação espacial em imagens 3D binárias
		void crop3DImage();
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
		void dtpgD345_3D(); ///< Calcula a distribuição de tamanho de poros e gargantas de imagens 3D, utilizando métrica D345.
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
		void exIntrinsicPermeabilityByNetwork();
		void exPercolationNetwork();
		void exProperties();
		void exCrop3DImage();
		void exReconstructionGT();
		void exReconstructionES();
		void exSegmentationPoresThroats();
		void treatFileChanged( QString _file );
		void fitToWindow();
		void highPass();
		void idf();
		void ira();
		void intrinsicPermeability();
		void intrinsicPermeabilityByNetwork();
		void import();
		void inversion();
		void inversion3D();
		void invertPoro();
		void labeling3D();
		void lowPass();
		void normalSize();
		void open();
		void opening();
		void opening3D();
		void open3DVisualization();
		void openMPV( );
		void openPNV( );
		void openEditor();
		void options(); ///< Abre diálogo de opções para o editor hexadecimal
		void optionsAccepted(); ///< Grava e lê as opções do editor hexadecimal
		void segmentationPoresThroats(); /// Executa segmentação de poros e gargantas
		void percolationNetwork(); /// Abre diálogo de opções para a criação de redes de percolação
		void porosity();
		void print();
		void properties();
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
		GLWidget * createGLWidget(TextEditor *_mdiChild);
		PbmImageViewer * createPbmImageViewer();
		PgmImageViewer * createPgmImageViewer();
		DbmImageViewer * createDbmImageViewer();
		DgmImageViewer * createDgmImageViewer();
		Ploter * createPloter();
		TextEditor * createTextEditor();
		HexEditor * createHexEditor();
		/*
		void readStdOutput() {
			textEditMessages->append(process->readAllStandardOutput());
		}
		void readStdError() {
			textEditMessages->append(process->readAllStandardError());
		}
		void readError() {
			textEditMessages->append("An Error Occured! Error Code is: "+QString::number(process->error()));
		}*/

	protected:
		/// Sobrecarrega método chamado quando o usuário fecha o software.
		void closeEvent(QCloseEvent *event);

		/// Sobrecarrega método que possibilita arrastar imagens para serem abertas no LVP.
		void dragEnterEvent(QDragEnterEvent *event);

		/// Sobrecarrega método que possibilita soltar imagens para serem abertas no LVP.
		void dropEvent(QDropEvent *event);

		//void dragMoveEvent(QDragMoveEvent *event);
		//void dragLeaveEvent(QDragLeaveEvent *event);

	private:
		/// Executa correlação em imagem 3D
		void correlation3D( CCorrelacao3D::Tipos tipo );
		/// Executa operações morfológicas em imagens 2D
		void mathematicalMorphology( MorphType mtype );
		/// Executa operações morfológicas em imagens 3D
		void mathematicalMorphology3D( MorphType mtype );
		/// Calcula distribuição em imagem 2D
		void distribution( CBaseDistribuicao::Tipos tipo, Metrics2D m2d );
		/// Calcula distribuição em imagem 3D
		void distribution3D( CBaseDistribuicao::Tipos tipo, Metrics3D m3d );
		/// Cria ações dos objetos da interface QT
		void createActions();
		/// Lê últimas configurações utilizadas
		void readSettings();
		/// Grava as últimas configurações utilizadas
		void writeSettings();
		/// Gera dinamicamete o menu de traduções do software
		void createLanguageMenu();
		/// Pega ponteiro para widget Image Viewer ativo
		BaseImageViewer *activeImageViewer();
		/// Pega ponteiro para widget Image Viewer 3D ativo
		BaseDnmImageViewer *active3DImageViewer();
		/// Pega ponteiro para widget Image Viewer 2D ativo
		BasePnmImageViewer *active2DImageViewer();
		/// Pega ponteiro para widget de imagem DBM ativa
		DbmImageViewer *activeDbmImageViewer();
		/// Pega ponteiro para widget de imagem DGM ativa
		DgmImageViewer *activeDgmImageViewer();
		/// Pega ponteiro para widget de imagem PBM ativa
		PbmImageViewer *activePbmImageViewer();
		/// Pega ponteiro para widget de imagem PGM ativa
		PgmImageViewer *activePgmImageViewer();
		/// Pega ponteiro para widget OpenGL ativo
		GLWidget *activeGLWidget();
		/// Pega ponteiro para widget Ploter ativo
		Ploter *activePloter();
		/// Pega ponteiro para widget Editor de Textos ativo
		TextEditor *activeTextEditor();
		/// Pega ponteiro para widget para visualizador exadecimal ativa
		HexEditor *activeHexEditor();
		/// Pega lista de imagens 2D selecionadas
		QList<BasePnmImageViewer *> selected2DImagesList();
		/// Pega lista de imagens 3D selecionadas
		QList<BaseDnmImageViewer *> selected3DImagesList();
		/// Pega lista de imagens PBM selecionadas
		QList<PbmImageViewer *> selectedPbmImagesList();
		/// Pega lista de imagens PGM selecionadas
		QList<PgmImageViewer *> selectedPgmImagesList();
		/// Pega lista de imagens DBM selecionadas
		QList<DbmImageViewer *> selectedDbmImagesList();
		/// Pega lista de imagens DGM selecionadas
		QList<DgmImageViewer *> selectedDgmImagesList();
		/// Pega lista graficos selecionados
		QList<Ploter *> selectedPloterList();
		/// Pega lista de todas as imagens selecionadas
		QList<BaseImageViewer *> selectedAllImagesList();
		/// Retorna ponteiro para janela de widget Image Viewer de acordo com o nome de arquivo passado como parâmetro
		QMdiSubWindow *findImageViewer(const QString &_fileName);
		/// Retorna ponteiro para janela de widget OpenGL de acordo com o nome de arquivo passado como parâmetro
		QMdiSubWindow *findGLWidget(const QString &_fileName);
		/// Retorna ponteiro para janela de widget Ploter de acordo com o nome de arquivo passado como parâmetro
		QMdiSubWindow *findPloter(const QString &_fileName);
		/// Retorna ponteiro para janela de widget Text Editor de acordo com o nome de arquivo passado como parâmetro
		QMdiSubWindow *findTextEditor(const QString & _fileName);
		/// Retorna ponteiro para janela de widget Hex Editor de acordo com o nome de arquivo passado como parâmetro
		QMdiSubWindow *findHexEditor(const QString & _fileName);
		/// Verifica se existe arquivo com o nome informado. Caso exista retorna o nome de um arquivo inexistente.
		string validateFileName( const string _strname );
		/// Verifica se existe arquivo com o nome informado. Caso exista retorna o nome de um arquivo inexistente.
		QString validateFileName( const QString _name);

		//From HexEditor
		//QLabel *lbAddress, *lbAddressName;
		//QLabel *lbOverwriteMode, *lbOverwriteModeName;
		//QLabel *lbSize, *lbSizeName;
};

#endif
