#include "MainWindow.h"

#include <QtWidgets>
#include <QtCore/QCoreApplication>

#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);
	
	QGridLayout *mainLayout = new QGridLayout;

	for (int i = 0; i < NumRows; ++i) {
		for (int j = 0; j < NumColumns; ++j) {
			QColor clearColor;
			clearColor.setHsv(((i * NumColumns) + j) * 255
				/ (NumRows * NumColumns - 1),
				255, 63);

			glWidgets[i][j] = new GLWidget;
			glWidgets[i][j]->setClearColor(clearColor);
			glWidgets[i][j]->rotateBy(+42 * 16, +42 * 16, -21 * 16);
			mainLayout->addWidget(glWidgets[i][j], i, j);


		}
	}
	QWidget *widget = new QWidget();
	widget->setLayout(mainLayout);
	this->setCentralWidget(widget);

	currentGlWidget = glWidgets[0][0];



	setWindowTitle(tr("Textures"));
}
