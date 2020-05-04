#pragma once

#include "Asset.h"
#include <QOpenGLFunctions>
#include <glm/glm.hpp>

class RenderingManager : protected QOpenGLFunctions
{
private:
	QOpenGLShaderProgram* program;
	std::map<const char*, Asset*> assets;

public:
	RenderingManager();
	~RenderingManager();

	void setViewport(int width, int height);
	void addObject(const char* filename, const std::vector<Vertex>& vertices);
	void removeObjects();
	void render(const glm::mat4& cameraMatrix);
};
