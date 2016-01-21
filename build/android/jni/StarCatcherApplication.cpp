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
#include <jni.h>
#include "log.h"
#include "StarCatcherApplication.h"
#include "PlatformAndroid.h"
#include "Game.h"


StarCatcherApplication* GLOBAL_APPLICATION = nullptr;

StarCatcherApplication::StarCatcherApplication(JavaVM* vm)
{		
	LOGD("StarCatcherApplication constructor setting vm");
	_vm = vm;
	_lastFrameTime = getTime();
//	_APKArchive = NULL;	
}


void StarCatcherApplication::CleanAfterCall()
{
  JavaVM* vm = this->GetVM();
  if (_shouldDetach)
    vm->DetachCurrentThread();
}


JNIEnv *StarCatcherApplication::GetEnv()
{
	if (_vm == nullptr)
	{
		LOGE("The JavaVM IS NULL, the application will CRASH");
	}
	
	LOGD("In get environment");
	JNIEnv *env;
	_shouldDetach = false;  
	JavaVM* vm = this->GetVM();
	jint rc = vm->GetEnv((void **)&env, JNI_VERSION_1_6);
	LOGD("trying to get env %d",(int)rc);
	if (rc != JNI_OK)
	{
		_shouldDetach = true;
		vm->AttachCurrentThread(&env, NULL);
	}
	LOGD("Got environment ");
	return env;
}



StarCatcherApplication::~StarCatcherApplication()
{
	LOGD("Delete called");
	delete _game;
	_game = NULL;
	delete _platform;
	_platform = NULL;
}

void StarCatcherApplication::InitGame(PlatformAndroid* platform,int screenw,int screenh)
{
	_frameTimeMod = 1.f;
	_screenWidth = screenw;
	_screenHeight = screenh;
	_platform = platform;
	try
	{
		LOGD("Creating Game Object");
		_game = new Game(_platform,_screenWidth,_screenHeight);
	}
	catch(const std::exception& ex)
	{	 
		LOGE("Failed creating game object %s\n",ex.what());
	}
}

JavaVM*  StarCatcherApplication::GetVM()
{
	if (_vm == nullptr)
	{
		LOGE("The JavaVM IS NULL, the application will CRASH");
	}
	return _vm;
}
/*
zip* StarCatcherApplication::GetAPKArchive()
{
	if (_APKArchive == NULL)
	{
		LOGE("The _APKArchive IS NULL, the application will CRASH");
	}
	return _APKArchive;
}

void StarCatcherApplication::SetAPKArchive(zip* archive)
{
	if (archive == NULL)
	{
		LOGE("The archive setting to IS NULL, the application will CRASH");		
	}
	_APKArchive = archive;
}
*/
long StarCatcherApplication::getTime()
{
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	return now.tv_sec*1000UL + now.tv_nsec/1000000UL;
}	

float StarCatcherApplication::GetFrameTime()
{
	long curTicks = getTime();
	const float dt = (float)(curTicks-_lastFrameTime)*0.001f;
	_lastFrameTime = curTicks;
	return dt > 0.0333f ? 0.0333f : 0.01667f;
}

void StarCatcherApplication::Update()
{
//	LOGD("In update StarCatcherApplication %d",(int)_game);
	_game->Update(GetFrameTime() * _frameTimeMod);
//	LOGD("After update before display ");
	_game->Display();
//	LOGD("After display ");
}

void StarCatcherApplication::MouseDown(float x,float y)
{		
	_game->TouchDown(x,  _game->screenHeight - y);
}

void StarCatcherApplication::MouseUp(float x,float y)
{
	_game->TouchUp(x,  _game->screenHeight - y);
}

void StarCatcherApplication::MouseMotion(float x,float y)
{	
	_game->TouchMotion(x, _game->screenHeight - y);
}

void StarCatcherApplication::KeyDown()
{
	_frameTimeMod = 0.01f;
}
void StarCatcherApplication::KeyUp()
{
	_frameTimeMod = 1.f;
}
