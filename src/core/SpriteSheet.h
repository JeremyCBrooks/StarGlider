#ifndef SPRITESHEET_H_
#define SPRITESHEET_H_

#include <memory>

class SpriteAnimation;

class SpriteSheet
{
public:
	SpriteSheet(float start_frame, float end_frame, float frame_rate, float rows, float columns, float size_x, float size_y);
	~SpriteSheet();

	bool Update(float dt);

	float size_x;
	float size_y;

	float startx;
	float endx;
	float starty;
	float endy;

	std::unique_ptr<SpriteAnimation> animation;

private:

	float rows;
	float columns;

	float invs_cols, invs_rows;
	
	float curr_frame;
};

#endif