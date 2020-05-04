#include "GLWidget.h"
#include <QMouseEvent>
#include "AssetUtils.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

GLWidget::GLWidget(QWidget *parent)
{
	renderingManager = nullptr;
	eyePosition = glm::vec3(0, 0, 3);
}

GLWidget::~GLWidget()
{
	makeCurrent();
	if (renderingManager) {
		delete renderingManager;
		renderingManager = nullptr;
	}
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

void GLWidget::rotateBy(const glm::vec3& rotationAngle)
{
	rotation += rotationAngle;
	update();
}

glm::mat4 GLWidget::calculateMVPMatrix() {
	glm::mat4 model;

	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	glm::mat4 view = glm::lookAt(eyePosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 proj = glm::perspective(glm::radians(90.0f), (float)width() / height(), 0.1f, 10000.0f);

	return proj * view * model;
}

void GLWidget::initializeGL()
{
	renderingManager = new RenderingManager();
	makeObject();
}

void GLWidget::paintGL()
{
	glm::mat4 mvp = calculateMVPMatrix();
	renderingManager->render(mvp);
}

void GLWidget::resizeGL(int width, int height)
{
	renderingManager->setViewport(width, height);
}

void GLWidget::mousePressEvent(QMouseEvent* event)
{
	lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
	int dx = event->x() - lastPos.x();
	int dy = event->y() - lastPos.y();

	if (event->buttons() & Qt::LeftButton) {
		rotateBy(glm::vec3(dy, dx, 0));
	}
	else if (event->buttons() & Qt::RightButton) {
		rotateBy(glm::vec3(dy, 0, dx));
	}
	lastPos = event->pos();
}

void GLWidget::wheelEvent(QWheelEvent* event)
{
	eyePosition.z = eyePosition.z - event->delta() * 0.001;
	update();
}

void GLWidget::makeObject()
{
	renderingManager->addObject("images/earth.jpg", AssetUtils::createRectangle(0.5, 0.5));
	//renderingManager->addObject("images/side2.png", AssetUtils::createSphere(0.35));
}