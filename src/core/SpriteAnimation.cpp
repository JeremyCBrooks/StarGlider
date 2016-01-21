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
#include "SpriteAnimation.h"

SpriteAnimation::SpriteAnimation(const float startframe, const float endframe, const float frame_rate):AS_STOP(0),AS_BOUNCE(1),AS_LOOP(2),AS_PLAY_ONCE(3)
{
	m_start_frame = startframe;
	m_end_frame = endframe;

	m_current_frame = m_start_frame;
	m_accumulator = 0.f;

	m_frames = (m_end_frame+1) - m_start_frame;

	if(frame_rate != 0.f)
		m_frame_rate = 1000.f/frame_rate;
	else
		m_frame_rate = frame_rate;

	delta_frame = 1;

	stop();
}

void SpriteAnimation::setFrame(const float newframe)
{
	if(newframe >= m_start_frame && newframe <= m_end_frame)
		m_current_frame = newframe;
}

void SpriteAnimation::update(float dt)
{
	if (m_state == AS_STOP || m_frame_rate == 0.f)
		return;

	if(m_end_frame == m_start_frame)
		m_state = AS_STOP;

	m_accumulator+=(dt*1000.f);
	if(m_accumulator > m_frame_rate)
	{		
		m_accumulator = 0.f;

		if(m_state == AS_LOOP)
		{
			++m_current_frame;
			if(m_current_frame > m_end_frame)
				m_current_frame = m_start_frame;
		}
		else if(m_state == AS_BOUNCE)
		{
			m_current_frame+=delta_frame;
			if(m_current_frame <= m_start_frame)
			{
				m_current_frame = m_start_frame;
				delta_frame*=-1;
			}
			else if(m_current_frame >= m_end_frame)
			{
				m_current_frame = m_end_frame;
				delta_frame*=-1;
			}
		}
		else if(m_state == AS_PLAY_ONCE)
		{
			++m_current_frame;
			if(m_current_frame > m_end_frame)
			{
				stop();
			}
		}
	}
}