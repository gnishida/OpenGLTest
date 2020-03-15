#include "GLWidget.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMouseEvent>
#include <iostream>

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent), program(0)
{
}

GLWidget::~GLWidget()
{
	makeCurrent();
	for (int i = 0; i < assets.size(); i++) {
		delete assets[i];
	}
	assets.clear();
	delete program;
	doneCurrent();
}

QSize GLWidget::minimumSizeHint() const
{
	return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
	return QSize(200, 200);
}

void GLWidget::rotateBy(const QVector3D& rotationAngle)
{
	rotation += rotationAngle;
	update();
}

void GLWidget::initializeGL()
{
	initializeOpenGLFunctions();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	program = new QOpenGLShaderProgram;
	program->addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/vert.glsl");
	program->addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/frag.glsl");
	program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
	program->bindAttributeLocation("texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);
	program->link();

	program->bind();
	program->setUniformValue("texture", 0);

	makeObject();
}

void GLWidget::paintGL()
{
	glClearColor(0, 0.1, 0.1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	QMatrix4x4 m;
	m.ortho(-0.5f, +0.5f, +0.5f, -0.5f, 4.0f, 15.0f);
	m.translate(0.0f, 0.0f, -10.0f);
	m.rotate(rotation.x(), 1.0f, 0.0f, 0.0f);
	m.rotate(rotation.y(), 0.0f, 1.0f, 0.0f);
	m.rotate(rotation.z(), 0.0f, 0.0f, 1.0f);
	program->setUniformValue("matrix", m);

	for (int i = 0; i < assets.size(); i++) {
		assets[i]->createArrays();
		assets[i]->bind();
		glDrawArrays(GL_TRIANGLES, 0, assets[i]->numVertices());
	}
}

void GLWidget::resizeGL(int width, int height)
{
	int side = qMin(width, height);
	glViewport((width - side) / 2, (height - side) / 2, side, side);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
	lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	int dx = event->x() - lastPos.x();
	int dy = event->y() - lastPos.y();

	if (event->buttons() & Qt::LeftButton) {
		rotateBy(QVector3D(dy, dx, 0));
	}
	else if (event->buttons() & Qt::RightButton) {
		rotateBy(QVector3D(dy, 0, dx));
	}
	lastPos = event->pos();
}

void GLWidget::makeObject()
{
	assets.resize(6);

	static const GLfloat coords[6][4][3] = {
		{ { 0.2f, -0.2f, -0.2f }, { -0.2f, -0.2f, -0.2f }, { -0.2f, 0.2f, -0.2f }, { 0.2f, 0.2f, -0.2f } },
		{ { 0.2f, 0.2f, -0.2f }, { -0.2f, 0.2f, -0.2f }, { -0.2f, 0.2f, 0.2f }, { 0.2f, 0.2f, 0.2f } },
		{ { 0.2f, -0.2f, 0.2f }, { 0.2f, -0.2f, -0.2f }, { 0.2f, 0.2f, -0.2f }, { 0.2f, 0.2f, 0.2f } },
		{ { -0.2f, -0.2f, -0.2f }, { -0.2f, -0.2f, 0.2f }, { -0.2f, 0.2f, 0.2f }, { -0.2f, 0.2f, -0.2f } },
		{ { 0.2f, -0.2f, 0.2f }, { -0.2f, -0.2f, 0.2f }, { -0.2f, -0.2f, -0.2f }, { 0.2f, -0.2f, -0.2f } },
		{ { -0.2f, -0.2f, 0.2f }, { 0.2f, -0.2f, 0.2f }, { 0.2f, 0.2f, 0.2f }, { -0.2f, 0.2f, 0.2f } }
	};

	for (int i = 0; i < 6; i++) {
		assets[i] = new Asset(program);
		assets[i]->setTexture(QString("images/side%1.png").arg(i + 1).toUtf8().constData());

		assets[i]->addVertex(coords[i][0][0], coords[i][0][1], coords[i][0][2], 0, 0);
		assets[i]->addVertex(coords[i][1][0], coords[i][1][1], coords[i][1][2], 1, 0);
		assets[i]->addVertex(coords[i][2][0], coords[i][2][1], coords[i][2][2], 1, 1);

		assets[i]->addVertex(coords[i][0][0], coords[i][0][1], coords[i][0][2], 0, 0);
		assets[i]->addVertex(coords[i][2][0], coords[i][2][1], coords[i][2][2], 1, 1);
		assets[i]->addVertex(coords[i][3][0], coords[i][3][1], coords[i][3][2], 0, 1);
	}
}