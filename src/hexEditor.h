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

class HexEditor : public QHexEdit //: public QMainWindow
{
		Q_OBJECT

	public:
		HexEditor(QMainWindow *_parent = 0);

	//protected:
		//void closeEvent(QCloseEvent *event);

	public slots:
		void about();
		bool loadFile(const QString &fileName);
		void open();
		//void optionsAccepted();
		//void findNext();
		bool save();
		bool saveAs();
		void saveSelectionToReadableFile();
		void saveToReadableFile();
		//void showOptionsDialog();
		//void showSearchDialog();
		void documentWasModified();
		inline bool IsModified() { return isModified; }

	private:
		//void init();
		//void createActions();
		//void createMenus();
		//void createStatusBar();
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

};

#endif
