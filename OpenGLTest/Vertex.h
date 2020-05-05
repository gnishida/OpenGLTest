#pragma once

struct Vertex
{
	float x;
	float y;
	float z;
	float nx;
	float ny;
	float nz;
	float u;
	float v;

	Vertex(float x, float y, float z, float nx, float ny, float nz, float u, float v) : x(x), y(y), z(z), nx(nx), ny(ny), nz(nz), u(u), v(v) {}
};

