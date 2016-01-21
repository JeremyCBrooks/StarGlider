#ifndef IPlatform_H_
#define IPlatform_H_

#include <string>
#include <memory>

class Texture;
class Shader;
class Font;

class IPlatform
{
public:
	virtual std::unique_ptr<Texture> LoadTexture(const std::string& name, bool wrap=false) = 0;
	virtual std::unique_ptr<Shader> LoadShader(const std::string& vert_shader_name, const std::string& frag_shader_name) = 0;
	virtual std::unique_ptr<Font> LoadFont(const std::string& name, Texture& texture, float ratio) = 0;

	virtual std::string GetUserData(const std::string& key) = 0;
	virtual void SetUserData(const std::string& key, const std::string& value) = 0;

    virtual void GameStarted() = 0;
    virtual void GameEnded() = 0;
    
	virtual ~IPlatform() {}
};

#endif