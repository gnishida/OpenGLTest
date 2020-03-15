#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "RenderingManager.h"

#include <QOpenGLWidget>
#include <QVector3D>

#include <glm/glm.hpp>

class GLWidget : public QOpenGLWidget
{
	Q_OBJECT

private:
	QPoint lastPos;
	QVector3D rotation;
	RenderingManager* renderingManager;

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
};

#endif