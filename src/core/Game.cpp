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
#include "Game.h"
#include "Shader.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Font.h"
#include "Entity.h"
#include "SpriteSheet.h"
#include "SpriteAnimation.h"

#include "platform.h"
#include "IPlatform.h"

#include <stdio.h>
#include <cmath>
#include <string>
#include <vector>
#include <ctime>
#include <sstream>
void checkGLError()
{
	GLenum err = glGetError();
	if (err != GL_NO_ERROR){
		fprintf(stderr, "glGetError returned %d", err);
	}
}

Game::Game(IPlatform* _platform, float screenW, float screenH) : platform(_platform)
{
	srand((unsigned int)time(NULL));

	gameState = Game::Menu;

	screenWidth = screenW;
	screenHeight = screenH;
	ratio = screenHeight / screenWidth;

	viewport[0] = 0;
	viewport[1] = 0;
	viewport[2] = (int)screenW;
	viewport[3] = (int)screenH;

	// Basic GL setup
    glClearColor    (0.f, 0.f, 0.f, 0.f);
    glDisable       (GL_CULL_FACE);
    glEnable		(GL_TEXTURE_2D);
	glDisable		(GL_DEPTH_TEST);
	glDepthMask		(GL_FALSE);
	glEnable		(GL_BLEND);
	glBlendFunc		(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	gldLoadIdentity(sceneProj);
	Ortho(sceneProj, -1.f, 1.f, -1.f, 1.f, -1.0, 100.0f);

	resources.reset(new ResourceManager(*platform.get(), ratio));

	bird = resources->entities["bird"].get();
	bird->spriteSheet->animation->bounce();
	egg = resources->entities["egg"].get();
	for (int i = 0; i < 3; ++i){
		Entity* babyBird = new Entity(*resources->meshes["baby_bird"].get(), *resources->textures["bird.png"].get(), new SpriteSheet(0.f, 2.f, 2.f, 1.f, 3.f, (float)resources->textures["bird.png"]->image.width, (float)resources->textures["bird.png"]->image.height));
		babyBird->spriteSheet->animation->bounce();
		char buffer[16];
		snprintf(buffer, 15, "baby_bird_%d", i);
		resources->entities[buffer].reset(babyBird);
		babyBirds.push_back(babyBird);
	}

	for (int i = 0; i < 2; ++i){
		Entity* zap = new Entity(*resources->meshes["zap"].get(), *resources->textures["zap.png"].get(), new SpriteSheet(0.f, 7.f, 15.f, 8.f, 1.f, (float)resources->textures["zap.png"]->image.width, (float)resources->textures["zap.png"]->image.height));
		char buffer[16];
		snprintf(buffer, 15, "zaps_%d", i);
		resources->entities[buffer].reset(zap);
		zaps.push_back(zap);
	}

	for (int i = 0; i < 6; ++i){
		Entity* blackhole = new Entity(*resources->meshes["blackhole"].get(), *resources->textures["blackhole.png"].get());
		char buffer[16];
		snprintf(buffer, 15, "_blackhole_%d", i);
		resources->entities[buffer].reset(blackhole);
		blackholes.push_back(blackhole);
	}

	for (unsigned int i = 0; i < blackholes.size() / 2; ++i){
		Entity* star = new Entity(*resources->meshes["star"].get(), *resources->textures["star.png"].get());
		char buffer[16];
		snprintf(buffer, 15, "star_%d", i);
		resources->entities[buffer].reset(star);
		stars.push_back(star);
	}

	starfield = resources->entities["0_starfield"].get();
	flat_color_shader = resources->shaders["flat_colored"].get();
	background = resources->meshes["background"].get();
	default_texture = resources->textures["default.png"].get();

	start_button = resources->buttons["start_button"].get();

	newGame();
}

Game::~Game()
{
}

void Game::Display()
{
	float MVM[16];
	
	flat_color_shader->Use();
	flat_color_shader->SetUniformMatrix("Proj", sceneProj);

	//draw background
	cameraMatrix.LoadIdentity();
	cameraMatrix.GetMatrixArray(MVM);
	flat_color_shader->SetUniformMatrix("Model", MVM);
	default_texture->Use();
	background->Display();

	//draw scene
	cameraMatrix.LoadIdentity();
	cameraMatrix.Scale(ratio, 1.f, 1.f);
	for (std::map<std::string, std::unique_ptr<Entity> >::iterator i = resources->entities.begin(); i != resources->entities.end(); ++i){
		if (i->second->active){
			i->second->Display(cameraMatrix, *flat_color_shader);
		}
	}

	if (gameState == Game::Menu){
        //draw score board
		std::string high_score_text = "High Score: ";
		std::string str_hiscore = platform->GetUserData("high_score");
		if ("" == str_hiscore){ str_hiscore = "0"; }
		high_score_text = high_score_text + str_hiscore;
		float high_score_width = resources->font->Width(high_score_text, 0.2f);

		std::string your_score_text = "Your Score: ";
		char str_score[8];
		snprintf(str_score, 7, "%d", score);
		your_score_text = your_score_text + str_score;
		float your_score_width = resources->font->Width(your_score_text, 0.2f);

		float w = fmax(your_score_width, high_score_width);
		start_button->position = Vector3D(0.f, 0.5f, 0.f);
		start_button->scale = Vector3D(w*0.4f, 0.2f, 1.f);
		start_button->Display(cameraMatrix, *flat_color_shader);
		resources->font->Display(high_score_text, Vector3D(-w*0.5f, start_button->position.y - 0.0f, 0.f), cameraMatrix, *flat_color_shader, 0.2f);
		resources->font->Display(your_score_text, Vector3D(-w*0.5f, start_button->position.y - 0.175f, 0.f), cameraMatrix, *flat_color_shader, 0.2f);
  
		//draw buttons
        std::string text = "Start";
        w = resources->font->Width(text, 0.2f);
		start_button->position = Vector3D(0.f, -0.475f, 0.f);
        start_button->scale = Vector3D(w*0.6f,0.1f,1.f);
		start_button->Display(cameraMatrix, *flat_color_shader);
		resources->font->Display(text, Vector3D(-w*0.5f, start_button->position.y - 0.095f, 0.f), cameraMatrix, *flat_color_shader, 0.2f);
	}
    else if(gameState == Game::Playing){
		if (!didFlap){
			std::string text = "collect";
			float w = resources->font->Width(text, 0.2f);
			resources->font->Display(text, Vector3D(-w*0.5f - 0.4f, 0.5f, 0.f), cameraMatrix, *flat_color_shader, 0.2f);
			Vector3D oldP = stars[0]->position;
			stars[0]->position = Vector3D(0.3f, 0.5f - stars[0]->mesh.height*0.5f, 0.f);
			stars[0]->Display(cameraMatrix, *flat_color_shader);
			stars[0]->position = oldP;

			text = "avoid";
			w = resources->font->Width(text, 0.2f);
			resources->font->Display(text, Vector3D(-w*0.5f - 0.4f, 0.f, 0.f), cameraMatrix, *flat_color_shader, 0.2f);
			oldP = blackholes[0]->position;
			blackholes[0]->position = Vector3D(0.3f, 0.1f, 0.f);
			blackholes[0]->Display(cameraMatrix, *flat_color_shader);
			blackholes[0]->position = oldP;

			text = "tap to fly!";
			w = resources->font->Width(text, 0.2f);
			resources->font->Display(text, Vector3D(-w*0.5f, -0.5f, 0.f), cameraMatrix, *flat_color_shader, 0.2f);
		}
        else{
            //draw hud
            char buffer[8];
            snprintf(buffer, 7, "%d", score);
            float w = resources->font->Width(buffer, 0.3f);
            resources->font->Display(buffer, Vector3D(-w*0.5f, 0.7f, 0.f), cameraMatrix, *flat_color_shader, 0.3f);
        }
    }
}

static float accumulator = 0.f;
const float gameTick = 0.003f;
void Game::Update(float frameTime)
{
	accumulator += frameTime;
	while (accumulator >= gameTick)
    {
		stepGameTime(gameTick);
		accumulator -= gameTick;
	}

	updateGameState();
}

void Game::stepGameTime(float dt)
{
	if(gameState == Game::Menu){
	}
	else if (gameState == Game::Playing){
		if (!didFlap){
			stars[0]->angle += dt;
			blackholes[0]->angle += 2.f*dt;
		}
		else{
			bird->Update(dt);
			if (hitBlackhole != NULL){
				bird->angle += 10.f * dt;
				bird->scale = bird->scale - Vector3D(dt);
			}

			if (egg->active){
				egg->Update(dt);
				egg->position.x += scrollSpeed * dt;
				eggHatchCounter += dt;
			}

			starfield->textureOffset[0] -= scrollSpeed * 0.1f * dt;
			if (starfield->textureOffset[0] > 1.f){
				starfield->textureOffset[0] -= 1.f;
			}

			for (unsigned int i = 0; i < stars.size(); ++i){
				stars[i]->angle += dt;
				stars[i]->position.x += scrollSpeed * dt;
			}

			for (unsigned int i = 0; i < blackholes.size(); ++i){
				blackholes[i]->angle += 2.f*dt;
				blackholes[i]->position.x += scrollSpeed * dt;
			}

			for (unsigned int i = 0; i < zaps.size(); ++i){
				zaps[i]->Update(dt);
			}
		}
	}

	for (unsigned int i = 0; i < babyBirds.size(); ++i){
		if (babyBirds[i]->active){
			babyBirds[i]->Update(dt);
			if (hitBlackhole != NULL){
				babyBirds[i]->angle -= 20.f * dt;
			}
		}
	}
}

void Game::updateGameState(){

    if(gameState != Game::Playing){
        return;
    }
    
	if (hitBlackhole != NULL){
		bird->acceleration = (hitBlackhole->position - bird->position) * 200.f;
		bird->acceleration = bird->acceleration - ((bird->velocity - hitBlackhole->velocity) * 5.f);

		if (bird->scale.x <= 0){
            deadBird = true;
			bird->scale = Vector3D();
			gameState = Game::Menu;
            platform->GameEnded();
		}
	}
	else if(!deadBird){
        
        if(!didFlap){return;}
        
		for (unsigned int i = 0; i < blackholes.size(); ++i){
			if (blackholes[i]->InRadius(*bird, 0.55f)){
				//hit blackhole
				hitBlackhole = blackholes[i];
				egg->active = false;
				scrollSpeed = 0.f;

				const Vector3D d = blackholes[i]->position - bird->position;
				const Vector3D n = vmCrossProduct(d, (bird->velocity));
				bird->velocity = vmNormalize(vmCrossProduct(d, n)) * 2.f;

				deadBird = true;
			}
		}

		if (!deadBird &&
			(bird->position.y >(zaps[1]->position.y + zaps[1]->mesh.height * 0.5f) || bird->position.y < (zaps[0]->position.y - zaps[1]->mesh.height * 0.5f))){
			//hit zapper
			gameState = Game::Menu;
			deadBird = true;
            platform->GameEnded();
		}

		if (deadBird){
			for (unsigned int i = 0; i < babyBirds.size(); ++i){
				babyBirds[i]->acceleration = gravity;
				babyBirds[i]->velocity = vmNormalize(Vector3D((frand()*0.30f) - 0.15f, 1.f, 0.f)) * 3.f;
			}
            updateHighScore();
			return;
		}

		if (egg->active){
			if (eggHatchCounter >= 0.25f){
				//egg hatches!
				for (unsigned int i = 0; i < babyBirds.size(); ++i){
					if (!babyBirds[i]->active){
						babyBirds[i]->active = true;
						babyBirds[i]->position = egg->position;
						babyBirds[i]->velocity = Vector3D();
						break;
					}
				}
				egg->active = false;
				eggHatchCounter = 0.f;
			}
		}

		if (flyUp){
			bird->acceleration = upThrust;
			if (bird->velocity.y > maxVelocity){
				bird->velocity.y = maxVelocity;
				flyUp = false;
			}
		}
		else{
			bird->acceleration = gravity;
		}

		//update bird angle based on speed
		bird->angle = (bird->velocity.y / maxVelocity) * 0.35f;
		if (bird->angle > birdMaxAngle){ bird->angle = birdMaxAngle; }
		else if (bird->angle < -birdMaxAngle){ bird->angle = -birdMaxAngle; }

		//update flap speed
		if (flyUp){
			bird->spriteSheet->animation->setFrameRate(bird->acceleration.y*0.75f);
		}
		else{
			bird->spriteSheet->animation->setFrameRate(bird->velocity.y > 0.f ? bird->velocity.y*10.f + 4.f : 0.f);
		}
        
		if (bird->velocity.y <= 0.f){
			bird->spriteSheet->animation->setFrame(2.f);
		}

		for (unsigned int i = 0; i < babyBirds.size(); ++i){
			if (babyBirds[i]->active){
				//update baby bird acceleration
				const Vector3D target = i == 0 ? bird->position : babyBirds[i - 1]->position;
				babyBirds[i]->acceleration = (target - Vector3D(bird->mesh.width*0.55f, 0.f, 0.f) - babyBirds[i]->position) * 50.f;
				babyBirds[i]->acceleration = babyBirds[i]->acceleration - ((babyBirds[i]->velocity - bird->velocity) * 10.f);
				babyBirds[i]->angle = babyBirds[i]->velocity.y / 45.f;
				if (babyBirds[i]->angle > birdMaxAngle){ babyBirds[i]->angle = birdMaxAngle; }
				else if (babyBirds[i]->angle < -birdMaxAngle){ babyBirds[i]->angle = -birdMaxAngle; }

				//update flap speed
				babyBirds[i]->spriteSheet->animation->setFrameRate(babyBirds[i]->velocity.y > 0.f ? babyBirds[i]->velocity.y*20.f + 8.f : 0.f);
				if (babyBirds[i]->velocity.y <= 0.f){
					babyBirds[i]->spriteSheet->animation->setFrame(2.f);
				}
			}
		}

		bool birdTouchedStar = false;
		for (unsigned int i = 0; i < stars.size(); ++i){

			if ((stars[i]->position.x + (stars[i]->mesh.width * 0.5f)) <= -(1.f / ratio)){
				//star off screen
				resetSector(i, true);
				continue;
			}

			if (!stars[i]->active || stars[i]->position.x > 0.f){
				continue;
			}

			if (stars[i]->InRadius(*bird, 0.75f)){
				//bird touched star
				birdTouchedStar = true;
				stars[i]->active = false;
				
			}
			else{
				for (unsigned int j = 0; j < babyBirds.size(); ++j){
					if (babyBirds[j]->active &&
						stars[i]->InRadius(*babyBirds[j], 0.75f)){
						//baby bird touched star
						birdTouchedStar = true;
						stars[i]->active = false;
						break;
					}
				}
			}
		}

		if (birdTouchedStar){
			++score;
			for (unsigned int j = 0; j < babyBirds.size(); ++j){
				if (!babyBirds[j]->active){// && score % 4 == 0){
					//lay an egg!
					egg->active = true;
					egg->velocity = Vector3D();
					egg->position = bird->position;
					egg->acceleration = gravity;
					break;
				}
			}
		}
	}
}

void Game::updateHighScore(){
    std::string str_highscore = platform->GetUserData("high_score");
    int high_score = 0;
    if("" != str_highscore){
        high_score = atoi(str_highscore.c_str());
    }
    if(score > high_score){
		
		#ifdef ANDROID
		platform->SetUserData("high_score", patch::to_string(score));
		#else
        platform->SetUserData("high_score", std::to_string(score));
		#endif // ANDROID
    }
}

void Game::resetSector(int sector_i, bool use_next_sector){
	const int i = sector_i * 2;
	const float width = blackholes[i]->mesh.width;
	const float halfWidth = width * 0.5f;
	const float sectorWidth = width*2.f;
	//update x pos
	float referenceX = 0.f;
	if (use_next_sector){
		const unsigned int next_sector = sector_i > 0 ? sector_i - 1 : ((int)blackholes.size() / 2) - 1;
		const unsigned int n_i = next_sector * 2;
		referenceX = blackholes[n_i]->position.x - halfWidth + sectorWidth;
	}
	else{
		referenceX = (1.f/ratio) + sectorWidth + (sector_i * sectorWidth);
	}

	const float newX = referenceX + width;

	blackholes[i]->position.x = blackholes[i + 1]->position.x = newX;
	blackholes[i]->angle = blackholes[i + 1]->angle = 0.f;

	stars[sector_i]->position.x = newX + sectorWidth * 0.5f;
	stars[sector_i]->angle = 0.f;
	stars[sector_i]->active = true;

	//update y pos
	const float zone = 1.f;
	const float star_zone = 1.f;

	stars[sector_i]->position.y = (frand()*star_zone) - (star_zone*0.5f);
	blackholes[i]->position.y = (frand()*zone);
	blackholes[i + 1]->position.y = (frand()*zone) - zone;
}

void Game::newGame()
{
	//reset everything back to starting states
	gravity = Vector3D(0.f, -5.f, 0.f);
	upThrust = Vector3D(0.f, 15.f, 0.f);

	bird->position = Vector3D(1.f / ratio * -0.425f, -0.275f, 0.f);
	bird->velocity = Vector3D();
	bird->acceleration = Vector3D();
	bird->angle = 0.f;
	bird->scale = Vector3D(1.f);
	bird->spriteSheet->animation->setFrame(0.f);

	for (unsigned int i = 0; i < babyBirds.size(); ++i){
		babyBirds[i]->position = Vector3D();
		babyBirds[i]->velocity = Vector3D();
		babyBirds[i]->acceleration = Vector3D();
		babyBirds[i]->angle = 0.f;
		babyBirds[i]->scale = Vector3D(1.f);
		babyBirds[i]->active = false;
	}

	egg->position = Vector3D();
	egg->velocity = Vector3D();
	egg->acceleration = Vector3D();
	egg->angle = 0.f;
	egg->scale = Vector3D(1.f);
	egg->active = false;

	zaps[0]->position.x = 0.f;
	zaps[0]->position.y = -1.f - (zaps[0]->mesh.height * 0.25f);
	zaps[1]->position.x = 0.f;
	zaps[1]->position.y = 1.f + (zaps[1]->mesh.height * 0.25f);

	birdMaxAngle = 0.78f;
	eggHatchCounter = 0.f;
	score = 0;
	maxVelocity = 1.5f;
	flyUp = false;
	hitBlackhole = hitStar = NULL;
	scrollSpeed = -1.15f;
	deadBird = false;
    didFlap = false;

	for (std::map<std::string, std::unique_ptr<Entity> >::iterator i = resources->entities.begin(); i != resources->entities.end(); ++i){
		i->second->Update(0.f, true);
	}

	for (unsigned int i = 0; i < stars.size(); ++i){
		resetSector(i);
	}
}

void Game::TouchDown(float x, float y)
{
	if(gameState == Game::Menu)
	{
        Vector3D worldPos;
		ConvertScreenToWorld(x, y, worldPos);
		if (start_button->Hit(worldPos)){
			newGame();
			gameState = Game::Playing;
		}
	}
	else if(gameState == Game::Playing)
	{
        if(!didFlap){
            didFlap = true;
            platform->GameStarted();
        }
		flyUp = true;
	}
}

void Game::TouchUp(float x, float y)
{
	if(gameState == Game::Menu)
	{
	}
	else if(gameState == Game::Playing)
	{
		flyUp = false;
	}
}

void Game::TouchMotion(float x, float y)
{
}

void Game::ConvertScreenToWorld(float sx, float sy, Vector3D& wxy)
{
	//	fprintf(stderr, "%f %f\n", sx, sy);

	float MVM[16];
	cameraMatrix.GetMatrixArray(MVM);
	const Vector3D camPos(-MVM[12], -MVM[13], -MVM[14]);

	float obj[3];
	glhUnProjectf(sx, sy, 0.f, MVM, sceneProj, viewport, obj);
	Vector3D p0(obj[0], obj[1], obj[2]);
	glhUnProjectf(sx, sy, 1.f, MVM, sceneProj, viewport, obj);
	Vector3D p1(obj[0], obj[1], obj[2]);

	//	fprintf(stderr, "%f %f %f, %f %f %f\n", p0.x, p0.y, p0.z, p1.x, p1.y, p1.z);

	const Vector3D n = vmNormalize(p1 - p0);
	float t = IntersectRayPlane(camPos, n, Vector3D(), Vector3D(0, 0, 1));
	p0 = p0 + n*t;

	wxy.x = p0.x;
	wxy.y = p0.y;
	wxy.z = p0.z;

//	fprintf(stderr, "%f %f %f\n", p0.x, p0.y, p0.z);
}

//std::to_string is not supported by current android
#ifdef ANDROID
namespace patch
{
	template < typename T > std::string to_string(const T& n)
	{
		std::ostringstream stm;
		stm << n;
		return stm.str();
	}
}
#endif
