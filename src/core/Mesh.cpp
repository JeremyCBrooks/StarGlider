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
#include "Mesh.h"
#include "VertexBuffer.h"
#include "platform.h"
#include <string.h>//for memcpy
#include "VectorMath.h"

Mesh::Mesh(const std::vector<unsigned short>& indices, const std::vector<float>& vertices) :
	positionLoc(-1),
	normalLoc(-1), 
	texCoordLoc(-1), 
	colorLoc(-1)
{
	this->left = vertices[0];
	this->top = vertices[1];
	this->right = vertices[18];
	this->bottom = vertices[19];

	this->width = this->right - this->left;
	this->height = this->bottom - this->top;

	vbo.reset(new VertexBuffer(indices, vertices));
}

Mesh::~Mesh()
{
}

void Mesh::updateVertices(const std::vector<float>& v)
{
	vbo->Use();
	vbo->Update(v);
	VertexBuffer::Disable();
}

//pass -1 to disable an attribute
void Mesh::setAttributeLocations(int _positionLoc, int _normalLoc, int _texCoordLoc, int _colorLoc)
{
	positionLoc = _positionLoc;
	normalLoc = _normalLoc;
	texCoordLoc = _texCoordLoc;
	colorLoc = _colorLoc;
}

void Mesh::Display()const
{
	//bind
	vbo->Use();
	const int stride = 9*sizeof(float);
	vbo->Setup(positionLoc, 3, GL_FLOAT, stride, 0);
	vbo->Setup(texCoordLoc, 2, GL_FLOAT, stride, sizeof(float)*3);
	vbo->Setup(colorLoc, 4, GL_FLOAT, stride, sizeof(float)*5);

	//draw
	vbo->Draw();

	VertexBuffer::Disable(positionLoc, normalLoc, texCoordLoc, colorLoc);
}

Mesh* Mesh::createColoredQuad(float left, float top, float right, float bottom, float* color)
{
	std::vector<float> verts = {
		left,  bottom, 0.f, 0.f, 0.f, color[0],  color[1],  color[2],  color[3],
		left,  top,    0.f, 0.f, 1.f, color[4],  color[5],  color[6],  color[7],
		right, top,    0.f, 1.f, 1.f, color[8],  color[9],  color[10], color[11],
		right, bottom, 0.f, 1.f, 0.f, color[12], color[13], color[14], color[15]
	};
	std::vector<unsigned short> indices = {
		2,1,0,
		3,2,0
	};

	return new Mesh(indices, verts);
}