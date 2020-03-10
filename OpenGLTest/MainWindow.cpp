#include "MainWindow.h"

#include <QtWidgets>
#include <QtCore/QCoreApplication>

#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);
	
	glWidget.setClearColor(QColor(0, 255, 0));
	glWidget.rotateBy(+42 * 16, +42 * 16, -21 * 16);
	this->setCentralWidget(&glWidget);

	setWindowTitle(tr("Textures"));

	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
}
