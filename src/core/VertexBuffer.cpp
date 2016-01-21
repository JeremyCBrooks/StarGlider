/*
The MIT License (MIT)

Copyright (c) 2014 Jeremy C Brooks
http://vertexarray.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include "VertexBuffer.h"
#include "platform.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

VertexBuffer::VertexBuffer(const std::vector<unsigned short>& indices, const std::vector<float>& vertices) : 
	numIndices(0),
	VBOVertices(0),
	VBOIndices(0)
{
	//generate new buffer objects
	glGenBuffers(1, &VBOVertices);
	glBindBuffer(GL_ARRAY_BUFFER, VBOVertices);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &VBOIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

	//disable buffers, for now...
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	numIndices = (int)indices.size();
}

VertexBuffer::~VertexBuffer()
{
	//clean up any previously created buffers
	if(VBOVertices>0){
		glDeleteBuffers(1, &VBOVertices);
	}
	checkGLError();

	if(VBOIndices>0){
		glDeleteBuffers(1, &VBOIndices);
	}
	checkGLError();

	VBOVertices = 0;
	VBOIndices = 0;
	numIndices = 0;
}

void VertexBuffer::Update(const std::vector<float>& vertices)
{
	//generate new buffer objects
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), NULL, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), &vertices[0], GL_STATIC_DRAW);
}

void VertexBuffer::Use()const
{
	glBindBuffer(GL_ARRAY_BUFFER, VBOVertices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOIndices);
}

void VertexBuffer::Setup(int attribLoc, int numElems, int type, int stride, int offset)
{
	glEnableVertexAttribArray(attribLoc);
	glVertexAttribPointer(attribLoc, numElems, type, GL_FALSE, stride, BUFFER_OFFSET(offset));
}

void VertexBuffer::Draw(int ni, int offset)const
{
	glDrawElements(GL_TRIANGLES, ni, GL_UNSIGNED_SHORT,  BUFFER_OFFSET(offset));
}

void VertexBuffer::Draw()const
{
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT,  BUFFER_OFFSET(0));
}

void VertexBuffer::Disable(int attribLoc0, int attribLoc1, int attribLoc2, int attribLoc3)
{
	//unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	if(attribLoc0>-1){
		glDisableVertexAttribArray(attribLoc0);
	}
	if(attribLoc1>-1){
		glDisableVertexAttribArray(attribLoc1);
	}
	if(attribLoc2>-1){
		glDisableVertexAttribArray(attribLoc2);
	}
	if(attribLoc3>-1){
		glDisableVertexAttribArray(attribLoc3);
	}
}