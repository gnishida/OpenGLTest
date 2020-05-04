#include "MainWindow.h"

#include <QtWidgets>
#include <QtCore/QCoreApplication>

#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);
	
	glWidget.rotateBy(glm::vec3(42, 42, -21));
	this->setCentralWidget(&glWidget);

	setWindowTitle(tr("Textures"));

	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
}
