#ifndef PlatformWin_H_
#define PlatformWin_H_

#include "IPlatform.h"
#include <map>

class PlatformWin : public IPlatform
{
public:
	PlatformWin(const std::string& dataPath);
	~PlatformWin();

	std::unique_ptr<Texture> LoadTexture(const std::string& name, bool wrap = false);
	std::unique_ptr<Shader> LoadShader(const std::string& vert_shader_name, const std::string& frag_shader_name);
	std::unique_ptr<Font> PlatformWin::LoadFont(const std::string& name, Texture& texture, float ratio);

    std::string GetUserData(const std::string& key);
	void SetUserData(const std::string& key, const std::string& value);
	void GameStarted();
	void GameEnded();
private:
	std::string dataPath;
	std::map<std::string, std::string> userData;	
};

#endif