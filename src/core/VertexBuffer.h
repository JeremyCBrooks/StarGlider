#ifndef VertexBuffer_H_
#define VertexBuffer_H_

#include <vector>

class VertexBuffer
{
public:
	VertexBuffer(const std::vector<unsigned short>& indices, const std::vector<float>& vertices);
	~VertexBuffer();

	void Update(const std::vector<float>& vertices);
	void Use()const;
	void Setup(int attribLoc, int numElems, int type, int stride, int offset);
	
	void Draw(int numIndices, int offset = 0)const;//draw subset
	void Draw()const;//draw all

	int GetNumIndices()const{ return numIndices; }

	static void Disable(int=-1,int=-1,int=-1,int=-1);

protected:
	int numIndices;
	unsigned int VBOVertices,VBOIndices;
};

#endif
