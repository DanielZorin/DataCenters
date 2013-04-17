#ifndef qdebugstream_h
#define qdebugstream_h

#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QTextStream>
#include <QtGui/QTextCursor>
#include <QtCore/QMetaType>

#include <iostream>
#include <fstream>
#include <streambuf>
#include <string>

class QDebugStream : public std::basic_streambuf<char>
{
public:
 QDebugStream(std::ostream &stream, QTextEdit* text_edit, const char* logfile = "out.log") : m_stream(stream), fstream(logfile)
 {
  log_window = text_edit;
  m_old_buf = stream.rdbuf();
  stream.rdbuf(this);
 }
 ~QDebugStream()
 {
  // output anything that is left
  if (!m_string.empty()) {
   fstream << m_string << std::endl;
   QMetaObject::invokeMethod(log_window, "append", Qt::QueuedConnection, Q_ARG(QString, m_string.c_str()));
  }

  m_stream.rdbuf(m_old_buf);
  fstream.close();
 }

protected:
 virtual int_type overflow(int_type v)
 {
  if (v == '\n')
  {
   fstream << m_string << std::endl;
   QMetaObject::invokeMethod(log_window, "append", Qt::QueuedConnection, Q_ARG(QString, m_string.c_str()));
   m_string.erase(m_string.begin(), m_string.end());
  }
  else
   m_string += v;

  return v;
 }

 virtual std::streamsize xsputn(const char *p, std::streamsize n)
 {
  m_string.append(p, p + n);

  int pos = 0;
  while (pos != std::string::npos)
  {
   pos = m_string.find('\n');
   if (pos != std::string::npos)
   {
    std::string tmp(m_string.begin(), m_string.begin() + pos);
    fstream << tmp << std::endl;
    QMetaObject::invokeMethod(log_window, "append", Qt::QueuedConnection, Q_ARG(QString, tmp.c_str()));
    m_string.erase(m_string.begin(), m_string.begin() + pos + 1);
   }
  }

  return n;
 }

private:
 std::ofstream fstream;
 std::ostream &m_stream;
 std::streambuf *m_old_buf;
 std::string m_string;
 QTextEdit* log_window;
};
#endif
