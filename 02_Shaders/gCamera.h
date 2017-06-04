#pragma once

#include <SDL.h>
#include <glm/glm.hpp>

class gCamera
{
public:
	gCamera(void);
	gCamera(glm::vec3 _eye, glm::vec3 _at, glm::vec3 _up);
	~gCamera(void);

	bool Update(float _deltaTime);

	void SetView(glm::vec3 _eye, glm::vec3 _at, glm::vec3 _up);
	void LookAt(glm::vec3 _at);

	inline void SetSpeed(float _val){ m_speed = _val; }
	inline float GetSpeed() const { return m_speed; }

	inline glm::vec3 GetEye()const {	return m_eye; }
	inline glm::vec3 GetAt() const { return m_at; }
	inline glm::vec3 GetUp() const {	return m_up; }

	bool KeyboardDown(SDL_KeyboardEvent& key);
	bool KeyboardUp(SDL_KeyboardEvent& key);
	bool MouseMove(SDL_MouseMotionEvent& mouse);

private:
	void UpdateUV(float du, float dv);

	float	m_speed;
	bool	m_slow;

	glm::vec3	m_eye;
	glm::vec3	m_up;
	glm::vec3	m_at;

	float	m_u;
	float	m_v;
	float	m_dist;

	glm::vec3	m_fw;
	glm::vec3	m_st;

	float	m_goFw;
	float	m_goRight;
};

