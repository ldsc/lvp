#include "textEditor.h"

using namespace std;

TextEditor::TextEditor( QMainWindow * _parent ) : QTextEdit(_parent) {
	parent = _parent;
	setAttribute(Qt::WA_DeleteOnClose);
	setBackgroundRole(QPalette::Base);
    item = NULL;
    connect(this->document(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()));
	connect( this, SIGNAL( destroyed() ), parent, SLOT( updateDockLista() ) );
}

TextEditor:: ~TextEditor() {
	//	cerr << "destrutor" << endl;
}

bool TextEditor::loadFile(const QString & _fileName)
{
	if ( ! _fileName.isEmpty() ) {
		getFileNames ( _fileName );
        //setWindowTitle ( fileName );
        QFile file( fullFileName );
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("Application"), tr("Cannot read file %1:\n%2.").arg(fullFileName).arg(file.errorString()));
            return false;
        }
        QTextStream in(&file);
        QApplication::setOverrideCursor(Qt::WaitCursor);
        this->setPlainText(in.readAll());
        setWindowModified(false);
        QApplication::restoreOverrideCursor();
        return true;
	} else {
		return false;
	}
}

void TextEditor::getFileNames(const QString & _fileName)
{
	fullFileName 	= _fileName;
	fileName 		= QFileInfo(fullFileName).fileName(); 	// retorna a string existente após a última /
	curFileNoExt	= fileName.section('.', 0, -2); 		// retorna o nome do arquivo sem a extensão
	fileExt 		= QFileInfo(fullFileName).suffix(); 	// retorna a string existente após o último .
}

bool TextEditor::save( ) {
    return saveFile(fullFileName);

}

bool TextEditor::saveAs( ) {
	QString newFileName = QFileDialog::getSaveFileName(this, tr("Save As"), QFileInfo(fullFileName).path(), tr("All Files (*)"));
	if ( newFileName.isEmpty() ) {
		return false;
	}
    return saveFile(newFileName);
}

bool TextEditor::saveFile(const QString & _fileName)
{
    QFile file(_fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr(".: LVP"), tr("Cannot write file %1:\n%2.").arg(_fileName).arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << this->toPlainText();
    QApplication::restoreOverrideCursor();

    getFileNames(_fileName);
    this->document()->setModified(false);
    setWindowModified(false);
    return true;
}

void TextEditor::closeEvent(QCloseEvent *event) {
	if (maybeSave()) {
		event->accept();
	} else {
		event->ignore();
	}
}

bool TextEditor::maybeSave() {
    if (this->document()->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr(".: LVP"), tr("The document has been modified.\nDo you want to save your changes?"), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void TextEditor::documentWasModified() {
    updateTitle();
    setWindowModified(this->document()->isModified());
}

void TextEditor::updateTitle(){
    setWindowTitle(tr("%1[*]").arg(fileName));
}

