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
#include "SpriteSheet.h"
#include "SpriteAnimation.h"
#include <cmath>

SpriteSheet::SpriteSheet(float sf, float ef, float fr, float r, float c, float sx, float sy)
{
	animation.reset(new SpriteAnimation(sf, ef, fr));
	animation->loop();

	rows = r;
	columns = c;

	invs_cols = 0;
	if (c>0.f)
		invs_cols = 1.0f / c;

	invs_rows = 0;
	if (r>0.f)
		invs_rows = 1.0f / r;

	startx = 0.0f;
	endx = 1.0f;
	starty = 0.0f;
	endy = 1.0f;
	curr_frame = 0;

	size_x = sx*0.5f;
	size_y = sy*0.5f;

	Update(0.f);
}

SpriteSheet::~SpriteSheet()
{
}

bool SpriteSheet::Update(float dt)
{
	if (!animation){ return false; }

	float last_frame = curr_frame;

	animation->update(dt);

	curr_frame = animation->currentFrame();
	startx = (fmod(curr_frame, columns)*invs_cols);
	endx = startx+invs_cols;
	starty = (floorf(curr_frame*invs_cols)*invs_rows);
	endy = starty+invs_rows;

	return curr_frame!=last_frame;
}
