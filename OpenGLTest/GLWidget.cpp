#include "GLWidget.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMouseEvent>

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent), program(0)
{
}

GLWidget::~GLWidget()
{
	makeCurrent();
	vbo.destroy();
	for (int i = 0; i < textures.size(); ++i) {
		delete textures[i];
	}
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

	makeObject();

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
}

void GLWidget::paintGL()
{
	glClearColor(0, 1, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	QMatrix4x4 m;
	m.ortho(-0.5f, +0.5f, +0.5f, -0.5f, 4.0f, 15.0f);
	m.translate(0.0f, 0.0f, -10.0f);
	m.rotate(rotation.x(), 1.0f, 0.0f, 0.0f);
	m.rotate(rotation.y(), 0.0f, 1.0f, 0.0f);
	m.rotate(rotation.z(), 0.0f, 0.0f, 1.0f);

	program->setUniformValue("matrix", m);
	program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
	program->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
	program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
	program->setAttributeBuffer(PROGRAM_TEXCOORD_ATTRIBUTE, GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));

	for (int i = 0; i < 6; ++i) {
		textures[i]->bind();
		glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
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
	static const int coords[6][4][3] = {
		{ { +1, -1, -1 }, { -1, -1, -1 }, { -1, +1, -1 }, { +1, +1, -1 } },
		{ { +1, +1, -1 }, { -1, +1, -1 }, { -1, +1, +1 }, { +1, +1, +1 } },
		{ { +1, -1, +1 }, { +1, -1, -1 }, { +1, +1, -1 }, { +1, +1, +1 } },
		{ { -1, -1, -1 }, { -1, -1, +1 }, { -1, +1, +1 }, { -1, +1, -1 } },
		{ { +1, -1, +1 }, { -1, -1, +1 }, { -1, -1, -1 }, { +1, -1, -1 } },
		{ { -1, -1, +1 }, { +1, -1, +1 }, { +1, +1, +1 }, { -1, +1, +1 } }
	};

	for (int j = 0; j < 6; ++j) {
		textures.push_back(new QOpenGLTexture(QImage(QString("images/side%1.png").arg(j + 1)).mirrored()));
	}

	QVector<GLfloat> vertData;
	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 4; ++j) {
			// vertex position
			vertData.append(0.2 * coords[i][j][0]);
			vertData.append(0.2 * coords[i][j][1]);
			vertData.append(0.2 * coords[i][j][2]);
			// texture coordinate
			vertData.append(j == 0 || j == 3);
			vertData.append(j == 0 || j == 1);
		}
	}

	vbo.create();
	vbo.bind();
	vbo.allocate(vertData.constData(), vertData.count() * sizeof(GLfloat));
}