#include "xmlconverter.h"
#include "algorithmdispatcher.h"

#include "algorithm.h"

#include <iostream>
using std::cerr;
using std::endl;

#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QTextStream>

#include <iostream>
#include <streambuf>
#include <string>

#include <QtGui/QTextEdit>
#include <QtGui/QApplication>
#include <QtGui/QMainWindow>

class QDebugStream : public std::basic_streambuf<char>
{
public:
 QDebugStream(std::ostream &stream, QTextEdit* text_edit) : m_stream(stream)
 {
  log_window = text_edit;
  m_old_buf = stream.rdbuf();
  stream.rdbuf(this);
 }
 ~QDebugStream()
 {
  // output anything that is left
  if (!m_string.empty())
   log_window->append(m_string.c_str());

  m_stream.rdbuf(m_old_buf);
 }

protected:
 virtual int_type overflow(int_type v)
 {
  if (v == '\n')
  {
   log_window->append(m_string.c_str());
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
    log_window->append(tmp.c_str());
    m_string.erase(m_string.begin(), m_string.begin() + pos + 1);
   }
  }

  return n;
 }

private:
 std::ostream &m_stream;
 std::streambuf *m_old_buf;
 std::string m_string;
 QTextEdit* log_window;
};

int main(int argc, char ** argv)
{
    if ( argc < 2 || argc > 4 )
    {
        cerr << "Usage: " << argv[0] << " <input_file> [output_file = input_file] [algorithm = [a|c|d]]" << endl;
        return 1;
    }

	QApplication app(argc, argv);
	QMainWindow* mw = new QMainWindow();
	QTextEdit* myTextEdit = new QTextEdit(mw);
	QDebugStream qout(std::cout, myTextEdit);
	QDebugStream qout2(std::cerr, myTextEdit);
	mw->show();

	std::cout << "Starting algorithm" << endl;

    QString inputName = QString(argv[1]);
    QString outputName = argc > 2 ? QString(argv[2]) : inputName;
    QString algorithmType = argc > 3 ? QString(argv[3]) : QString("d");
    
    QString input;
    {
        QFile inputFile(inputName);
        inputFile.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream inputStream(&inputFile);
        input = inputStream.readAll();
        inputFile.close();
    }

    XMLConverter converter(input);
    Network * network = converter.getNetwork();
    Requests requests = converter.getRequests();

    Algorithm * algorithm = AlgorithmDispatcher::Dispatch(algorithmType, network, requests); 
    algorithm->schedule();

    Assignments assignments = algorithm->getAssignments();
    converter.setAssignments(assignments);

    {
        QFile outputFile(outputName);
        outputFile.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream outputStream(&outputFile);
        outputStream << converter.getMixdownContent();
        outputFile.close();
    }
    
    
    delete algorithm;
	return app.exec();
}
