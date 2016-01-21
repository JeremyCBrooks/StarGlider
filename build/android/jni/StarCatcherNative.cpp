#include <jni.h>
#include <jniUtils.h>
#include "StarCatcherApplication.h"
#include "PlatformAndroid.h"
extern "C"
{

  jint JNI_OnLoad(JavaVM* vm, void* reserved)
  {
      JNIEnv* env;
      LOGD("Loading external library and passing jvm");
      if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
          return -1;
      }	
  	 GLOBAL_APPLICATION = new StarCatcherApplication(vm);
      // Get jclass with env->FindClass.
      // Register methods with env->RegisterNatives.

      return JNI_VERSION_1_6;
  }

/*
  static void loadAPK (const char* apkPath) {
    LOGI("Loading APK %s", apkPath);
    GLOBAL_APPLICATION->SetAPKArchive(zip_open(apkPath, 0, NULL));
    if (GLOBAL_APPLICATION->GetAPKArchive() == NULL) {
      LOGE("Error loading APK");
      return;
    }
    LOGD("Successfully loaded apkPath");
    //Just for debug, print APK contents
    #ifdef NDEBUG
    zip* APKArchive = GLOBAL_APPLICATION->GetAPKArchive();
    int numFiles = zip_get_num_files(APKArchive);
    for (int i=0; i<numFiles; i++) {
      const char* name = zip_get_name(APKArchive, i, 0);
      if (name == NULL) {
        LOGE("Error reading zip file name at index %i : %s",i, zip_strerror(APKArchive));
        return;
      }
      LOGI("File %i : %s\n", i, name);
    }
    #endif
  }*/

  JNIEXPORT void JNICALL Java_com_brooks_jeremy_starglider_glglue_NativeGame_createEngineJni(
  		JNIEnv* env, jobject object, jstring apkPath, jobject platform, jint screenw,jint screenh) 
  {
   const char* str;
    jboolean isCopy;
    str = env->GetStringUTFChars(apkPath, &isCopy);    
    jobject gplatform = env->NewGlobalRef(platform);
    PlatformAndroid* nplatform = new PlatformAndroid(gplatform);
    GLOBAL_APPLICATION->InitGame(nplatform,(int)screenw,(int)screenh);    
    (env)->ReleaseStringUTFChars(apkPath, str);    
  }


  JNIEXPORT void JNICALL Java_com_brooks_jeremy_starglider_glglue_NativeGame_deleteEngineJni(JNIEnv * env, jobject object)
  {
  	delete GLOBAL_APPLICATION;
    GLOBAL_APPLICATION = NULL;
  }



  JNIEXPORT void JNICALL Java_com_brooks_jeremy_starglider_glglue_NativeGame_updateJni(JNIEnv * env, jobject object)
  {    
    if (GLOBAL_APPLICATION != NULL)
    {
      GLOBAL_APPLICATION->Update();
    }
    else
    {
      LOGW("Trying co call update on disposed game");
    }
  } 


  JNIEXPORT void JNICALL Java_com_brooks_jeremy_starglider_glglue_NativeGame_mouseDownJni(JNIEnv * env, jobject object,jfloat x,jfloat y)
  {    
    if (GLOBAL_APPLICATION != NULL)
    {
      GLOBAL_APPLICATION->MouseDown(x,y);
    }    
    else
    {
      LOGW("Trying co call mousedown on disposed game");
    }
  } 

  JNIEXPORT void JNICALL Java_com_brooks_jeremy_starglider_glglue_NativeGame_mouseUpJni(JNIEnv * env, jobject object,jfloat x,jfloat y)
  {
    if (GLOBAL_APPLICATION != NULL)
    {
      GLOBAL_APPLICATION->MouseUp(x,y);
    }    
    else
    {
      LOGW("Trying co call mouseUp on disposed game");
    }
  } 

  JNIEXPORT void JNICALL Java_com_brooks_jeremy_starglider_glglue_NativeGame_mouseMotionJni(JNIEnv * env, jobject object,jfloat x,jfloat y)
  {
    if (GLOBAL_APPLICATION != NULL)
    {
      GLOBAL_APPLICATION->MouseMotion(x,y);
    }    
    else
    {
      LOGW("Trying co call MouseMotion on disposed game");
    }  

  } 


  JNIEXPORT void JNICALL Java_com_brooks_jeremy_starglider_glglue_NativeGame_keyDownJni(JNIEnv * env, jobject object)
  {
    if (GLOBAL_APPLICATION != NULL)
    {
      GLOBAL_APPLICATION->KeyDown();
    }    
    else
    {
      LOGW("Trying co call keyDown on disposed game");
    }  
  } 


  JNIEXPORT void JNICALL Java_com_brooks_jeremy_starglider_glglue_NativeGame_KeyUpJni(JNIEnv * env, jobject object)
  {
    if (GLOBAL_APPLICATION != NULL)
    {
      GLOBAL_APPLICATION->KeyUp();
    }    
    else
    {
      LOGW("Trying co call KeyUp on disposed game");
    }  
  } 

} //extern C
