#ifndef ___PLARFORM_ANDROID_H_
#define ___PLARFORM_ANDROID_H_
#include <IPlatform.h>
#include <map>
#include <jni.h>

class PlatformAndroid : public IPlatform
{
private:
	jobject _javaPlatform;
	jclass _jplatformClass;	
	JNIEnv *GetEnv();
	void CleanAfterCall();
	bool _shouldDetach = false;
	std::string GetFileContents(const std::string& filename);
public:
	PlatformAndroid(jobject jp);
	~PlatformAndroid();

	std::unique_ptr<Texture> LoadTexture(const std::string& name, bool wrap = false);
	std::unique_ptr<Shader> LoadShader(const std::string& vert_shader_name, const std::string& frag_shader_name);
	std::unique_ptr<Font> LoadFont(const std::string& name, Texture& texture, float ratio);

    std::string GetUserData(const std::string& key);
	void SetUserData(const std::string& key, const std::string& value);
	void GameStarted();
	void GameEnded();
private:
	std::string dataPath;
	std::map<std::string, std::string> userData;
};

#endif
