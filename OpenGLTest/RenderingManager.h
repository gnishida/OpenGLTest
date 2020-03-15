#pragma once

#include "Asset.h"
#include <QOpenGLFunctions>

class RenderingManager : protected QOpenGLFunctions
{
private:
	QOpenGLShaderProgram* program;
	std::map<const char*, Asset*> assets;

public:
	RenderingManager();
	~RenderingManager();

	void setViewport(int x, int y, int w, int h);
	void addObject(const char* filename, const std::vector<Vertex>& vertices);
	void render(const QMatrix4x4& cameraMatrix);
};
