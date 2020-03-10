#include "MainWindow.h"

#include <QtWidgets>
#include <QtCore/QCoreApplication>

#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);
	
	glWidgets = new GLWidget;
	glWidgets->setClearColor(QColor(0, 255, 0));
	glWidgets->rotateBy(+42 * 16, +42 * 16, -21 * 16);

	this->setCentralWidget(glWidgets);

	setWindowTitle(tr("Textures"));

	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
}
