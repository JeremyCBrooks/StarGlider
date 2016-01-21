#ifndef starcatcher_platform_h
#define starcatcher_platform_h

#include <GLES2/gl2.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#define snprintf sprintf_s

void checkGLError();

#endif