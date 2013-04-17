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
#include <QtCore/QThread>

#include <iostream>
#include <streambuf>
#include <string>

#include <QtGui/QTextEdit>
#include <QtGui/QApplication>
#include <QtGui/QMainWindow>
#include <QtGui/QVBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QProgressBar>
#include <QtCore/QTimer>
#include <QtCore/QObject>
#include <QtGui/QWidget>
#include "qdebugstream.h"

class MyThread : public QThread
{
	Q_OBJECT

	Algorithm* algorithm;

protected:
	void run() {algorithm->schedule();}
public:
	MyThread(Algorithm* alg) {algorithm = alg;}
};

class OurMainWindow : public QMainWindow
{
	Q_OBJECT

	Algorithm* algorithm;
	XMLConverter* converter;
	QString outputName;
	QPushButton* ok;
	QTextEdit* myTextEdit;
	QDebugStream *qout, *qout2;
public:
	OurMainWindow(int argc, char** argv) : QMainWindow()
	{
		QVBoxLayout* layout = new QVBoxLayout(centralWidget());
		
		myTextEdit = new QTextEdit(this);
		ok = new QPushButton("Close", this);
		ok->setEnabled(false);
		layout->addWidget(myTextEdit);
		layout->addWidget(ok);
		setCentralWidget(new QWidget());
		centralWidget()->setLayout(layout);
		qout = new QDebugStream(std::cout, myTextEdit);
		qout2 = new QDebugStream(std::cerr, myTextEdit);

		std::cout << "Starting algorithm" << endl;
		QString inputName = QString(argv[1]);
		outputName = argc > 2 ? QString(argv[2]) : inputName;
		QString algorithmType = argc > 3 ? QString(argv[3]) : QString("d");
    
		QString input;
		{
			QFile inputFile(inputName);
			inputFile.open(QIODevice::ReadOnly | QIODevice::Text);
			QTextStream inputStream(&inputFile);
			input = inputStream.readAll();
			inputFile.close();
		}

		converter = new XMLConverter(input);
		Network * network = converter->getNetwork();
		Requests requests = converter->getRequests();

		algorithm = AlgorithmDispatcher::Dispatch(algorithmType, network, requests); 
		QTimer* timer = new QTimer();
		timer->setInterval(1000);
		timer->setSingleShot(true);
		timer->start();
		QObject::connect(timer, SIGNAL(timeout()), SLOT(run()));
		QObject::connect(ok, SIGNAL(clicked()), SLOT(close()));
		show();
	}

public slots:
	void run()
	{
		MyThread* thread = new MyThread(algorithm);
		QObject::connect(thread, SIGNAL(finished()), SLOT(finish()));
		thread->start();
	}

	void finish()
	{
		Assignments assignments = algorithm->getAssignments();
		converter->setAssignments(assignments);

		{
			QFile outputFile(outputName);
			outputFile.open(QIODevice::WriteOnly | QIODevice::Text);
			QTextStream outputStream(&outputFile);
			outputStream << converter->getMixdownContent();
			outputFile.close();
		}
		delete converter;
		delete algorithm;
		ok->setEnabled(true);
	}
};
