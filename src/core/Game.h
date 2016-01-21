#ifndef GAME_H_
#define GAME_H_

#include <string>
#include <map>
#include <vector>
#include <memory>
#include "VectorMath.h"

class FrameBuffer;
class ResourceManager;
class Entity;
class Shader;
class Mesh;
class Texture;
class IPlatform;

class Game
{
public:

	Game(IPlatform* platform, float screenW, float screenH);
	~Game();

	void Update(float frameTime);
	void Display();
	
	void TouchMotion(float x, float y);
	void TouchDown(float x, float y);
	void TouchUp(float x, float y);

	float screenWidth, screenHeight;

private:

	enum State
	{
		Menu,
		Playing
	};
	
	void ConvertScreenToWorld(float sx, float sy, Vector3D& wxy);

	void newGame();
	
	float ratio;

	std::unique_ptr<ResourceManager> resources;

	State gameState;

	float sceneProj[16];
	Matrix cameraMatrix;
	int viewport[4];

	Entity* bird;
	std::vector<Entity*> babyBirds;
	Entity* egg;
	std::vector<Entity*> zaps;
	std::vector<Entity*> stars;
	std::vector<Entity*> blackholes;

	Shader* flat_color_shader;
	Mesh* background;
	Entity* starfield;
	Texture* default_texture;

	Entity* hitStar;
	Entity* hitBlackhole;

	Entity* start_button;

	float eggHatchCounter;
	float birdMaxAngle;
	float scrollSpeed;

	int score;
	bool flyUp;
	Vector3D gravity;
	Vector3D upThrust;
	float maxVelocity;
	bool deadBird;
    bool didFlap;

	void stepGameTime(float dt);
	void updateGameState();
	void resetSector(int i, bool use_next_sector = false);
	void newBabyBird(const Vector3D& position);
    void updateHighScore();

	std::unique_ptr<IPlatform> platform;

};

#endif