#include "ComputeProgram.h"
#include "ShaderCodeGenerator.h"
#include <iostream>
#include <fstream>

using namespace std;

const char* getShaderStringFromCode(GLenum shaderType)
{
	switch(shaderType)
	{
	case GL_FRAGMENT_SHADER: return "Fragment shader"; break;
	case GL_VERTEX_SHADER: return "Vertex shader"; break;
	case GL_GEOMETRY_SHADER: return "Geometry shader"; break;
	case GL_TESS_CONTROL_SHADER: return "Tess control shader"; break;
	case GL_TESS_EVALUATION_SHADER: return "Tess control shader"; break;
	default: return " - not a valid shader type - "; break;
	}
}

template<int N>
GLuint compileSourceCode(const SourceCode_N<N> &code, GLenum shaderType)
{
	GLuint loadedShader = glCreateShader(shaderType);
	assert(loadedShader != 0);
	glShaderSource(loadedShader, N, const_cast<const GLchar**>(code.code), code.length);
	glCompileShader(loadedShader);
	GLint result = GL_FALSE;
	glGetShaderiv(loadedShader, GL_COMPILE_STATUS, &result);
	if(GL_FALSE == result)
	{
		int infoLogLength;
		glGetShaderiv(loadedShader, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> VertexShaderErrorMessage(infoLogLength);
		glGetShaderInfoLog(loadedShader, infoLogLength, NULL, &VertexShaderErrorMessage[0]);
		cout << getShaderStringFromCode(shaderType) << " error message:\n" << &VertexShaderErrorMessage[0] << endl;
		ofstream error("log/error.shader");
		for(int i = 0; i < N; ++i)
			error << code.code[i] << endl;
		error.close();
	}
	return loadedShader;
}


void ComputeProgram::init()
{
	string vs_code = getVertexComputeShaderSourceCode();
	SourceCode_N<1> tmp;
	tmp.code[0] = vs_code.c_str();
	tmp.length[0] = vs_code.length();
	vertexShaderID = compileSourceCode(tmp, GL_VERTEX_SHADER);
	program = glCreateProgram();
	glAttachShader(program, vertexShaderID);
	code_generator.setSDF_id(0);
	code_generator.reloadAll();
	glBindAttribLocation(program, 0, "pos");
	link();
}

void ComputeProgram::link() const
{
	const SourceCode &tmp = code_generator.getShaderSourceCode();
	GLuint fragmentShaderID = compileSourceCode(tmp, GL_FRAGMENT_SHADER);
	glDetachShader(program, GL_FRAGMENT_SHADER);
	glAttachShader(program, fragmentShaderID);
	glLinkProgram(program);
	int infoLogLength = 0;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
	if(infoLogLength != 0)
	{
		std::vector<char> ProgramErrorMessage(infoLogLength);
		glGetProgramInfoLog(program, infoLogLength, NULL, &ProgramErrorMessage[0]);
		fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
		cout << "Compute Program failed to link:\n" << &ProgramErrorMessage[0] << endl;
	}
	glDeleteShader(fragmentShaderID);
}

void ComputeProgram::render() const
{
	glUseProgram(program);
	//TODO: uniforms
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void ComputeProgram::clean()
{
	glDeleteShader(vertexShaderID);
	glDeleteProgram(program);
}
