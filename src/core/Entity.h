#ifndef Entity_H_
#define Entity_H_

#include <string>
#include <memory>
#include "VectorMath.h"

class Mesh;
class Texture;
class Shader;
class SpriteSheet;

class Entity
{
public:
	Entity(Mesh& mesh, Texture& texture, SpriteSheet* spriteSheet=NULL);
	~Entity();

	void Update(float dt, bool force=false);
	void Display(const Matrix& camera, const Shader& shader)const;

	bool Hit(const Vector3D&)const;
	bool InRadius(Entity& entity, float radius_scale = 1.f)const;

	Mesh& mesh;
	Texture& texture;
	std::unique_ptr<SpriteSheet> spriteSheet;

	Vector3D position;
	Vector3D velocity;
	Vector3D acceleration;

	Vector3D scale;
	float angle;
	bool active;

	float textureOffset[2];

};

#endif