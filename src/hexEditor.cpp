#include <QtGui>

#include "hexEditor.h"

/*****************************************************************************/
/* Public methods */
/*****************************************************************************/
HexEditor::HexEditor(QMainWindow *_parent) : QHexEdit(_parent) {
	//init();
	parent = _parent;
	setCurrentFile("");
	isUntitled = true;
	isModified = false;
	connect(this, SIGNAL(dataChanged()), this, SLOT(documentWasModified()));
}

/*****************************************************************************/
/* Private Slots */
/*****************************************************************************/
void HexEditor::documentWasModified() {
	setWindowTitle(tr("%1[*]").arg(strippedName(curFile)));
	setWindowModified(true);
	isModified = true;
}

void HexEditor::about() {
	QMessageBox::about(this, tr("About QHexEdit"), tr("The QHexEdit example is a short Demo of the QHexEdit Widget."));
}

void HexEditor::open() {
	QString fileName = QFileDialog::getOpenFileName(this);
	if (!fileName.isEmpty()) {
		loadFile(fileName);
	}
}

bool HexEditor::save() {
	if (isUntitled) {
		return saveAs();
	} else {
		return saveFile(curFile);
	}
}

bool HexEditor::saveAs() {
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), curFile);
	if (fileName.isEmpty())
		return false;

	return saveFile(fileName);
}

void HexEditor::saveSelectionToReadableFile() {
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save To Readable File"));
	if (!fileName.isEmpty()) {
		QFile file(fileName);
		if (!file.open(QFile::WriteOnly | QFile::Text)) {
			QMessageBox::warning(this, tr("QHexEdit"), tr("Cannot write file %1:\n%2.").arg(fileName).arg(file.errorString()));
			return;
		}

		QApplication::setOverrideCursor(Qt::WaitCursor);
		file.write(selectionToReadableString().toLatin1());
		QApplication::restoreOverrideCursor();

		parent->statusBar()->showMessage(tr("File saved"), 2000);
	}
}

void HexEditor::saveToReadableFile() {
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save To Readable File"));
	if (!fileName.isEmpty()) {
		QFile file(fileName);
		if (!file.open(QFile::WriteOnly | QFile::Text)) {
			QMessageBox::warning(this, tr("QHexEdit"),tr("Cannot write file %1:\n%2.").arg(fileName).arg(file.errorString()));
			return;
		}

		QApplication::setOverrideCursor(Qt::WaitCursor);
		file.write(toReadableString().toLatin1());
		QApplication::restoreOverrideCursor();

		parent->statusBar()->showMessage(tr("File saved"), 2000);
	}
}

bool HexEditor::loadFile(const QString &fileName) {
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly)) {
		QMessageBox::warning(this, tr("SDI"), tr("Cannot read file %1:\n%2.") .arg(fileName) .arg(file.errorString()));
		return false;
	}

	QApplication::setOverrideCursor(Qt::WaitCursor);
	setData(file.readAll());
	QApplication::restoreOverrideCursor();

	setCurrentFile(fileName);
	setWindowTitle(tr("%1").arg(strippedName(fileName)));
	parent->statusBar()->showMessage(tr("File loaded"), 2000);
	return true;
}

bool HexEditor::saveFile(const QString &fileName) {
	QFile file(fileName);
	if (!file.open(QFile::WriteOnly | QFile::Text)) {
		QMessageBox::warning(this, tr("QHexEdit"), tr("Cannot write file %1:\n%2.").arg(fileName).arg(file.errorString()));
		return false;
	}

	QApplication::setOverrideCursor(Qt::WaitCursor);
	file.write(data());
	QApplication::restoreOverrideCursor();

	setCurrentFile(fileName);
	isModified = false;
	parent->statusBar()->showMessage(tr("File saved"), 2000);
	return true;
}

void HexEditor::setCurrentFile(const QString &fileName) {
	curFile = QFileInfo(fileName).canonicalFilePath();
	isUntitled = fileName.isEmpty();
	isModified = false;
	setWindowModified(false);
	setWindowFilePath(curFile);
}

