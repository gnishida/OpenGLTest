#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "RenderingManager.h"

#include <QOpenGLWidget>
#include <glm/glm.hpp>

class GLWidget : public QOpenGLWidget
{
	Q_OBJECT

private:
	QPoint lastPos;
	glm::vec3 eyePosition;
	glm::vec3 rotation;
	RenderingManager* renderingManager;

public:
	explicit GLWidget(QWidget *parent = 0);
	~GLWidget();

	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;
	void rotateBy(const glm::vec3& rotationAngle);
	glm::mat4 calculateMVPMatrix();

protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int width, int height) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;

private:
	void makeObject();
};

#endif