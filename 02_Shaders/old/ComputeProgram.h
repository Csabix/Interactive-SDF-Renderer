#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "ShaderCodeGenerator.h"

class ComputeProgram
{
	GLuint program = 0;
	GLuint vertexShaderID = 0;
public:
	//ShaderCodeGenerator code_generator;
	void init();
	void link() const;
	void render() const; ///VAO must be set
	void clean();
};