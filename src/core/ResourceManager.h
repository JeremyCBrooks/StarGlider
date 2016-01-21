#ifndef ResourceManager_H_
#define ResourceManager_H_

#include <map>
#include <string>
#include <vector>
#include <memory>

class Texture;
class Mesh;
class Shader;
class Font;
class SpriteSheet;
class Entity;
class IPlatform;

class ResourceManager
{
public:

	ResourceManager(IPlatform& platform, float ratio);
	~ResourceManager();

	void CreateQuad(const std::string& name, float left, float top, float right, float bottom, float* color);
	void CreatePolygon(const std::string& name, const std::vector<unsigned short>& indices, const std::vector<float>& vertices);

	std::map<std::string, std::unique_ptr<Texture> > textures;
	std::map<std::string, std::unique_ptr<Mesh> > meshes;
	std::map<std::string, std::unique_ptr<Shader> > shaders;

	std::map<std::string, std::unique_ptr<Entity> > buttons;

	std::map<std::string, std::unique_ptr<Entity> > entities;

	std::unique_ptr<Font> font;
};

#endif