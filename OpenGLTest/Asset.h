#pragma once

#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

struct Vertex
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat u;
	GLfloat v;

	Vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v) : x(x), y(y), z(z), u(u), v(v) {}
};

class Asset
{
public:
	enum { PROGRAM_VERTEX_ATTRIBUTE = 0, PROGRAM_TEXCOORD_ATTRIBUTE = 1 };

private:
	QOpenGLShaderProgram *program;
	QOpenGLVertexArrayObject* vao;
	QOpenGLBuffer vbo;
	QOpenGLTexture* texture;
	std::vector<Vertex> vertices;
	bool dirty;

public:
	Asset(QOpenGLShaderProgram* program);
	~Asset();

	void setTexture(const char* filename);
	void createArrays();
	void bind();
	void release();
	void addVertex(GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v);
	int numVertices() { return vertices.size(); }
};