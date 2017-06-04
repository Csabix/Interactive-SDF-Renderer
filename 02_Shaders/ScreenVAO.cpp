#include "ScreenVAO.h"
#include <glm/glm.hpp>

ScreenVAO::ScreenVAO()
{
	glm::vec2 vert[] =
	{
		glm::vec2(-1, -1),
		glm::vec2(+1, -1),
		glm::vec2(-1, +1),
		glm::vec2(+1, +1),
	};
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer((GLuint)0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);


	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glcheck("VAOVBO");
}
ScreenVAO::~ScreenVAO()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}
