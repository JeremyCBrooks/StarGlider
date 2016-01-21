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
#include <stdio.h>
#include <math.h>
#include <string>
#include <vector>
#include <exception>

#include <GLES2/gl2.h>
#include <SDL/SDL.h>
#include "Game.h"
#include "PlatformWin.h"
#include "xgetopt.h"

#if WIN32
extern "C" 
GL_API int GL_APIENTRY _dgles_load_library(void *, void *(*)(void *, const char *));
static void *proc_loader(void *h, const char *name)
{
    (void) h;
    return SDL_GL_GetProcAddress(name);
}
#endif

Game* game = NULL;
float frameTimeMod = 1.f;

// Initializes the application data
int Init(const std::string& dataPath) 
{
    // Initialize the SDL library with the Video subsystem
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE);

    // create OpenGL 2.0 context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);

    // Set the video mode to full screen with OpenGL-ES support
	float screen_w = 640.f*1.f;// 480.f;
	float screen_h = 960.f*1.f;// 320.f;

	//switch to landscape
	SDL_SetVideoMode((int)screen_w, (int)screen_h, 0, SDL_OPENGL);

	fprintf(stderr, "Video mode set. W: %f H: %f\n", screen_w, screen_h);
#if WIN32
    // Load the desktop OpenGL-ES emulation library
    _dgles_load_library(NULL, proc_loader);
#endif

	try
	{
		game = new Game(new PlatformWin(dataPath), screen_w, screen_h);
	}
	catch(const std::exception& ex)
	{
		fprintf(stderr, "Critical: %s\n", ex.what());
		return GL_FALSE;
	}

    return GL_TRUE;
}

void shutdown()
{
	fprintf(stderr, "shutting down...");

	if (game){
		delete game;
		game = NULL;
	}

	SDL_Quit();

	fprintf(stderr, "done.\n");
}

int handleEvents(const SDL_Event *Event)
{
	 switch (Event->type) {
		case SDL_MOUSEBUTTONDOWN:
			{
				game->TouchDown(Event->button.x, game->screenHeight - Event->button.y);
			}
			return 0;

		case SDL_MOUSEBUTTONUP:
			{
				game->TouchUp(Event->button.x, game->screenHeight - Event->button.y);
			}
			return 0;

		case SDL_MOUSEMOTION:
			{
				game->TouchMotion(Event->button.x, game->screenHeight - Event->button.y);
			}
			return 0;

		case SDL_KEYDOWN:
			{
				frameTimeMod = 0.01f;
			}
			return 0;

		case SDL_KEYUP:
			{
					frameTimeMod = 1.f;
			}
			return 0;
	 }

	 return 1;
}

int main(int argc, char** argv)
{
	atexit(shutdown);

	std::string dataPath = "../../data/";
	
	int c = 0;
	while ((c = getopt (argc, argv, "dl:")) != -1)
	switch (c)
	{
		case 'd':
			dataPath = optarg;
			break;
			break;
		case '?':
			fprintf (stderr, "Bad command line arg. Options are -d <data_path>");
			break;
		default: break;
	}

	// Application specific Initialize of data structures & GL states
    if (Init(dataPath) == false)
        return -1;

    // Event descriptor
    SDL_Event Event;
    bool paused = false;
	Uint32 lastFrameTime=SDL_GetTicks();

	SDL_SetEventFilter(handleEvents);

    while (1) {
		if(!paused){
			Uint32 curTicks=SDL_GetTicks();
			float frameTime = (float)(curTicks-lastFrameTime)/1000.f;
			lastFrameTime = curTicks;

			game->Update(frameTime * frameTimeMod);
			game->Display();

			SDL_GL_SwapBuffers();
		}

        bool gotEvent;
        if (paused) {
            SDL_WaitEvent(&Event);
            gotEvent = true;
        }
        else {
            gotEvent = (1==SDL_PollEvent(&Event));
        }
        
        while (gotEvent) {
            switch (Event.type) {
                case SDL_ACTIVEEVENT:
                    if (Event.active.state == SDL_APPACTIVE) {
                        paused = !Event.active.gain;
                    }
                    break;

				case SDL_QUIT:
                    // We exit anytime we get a request to quit the app
                    // all shutdown code is registered via atexit() so this is clean.
                    exit(0);
                    break;

                default:
                    break;
            }
            gotEvent = (1==SDL_PollEvent(&Event));
        }
		SDL_Delay(1);
    }

    return 0;
}
