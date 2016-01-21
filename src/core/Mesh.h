#ifndef MESH_H_
#define MESH_H_

#include <vector>
#include <memory>

class VertexBuffer;

class Mesh
{
public:
	Mesh(const std::vector<unsigned short>& indices, const std::vector<float>& vertices);
	~Mesh();

	void setAttributeLocations(int positionLoc, int normalLoc, int texCoordLoc, int colorLoc);
	void updateVertices(const std::vector<float>& v);
	void Display()const;

	static Mesh* createColoredQuad(float left, float top, float right, float bottom, float* color);

	float left, top, right, bottom;

	float width, height;

private:
	std::unique_ptr<VertexBuffer> vbo;

	int positionLoc, normalLoc, texCoordLoc, colorLoc;
};
#endif