#pragma once
#include <GL/glew.h>
#include "GLCHECK.h"
class ScreenVAO
{
	GLuint vao;
	GLuint vbo;
public:
	ScreenVAO();
	~ScreenVAO();

	ScreenVAO(const ScreenVAO&) = delete;
	ScreenVAO& operator = (const ScreenVAO&) = delete;

	inline void on() { glBindVertexArray(vao); }
	inline void draw() const
	{
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
};

