#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QVector3D>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram);
QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	enum { PROGRAM_VERTEX_ATTRIBUTE = 0, PROGRAM_TEXCOORD_ATTRIBUTE = 1};

public:
	explicit GLWidget(QWidget *parent = 0);
	~GLWidget();

	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;
	void rotateBy(const QVector3D& rotationAngle);

protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int width, int height) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;

private:
	void makeObject();

	QPoint lastPos;
	QVector3D rotation;
	std::vector<QOpenGLTexture*> textures;
	QOpenGLShaderProgram *program;
	QOpenGLBuffer vbo;
};

#endif