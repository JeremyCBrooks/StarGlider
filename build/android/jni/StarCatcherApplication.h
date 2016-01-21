#ifndef ____SC_APPLICATION____
#define ____SC_APPLICATION____
#include <jni.h>
#include "log.h"
//#include "zip.h"
#include "Game.h"
#include "PlatformAndroid.h"

class StarCatcherApplication
{
private:
	float _frameTimeMod; 	
//	zip* _APKArchive;
	long _lastFrameTime = 0;

	Game* _game;
	PlatformAndroid* _platform;	

	bool _shouldDetach = false;
//Inited from java
	int _screenWidth;
	int _screenHeight;
	JavaVM* _vm;

//helper methods
	long getTime();
	float GetFrameTime();
	

public:
	StarCatcherApplication(JavaVM* vm);
	~StarCatcherApplication();
	void InitGame(PlatformAndroid* platform,int screenw,int screenh);
//	zip* GetAPKArchive();
//	void SetAPKArchive(zip* archive);
	
	JavaVM* GetVM();	
	JNIEnv *GetEnv();	
	void CleanAfterCall();
	
	void Update();
	void MouseDown(float x,float y);
	void MouseUp(float x,float y);
	void MouseMotion(float x,float y);
	void KeyDown();
	void KeyUp();

};

extern StarCatcherApplication* GLOBAL_APPLICATION;

#endif
