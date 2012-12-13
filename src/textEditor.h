#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QtGui>
#include <QTextEdit>
#include <QFile>
#include <iostream>
#include <fstream>

class QMainWindow;
class QListWidgetItem;

/*! TextEditor é a classe que implementa o editor de arquivos texto. */
class TextEditor : public QTextEdit
{
		Q_OBJECT
		// Construtor/Destrutor
	public:
		TextEditor( QMainWindow * _parent );
		virtual ~TextEditor();

		// Atributos
	public:
		QListWidgetItem *item;

	private:
		QMainWindow *parent;
		QString fullFileName;
		QString fileName;
		QString curFileNoExt;
		QString fileExt;

		// Métodos
	public:
		bool loadFile( const QString & _fileName );
		bool save();
		bool saveAs();

		inline QString getFullFileName() { return fullFileName; }
		inline QString getFileName() { return fileName; }
		inline QString getFileNameNoExt() { return curFileNoExt;	}
		inline QString getFileExt() { return fileExt; }

	private slots:
		void documentWasModified();

	protected:
		void closeEvent(QCloseEvent *event);

	private:
		bool maybeSave();
		void getFileNames( const QString & _fileName );
		bool saveFile(const QString & _fileName);
		void updateTitle();
};

#endif
