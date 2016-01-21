#include "PlatformAndroid.h"
#include "StarCatcherApplication.h"
//#include "png.h"
//#include "zip.h"
#include <string>
#include "platform.h"
#include "Texture.h"
#include "Shader.h"
#include "Font.h"

/*
zip_file* file;
void png_zip_read(png_structp png_ptr, png_bytep data, png_size_t length) {
  zip_fread(file, data, length);
}
*/

PlatformAndroid::PlatformAndroid(jobject jp)
{
  _javaPlatform = jp;
  JNIEnv *env = GLOBAL_APPLICATION->GetEnv();
  jclass jplatformClass = env->GetObjectClass(_javaPlatform);
  _jplatformClass = (jclass)env->NewGlobalRef(jplatformClass);
}

std::unique_ptr<Texture> PlatformAndroid::LoadTexture(const std::string& name, bool wrap)
{
  LOGD("In LoadTexture %s",name.c_str());
  JNIEnv *env = GLOBAL_APPLICATION->GetEnv();
  jmethodID loadTexture = env->GetMethodID( _jplatformClass, "LoadTexture", "(Ljava/lang/String;Z)I");
  
  jstring filenameStr = env->NewStringUTF(name.c_str());
  

  int  result = (int) env->CallIntMethod( _javaPlatform, loadTexture,filenameStr,wrap?JNI_TRUE:JNI_FALSE); 
  
  GLOBAL_APPLICATION->CleanAfterCall();
	
std::unique_ptr<Texture> texture(new Texture(result));

  return texture;
}



//helper functions
std::string PlatformAndroid::GetFileContents(const std::string& filename)
{
  LOGD("In getFileContents %s",filename.c_str());
  JNIEnv *env = GLOBAL_APPLICATION->GetEnv();
  jmethodID getFileContent = env->GetMethodID( _jplatformClass, "ReadFileContent", "(Ljava/lang/String;)Ljava/lang/String;");
  
  jstring filenameStr = env->NewStringUTF(filename.c_str());
  

  jstring returnJNIStr = (jstring) env->CallObjectMethod( _javaPlatform, getFileContent,filenameStr); 
  
  if (returnJNIStr == NULL)
    return std::string("");

  const char *resultCStr = env->GetStringUTFChars(returnJNIStr, NULL);
  if (NULL == resultCStr) return std::string("");
  LOGD("The returned string content is : %s",resultCStr);
  std::string returnResult(resultCStr);
  env->ReleaseStringUTFChars(returnJNIStr, resultCStr);   
  GLOBAL_APPLICATION->CleanAfterCall();
  return returnResult;
}

std::unique_ptr<Shader> PlatformAndroid::LoadShader(const std::string& vert_shader_name, const std::string& frag_shader_name)
{
	std::string vert_shader_fn = vert_shader_name;
  std::string frag_shader_fn = frag_shader_name;

  LOGD("===================\n");
  LOGD("Building Shader\n");
  LOGD("vertex shader: %s\n", vert_shader_fn.c_str());
  LOGD("fragment shader: %s\n", frag_shader_fn.c_str());
  LOGD("Loading source...");
  std::string vertex_shader_source = GetFileContents(vert_shader_fn);
  std::string fragment_shader_source = GetFileContents(frag_shader_fn);
  LOGD("done.\n");

  return std::unique_ptr<Shader>(new Shader(vertex_shader_source, fragment_shader_source));
}

std::unique_ptr<Font> PlatformAndroid::LoadFont(const std::string& name, Texture& texture, float ratio)
{
  return std::unique_ptr<Font>(new Font(GetFileContents(name), texture, ratio));
}




void PlatformAndroid::GameEnded()
{
  JNIEnv *env = GLOBAL_APPLICATION->GetEnv();
  jmethodID gameEnded = env->GetMethodID(_jplatformClass, "GameEnded", "()V");
  env->CallVoidMethod(_javaPlatform, gameEnded); 
  GLOBAL_APPLICATION->CleanAfterCall();
}

void PlatformAndroid::GameStarted()
{
  //jni to java
  JNIEnv *env = GLOBAL_APPLICATION->GetEnv();
  jmethodID gameStarted = env->GetMethodID( _jplatformClass, "GameStarted", "()V");
  env->CallVoidMethod( _javaPlatform, gameStarted); 
  GLOBAL_APPLICATION->CleanAfterCall();
}

std::string PlatformAndroid::GetUserData(const std::string& key)
{
  LOGD("In get user data %s",key.c_str());
  JNIEnv *env = GLOBAL_APPLICATION->GetEnv();
  jmethodID getUserData = env->GetMethodID( _jplatformClass, "GetUserData", "(Ljava/lang/String;)Ljava/lang/String;");
  
  jstring keyStr = env->NewStringUTF(key.c_str());

  LOGD("After finding class %s",key.c_str());

  jstring returnJNIStr = (jstring) env->CallObjectMethod( _javaPlatform, getUserData,keyStr); 
  
  if (returnJNIStr == NULL)
    return std::string("");

  const char *resultCStr = env->GetStringUTFChars(returnJNIStr, NULL);
  if (NULL == resultCStr) return std::string("");
  
  std::string returnResult(resultCStr);
  env->ReleaseStringUTFChars(returnJNIStr, resultCStr);   
  GLOBAL_APPLICATION->CleanAfterCall();
  return returnResult;
}

void PlatformAndroid::SetUserData(const std::string& key, const std::string& value)
{
  JNIEnv *env = GLOBAL_APPLICATION->GetEnv();
  jmethodID setUserData = env->GetMethodID(_jplatformClass, "SetUserData", "(Ljava/lang/String;Ljava/lang/String;)V");
  
  jstring keyStr = env->NewStringUTF(key.c_str());
  jstring valueStr = env->NewStringUTF(value.c_str());
  env->CallVoidMethod(_javaPlatform, setUserData,keyStr,valueStr); 
  GLOBAL_APPLICATION->CleanAfterCall();
}

PlatformAndroid::~PlatformAndroid()
{
  //partial destructor will complete later
  JNIEnv *env = GLOBAL_APPLICATION->GetEnv();
  env->DeleteGlobalRef(_jplatformClass);
  env->DeleteGlobalRef(_javaPlatform);  
}