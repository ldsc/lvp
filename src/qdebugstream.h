#ifndef QDEBUGSTREAM_H
#define QDEBUGSTREAM_H
#include <iostream>
#include <streambuf>
#include <string>

//#include "qtextedit.h"
#include <QPlainTextEdit>

class QDebugStream : public std::basic_streambuf<char>
{
	public:
		QDebugStream(std::ostream &stream, QPlainTextEdit* text_edit) : m_stream(stream) {
			log_window = text_edit;
			m_old_buf = stream.rdbuf();
			stream.rdbuf(this);
		}

		~QDebugStream() {
			// output anything that is left
			if (!m_string.empty())
				//log_window->appendPlainText(m_string.c_str());
				log_window->appendPlainText(QString::fromUtf8(m_string.c_str()));
			m_stream.rdbuf(m_old_buf);
		}

	protected:
		virtual int_type overflow(int_type v) {
			if (v == '\n') {
				//log_window->appendPlainText(m_string.c_str());
				log_window->appendPlainText(QString::fromUtf8(m_string.c_str()));
				m_string.erase(m_string.begin(), m_string.end());
			} else {
				m_string += v;
			}
			return v;
		}

		virtual std::streamsize xsputn(const char *p, std::streamsize n) {
			m_string.append(p, p + n);
			// int pos = 0;
			// unsigned pos = 0;  // avoid conversion warnings
			size_t pos = 0;  // patch to avoid 64-bit conversion error
			while (pos != std::string::npos) {
				pos = m_string.find('\n');
				if (pos != std::string::npos) {
					std::string tmp(m_string.begin(), m_string.begin() + pos);
					//log_window->appendPlainText(tmp.c_str());
					log_window->appendPlainText(QString::fromUtf8(tmp.c_str()));
					m_string.erase(m_string.begin(), m_string.begin() + pos + 1);
				}
			}
			return n;
		}

	private:
		std::ostream &m_stream;
		std::streambuf *m_old_buf;
		std::string m_string;
		//QTextEdit* log_window;
		QPlainTextEdit* log_window;
};

#endif // QDEBUGSTREAM_H
