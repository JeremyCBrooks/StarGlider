#ifndef SPRITEANIMATION_H_
#define SPRITEANIMATION_H_

class SpriteAnimation
{
public:
	SpriteAnimation(const float startframe = 0.f, const float endframe = 0.f, const float frame_rate = 0.f);

	void setFrame(const float f);
	void setFrameRate(float fr) { m_frame_rate = (fr != 0.f ? 1000.f / fr : fr); }

	void update(float dt);
	void nextFrame() { ++m_current_frame; if(m_current_frame >= m_end_frame) m_current_frame = m_start_frame; m_accumulator = 0.f; }
	void previousFrame() { --m_current_frame; if(m_current_frame < m_start_frame) m_current_frame = m_end_frame; m_accumulator = 0.f; }

	void loop() { m_state = AS_LOOP; m_current_frame = m_start_frame; m_last_state = m_state; m_accumulator = 0.f; }
	void bounce() { m_state = AS_BOUNCE; m_current_frame = m_start_frame; delta_frame = 1; m_last_state = m_state; m_accumulator = 0.f; }
	void playOnce() { m_state = AS_PLAY_ONCE; m_current_frame = m_start_frame; m_last_state = m_state; m_accumulator = 0.f; }
	void stop() { m_state = AS_STOP; m_current_frame = m_start_frame; m_last_state = m_state; m_accumulator = 0.f; }
	void pause() { m_last_state = m_state; m_state = AS_STOP; }
	void resume() { m_state = m_last_state; }

	float numberOfFrames()const { return m_frames; }
	float currentFrame()const { return m_current_frame; }
	bool isPlaying()const { return m_state != AS_STOP; }

	float getStartFrame()const { return m_start_frame; }
	float getEndFrame()const { return m_end_frame; }

	void setStartFrame(float sf) {m_start_frame = sf;}
	void setEndFrame(float ef) {m_end_frame = ef;}

private:
	int m_last_state;
	int m_state;
	
	float m_start_frame;
	float m_end_frame;

	float m_frames;
	float m_current_frame;
	float m_accumulator;

	float m_frame_rate;

	float delta_frame;

	const int AS_STOP;
	const int AS_BOUNCE;
	const int AS_LOOP;
	const int AS_PLAY_ONCE;
};

#endif