QString HexEditor::strippedName(const QString &fullFileName) {
	return QFileInfo(fullFileName).fileName();
}

/*
void HexEditor::findNext() {
	searchDialog->findNext();
}

void HexEditor::showOptionsDialog() {
	optionsDialog->show();
}

void HexEditor::showSearchDialog() {
	searchDialog->show();
}
*/

/*****************************************************************************/
/* Private Methods */
/*****************************************************************************/
/*
void HexEditor::init()
{
	setAttribute(Qt::WA_DeleteOnClose);
	optionsDialog = new OptionsDialog(this);
	connect(optionsDialog, SIGNAL(accepted()), this, SLOT(optionsAccepted()));
	isUntitled = true;

	hexEdit = new QHexEdit;
	setCentralWidget(hexEdit);
	connect(hexEdit, SIGNAL(overwriteModeChanged(bool)), this, SLOT(setOverwriteMode(bool)));
	searchDialog = new SearchDialog(hexEdit, this);

	createActions();
	createMenus();
	createToolBars();
	createStatusBar();

	readSettings();

	setUnifiedTitleAndToolBarOnMac(true);
}


void HexEditor::createActions()
{
	openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
	openAct->setShortcuts(QKeySequence::Open);
	openAct->setStatusTip(tr("Open an existing file"));
	connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

	saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
	saveAct->setShortcuts(QKeySequence::Save);
	saveAct->setStatusTip(tr("Save the document to disk"));
	connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

	saveAsAct = new QAction(tr("Save &As..."), this);
	saveAsAct->setShortcuts(QKeySequence::SaveAs);
	saveAsAct->setStatusTip(tr("Save the document under a new name"));
	connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

	saveReadable = new QAction(tr("Save &Readable..."), this);
	saveReadable->setStatusTip(tr("Save document in readable form"));
	connect(saveReadable, SIGNAL(triggered()), this, SLOT(saveToReadableFile()));

	exitAct = new QAction(tr("E&xit"), this);
	exitAct->setShortcuts(QKeySequence::Quit);
	exitAct->setStatusTip(tr("Exit the application"));
	connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

	undoAct = new QAction(tr("&Undo"), this);
	undoAct->setShortcuts(QKeySequence::Undo);
	connect(undoAct, SIGNAL(triggered()), hexEdit, SLOT(undo()));

	redoAct = new QAction(tr("&Redo"), this);
	redoAct->setShortcuts(QKeySequence::Redo);
	connect(redoAct, SIGNAL(triggered()), hexEdit, SLOT(redo()));

	saveSelectionReadable = new QAction(tr("&Save Selection Readable..."), this);
	saveSelectionReadable->setStatusTip(tr("Save selection in readable form"));
	connect(saveSelectionReadable, SIGNAL(triggered()), this, SLOT(saveSelectionToReadableFile()));

	aboutAct = new QAction(tr("&About"), this);
	aboutAct->setStatusTip(tr("Show the application's About box"));
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

	aboutQtAct = new QAction(tr("About &Qt"), this);
	aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
	connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

	findAct = new QAction(tr("&Find/Replace"), this);
	findAct->setShortcuts(QKeySequence::Find);
	findAct->setStatusTip(tr("Show the Dialog for finding and replacing"));
	connect(findAct, SIGNAL(triggered()), this, SLOT(showSearchDialog()));

	findNextAct = new QAction(tr("Find &next"), this);
	findNextAct->setShortcuts(QKeySequence::FindNext);
	findNextAct->setStatusTip(tr("Find next occurrence of the searched pattern"));
	connect(findNextAct, SIGNAL(triggered()), this, SLOT(findNext()));

	optionsAct = new QAction(tr("&Options"), this);
	optionsAct->setStatusTip(tr("Show the Dialog to select applications options"));
	connect(optionsAct, SIGNAL(triggered()), this, SLOT(showOptionsDialog()));
}

void HexEditor::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(openAct);
	fileMenu->addAction(saveAct);
	fileMenu->addAction(saveAsAct);
	fileMenu->addAction(saveReadable);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAct);

	editMenu = menuBar()->addMenu(tr("&Edit"));
	editMenu->addAction(undoAct);
	editMenu->addAction(redoAct);
	editMenu->addAction(saveSelectionReadable);
	editMenu->addSeparator();
	editMenu->addAction(findAct);
	editMenu->addAction(findNextAct);
	editMenu->addSeparator();
	editMenu->addAction(optionsAct);

	helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAct);
	helpMenu->addAction(aboutQtAct);
}

void HexEditor::createStatusBar()
{
	// Address Label
	lbAddressName = new QLabel();
	lbAddressName->setText(tr("Address:"));
	statusBar()->addPermanentWidget(lbAddressName);
	lbAddress = new QLabel();
	lbAddress->setFrameShape(QFrame::Panel);
	lbAddress->setFrameShadow(QFrame::Sunken);
	lbAddress->setMinimumWidth(70);
	statusBar()->addPermanentWidget(lbAddress);
	connect(hexEdit, SIGNAL(currentAddressChanged(int)), this, SLOT(setAddress(int)));

	// Size Label
	lbSizeName = new QLabel();
	lbSizeName->setText(tr("Size:"));
	statusBar()->addPermanentWidget(lbSizeName);
	lbSize = new QLabel();
	lbSize->setFrameShape(QFrame::Panel);
	lbSize->setFrameShadow(QFrame::Sunken);
	lbSize->setMinimumWidth(70);
	statusBar()->addPermanentWidget(lbSize);
	connect(hexEdit, SIGNAL(currentSizeChanged(int)), this, SLOT(setSize(int)));

	// Overwrite Mode Label
	lbOverwriteModeName = new QLabel();
	lbOverwriteModeName->setText(tr("Mode:"));
	statusBar()->addPermanentWidget(lbOverwriteModeName);
	lbOverwriteMode = new QLabel();
	lbOverwriteMode->setFrameShape(QFrame::Panel);
	lbOverwriteMode->setFrameShadow(QFrame::Sunken);
	lbOverwriteMode->setMinimumWidth(70);
	statusBar()->addPermanentWidget(lbOverwriteMode);
	setOverwriteMode(hexEdit->overwriteMode());

	statusBar()->showMessage(tr("Ready"));
}

void HexEditor::createToolBars()
{
	fileToolBar = addToolBar(tr("File"));
	fileToolBar->addAction(openAct);
	fileToolBar->addAction(saveAct);
}

void HexEditor::readSettings()
{
	QSettings settings;
	QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
	QSize size = settings.value("size", QSize(610, 460)).toSize();
	move(pos);
	resize(size);

	hexEdit->setAddressArea(settings.value("AddressArea").toBool());
	hexEdit->setAsciiArea(settings.value("AsciiArea").toBool());
	hexEdit->setHighlighting(settings.value("Highlighting").toBool());
	hexEdit->setOverwriteMode(settings.value("OverwriteMode").toBool());
	hexEdit->setReadOnly(settings.value("ReadOnly").toBool());

	hexEdit->setHighlightingColor(settings.value("HighlightingColor").value<QColor>());
	hexEdit->setAddressAreaColor(settings.value("AddressAreaColor").value<QColor>());
	hexEdit->setSelectionColor(settings.value("SelectionColor").value<QColor>());
	hexEdit->setFont(settings.value("WidgetFont").value<QFont>());

	hexEdit->setAddressWidth(settings.value("AddressAreaWidth").toInt());
}

void HexEditor::writeSettings()
{
	QSettings settings;
	settings.setValue("pos", pos());
	settings.setValue("size", size());
}

void HexEditor::optionsAccepted()
{
	writeSettings();
	readSettings();
}

*/
/*****************************************************************************/
/* Protected methods */
/*****************************************************************************/
/*void HexEditor::closeEvent(QCloseEvent *)
{
	writeSettings();
}
*/
