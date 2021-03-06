#include "AssetUtils.h"

#include <algorithm>
#include <limits>

std::vector<Vertex> AssetUtils::createSphere(float x, float y, float z, float radius) {
	std::vector<Vertex> vertices;

	const int HOR_SLICES = 64;
	const int VERT_SLICES = 32;

	const float M_PI = atan(1) * 4;
	const float r = radius;
	for (int j = 0; j < VERT_SLICES; j++) {
		const float nz1 = sin(j * M_PI / VERT_SLICES - M_PI / 2);
		const float nz2 = sin((j + 1) * M_PI / VERT_SLICES - M_PI / 2);
		const float z1_coord = z + r * nz1;
		const float z2_coord = z + r * nz2;

		const float new_r = r * cos(j * M_PI / VERT_SLICES - M_PI / 2);
		const float new_r2 = r * cos((j + 1) * M_PI / VERT_SLICES - M_PI / 2);
		for (int i = 0; i < HOR_SLICES; i++) {
			const float nx1 = cos(i * 2 * M_PI / HOR_SLICES);
			const float ny1 = sin(i * 2 * M_PI / HOR_SLICES);
			const float nx2 = cos((i + 1) * 2 * M_PI / HOR_SLICES);
			const float ny2 = sin((i + 1) * 2 * M_PI / HOR_SLICES);
			const float nx3 = cos((i + 1) * 2 * M_PI / HOR_SLICES);
			const float ny3 = sin((i + 1) * 2 * M_PI / HOR_SLICES);
			const float nx4 = cos(i * 2 * M_PI / HOR_SLICES);
			const float ny4 = sin(i * 2 * M_PI / HOR_SLICES);

			const float x1_coord = x + new_r * nx1;
			const float y1_coord = y + new_r * ny1;
			const float x2_coord = x + new_r * nx2;
			const float y2_coord = y + new_r * ny2;
			const float x3_coord = x + new_r2 * nx3;
			const float y3_coord = y + new_r2 * ny3;
			const float x4_coord = x + new_r2 * nx4;
			const float y4_coord = y + new_r2 * ny4;

			vertices.push_back({ x1_coord, y1_coord, z1_coord, nx1, ny1, nz1, (float)i / HOR_SLICES, (float)j / VERT_SLICES });
			vertices.push_back({ x2_coord, y2_coord, z1_coord, nx2, ny2, nz1, (float)(i + 1) / HOR_SLICES, (float)j / VERT_SLICES });
			vertices.push_back({ x3_coord, y3_coord, z2_coord, nx3, ny3, nz2, (float)(i + 1) / HOR_SLICES, (float)(j + 1) / VERT_SLICES });

			vertices.push_back({ x1_coord, y1_coord, z1_coord, nx1, ny1, nz1, (float)i / HOR_SLICES, (float)j / VERT_SLICES });
			vertices.push_back({ x3_coord, y3_coord, z2_coord, nx3, ny3, nz2, (float)(i + 1) / HOR_SLICES, (float)(j + 1) / VERT_SLICES });
			vertices.push_back({ x4_coord, y4_coord, z2_coord, nx4, ny4, nz2, (float)i / HOR_SLICES, (float)(j + 1) / VERT_SLICES });
		}
	}

	return vertices;
}

std::vector<Vertex> AssetUtils::createPrism(const std::vector<glm::vec2>& polygon, float baseHeight, float height) {
	std::vector<Vertex> vertices;
	const int NUM_VERTEX = polygon.size();

	for (int i = 0; i < NUM_VERTEX; i++) {
		const float& x1 = polygon[i].x;
		const float& y1 = polygon[i].y;
		const int next = (i + 1) % NUM_VERTEX;
		const float& x2 = polygon[next].x;
		const float& y2 = polygon[next].y;
		const float length = glm::length(glm::vec2(x2 - x1, y2 - y1));

		const float nx = (y2 - y1) / length;
		const float ny = (x1 - x2) / length;

		vertices.push_back({ x1, y1, baseHeight, nx, ny, 0, 0, 0 });
		vertices.push_back({ x2, y2, baseHeight, nx, ny, 0, length / 11.0f, 0 });
		vertices.push_back({ x2, y2, baseHeight + height, nx, ny, 0, length / 11.0f, height / 10.0f });

		vertices.push_back({ x1, y1, baseHeight, nx, ny, 0, 0, 0 });
		vertices.push_back({ x2, y2, baseHeight + height, nx, ny, 0, length / 11.0f, height / 10.0f });
		vertices.push_back({ x1, y1, baseHeight + height, nx, ny, 0, 0, height / 10.0f });
	}

	return vertices;
}

std::vector<Vertex> AssetUtils::createRectangle(float width, float length, float height) {
	std::vector<Vertex> vertices;
	const float x1 = -width / 2;
	const float x2 = width / 2;
	const float y1 = -length / 2;
	const float y2 = length / 2;
	vertices.push_back({ x1, y1, height, 0, 0, 1, 0, 0 });
	vertices.push_back({ x2, y1, height, 0, 0, 1, 1, 0 });
	vertices.push_back({ x2, y2, height, 0, 0, 1, 1, 1 });

	vertices.push_back({ x1, y1, height, 0, 0, 1, 0, 0 });
	vertices.push_back({ x2, y2, height, 0, 0, 1, 1, 1 });
	vertices.push_back({ x1, y2, height, 0, 0, 1, 0, 1 });

	return vertices;
}

std::vector<Vertex> AssetUtils::createConvexPolygon(const std::vector<glm::vec2>& polygon, float height) {
	if (polygon.size() < 3) return {};

	std::vector<Vertex> vertices;

	const float& x0 = polygon[0].x;
	const float& y0 = polygon[0].y;

	for (int i = 0; i < polygon.size(); i++) {
		const float& x1 = polygon[i].x;
		const float& y1 = polygon[i].y;
		const int next = (i + 1) % polygon.size();
		const float& x2 = polygon[next].x;
		const float& y2 = polygon[next].y;

		vertices.push_back({ x0, y0, height, 0, 0, 1, 0, 0 });
		vertices.push_back({ x1, y1, height, 0, 0, 1, 1, 0.2 });
		vertices.push_back({ x2, y2, height, 0, 0, 1, 0.4, 1 });
	}

	return vertices;
}

std::vector<Vertex> AssetUtils::createConvexPolygon(const std::vector<glm::vec2>& polygon, float height, float minX, float minY, float maxX, float maxY) {
	if (polygon.size() < 3) return {};

	std::vector<Vertex> vertices;

	const float& x0 = polygon[0].x;
	const float& y0 = polygon[0].y;

	float u0 = (polygon[0].x - minX) / (maxX - minX);
	float v0 = (polygon[0].y - minY) / (maxY - minY);

	for (int i = 0; i < polygon.size(); i++) {
		const float& x1 = polygon[i].x;
		const float& y1 = polygon[i].y;
		float u1 = (x1 - minX) / (maxX - minX);
		float v1 = (y1 - minY) / (maxY - minY);

		const int next = (i + 1) % polygon.size();
		const float& x2 = polygon[next].x;
		const float& y2 = polygon[next].y;
		float u2 = (x2 - minX) / (maxX - minX);
		float v2 = (y2 - minY) / (maxY - minY);

		vertices.push_back({ x0, y0, height, 0, 0, 1, u0, v0 });
		vertices.push_back({ x1, y1, height, 0, 0, 1, u1, v1 });
		vertices.push_back({ x2, y2, height, 0, 0, 1, u2, v2 });
	}

	return vertices;
}
