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
#include "Entity.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "SpriteSheet.h"

Entity::Entity(Mesh& mesh, Texture& texture, SpriteSheet* spriteSheet) :
	mesh(mesh),
	texture(texture),
	spriteSheet(spriteSheet),
	position(Vector3D()),
	velocity(Vector3D()),
	scale(Vector3D(1.f)),
	angle(0.f),
	active(true)
{
	textureOffset[0] = textureOffset[1] = 0.f;
}

Entity::~Entity()
{
}

void Entity::Update(float dt, bool force)
{
	position.x += velocity.x * dt;
	position.y += velocity.y * dt;

	velocity.x += acceleration.x * dt;
	velocity.y += acceleration.y * dt;

    acceleration.z = velocity.z = position.z = 0.f;
    
	if (spriteSheet && (spriteSheet->Update(dt) || force))
	{
		std::vector<float> vertices = {
			mesh.left,  mesh.bottom, 0.0f, spriteSheet->startx, spriteSheet->endy,   1.f, 1.f, 1.f, 1.f,
			mesh.left,	mesh.top,	 0.0f, spriteSheet->startx, spriteSheet->starty, 1.f, 1.f, 1.f, 1.f,
			mesh.right, mesh.top,	 0.0f, spriteSheet->endx,	spriteSheet->starty, 1.f, 1.f, 1.f, 1.f,
			mesh.right, mesh.bottom, 0.0f, spriteSheet->endx,	spriteSheet->endy,	 1.f, 1.f, 1.f, 1.f
		};
		mesh.updateVertices(vertices);
	}
}

void Entity::Display(const Matrix& camera, const Shader& shader)const
{
	float MVM[16];
	Matrix modelView;
	modelView.Rotate(angle, Vector3D(0.0f, 0.0f, 1.0f));
	modelView.Scale(scale.x, scale.y, scale.z);
	modelView.Translate(Vector3D(position.x, position.y, position.z));
	modelView.Concatenate(camera);
	modelView.GetMatrixArray(MVM);
	shader.SetUniformMatrix("Model", MVM);
	shader.SetUniform2fv("TexOffset", textureOffset);
	texture.Use();
	mesh.Display();
}

bool Entity::Hit(const Vector3D& p)const
{
	return	p.x >= (mesh.left + position.x)	&& p.x <= (mesh.right + position.x) &&
			p.y >= (mesh.bottom + position.y)	&& p.y <= (mesh.top + position.y);
}

bool Entity::InRadius(Entity& other, float radius_scale)const
{
	const float this_r = mesh.width*0.5f*radius_scale;
    const float other_r = other.mesh.width*0.5f*radius_scale;
    const float sum_r = this_r+other_r;
    const Vector3D d = position - other.position;
	return (vmDotProduct(d, d)) <= (sum_r*sum_r);
}