#ifndef HEXEDITOR_H
#define HEXEDITOR_H

#include "qHexEdit/qhexedit.h"
//#include "optionsdialog.h"
//#include "searchdialog.h"
/*
QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QUndoStack;
QT_END_NAMESPACE
*/

/*! HexEditor é a classe que disponibiliza o editor exadecimal QHexEdit. */
class HexEditor : public QHexEdit //: public QMainWindow
{
		Q_OBJECT

	public:
		HexEditor(QMainWindow *_parent = 0);
		~HexEditor(){
			destroyStatusBar();
			if (lbSize) delete lbSize;
			if (lbAddress) delete lbAddress;
			if (lbSizeName) delete lbSizeName;
			if (lbAddressName) delete lbAddressName;
			if (lbOverwriteMode) delete lbOverwriteMode;
			if (lbOverwriteModeName) delete lbOverwriteModeName;
		}

	//protected:
		//void closeEvent(QCloseEvent *event);

	public slots:
		void about();
		void createStatusBar();
		void destroyStatusBar();
		bool loadFile(const QString &fileName);
		void open();
		//void optionsAccepted();
		//void findNext();
		bool save();
		bool saveAs();
		void saveSelectionToReadableFile();
		void saveToReadableFile();
		void setAddress(int address);
		void setOverwriteMode(bool mode);
		void setSize(int size);
		//void showOptionsDialog();
		//void showSearchDialog();
		void documentWasModified();
		inline bool IsModified() { return isModified; }
		inline QString CurFile() { return curFile; }

	private:
		//void init();
		//void createActions();
		//void createMenus();
		//void createToolBars();
		//void readSettings();
		bool saveFile(const QString &fileName);
		void setCurrentFile(const QString &fileName);
		QString strippedName(const QString &fullFileName);
		//void writeSettings();

		QString curFile;
		bool isUntitled;
		bool isModified;

		QMainWindow *parent;

		QMenu *fileMenu;
		QMenu *editMenu;
		QMenu *helpMenu;

		QToolBar *fileToolBar;

		QAction *openAct;
		QAction *saveAct;
		QAction *saveAsAct;
		QAction *saveReadable;
		QAction *closeAct;
		QAction *exitAct;

		QAction *undoAct;
		QAction *redoAct;
		QAction *saveSelectionReadable;

		QAction *aboutAct;
		QAction *aboutQtAct;
		QAction *optionsAct;
		QAction *findAct;
		QAction *findNextAct;

		//QHexEdit *hexEdit;
		//OptionsDialog *optionsDialog;
		//SearchDialog *searchDialog;
		QLabel *lbAddress, *lbAddressName;
		QLabel *lbOverwriteMode, *lbOverwriteModeName;
		QLabel *lbSize, *lbSizeName;

};

#endif
