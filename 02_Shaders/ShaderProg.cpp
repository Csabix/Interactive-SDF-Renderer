#include "ShaderProg.h"
#include <iostream>
#include "FileIO.h"
#include <string>
#include "GLCHECK.h"

GLuint ShaderProg::used_program = 0;

ShaderProg::ShaderProg(std::ostream &out) : uniform_locations(), out(out)
{
	program = glCreateProgram();
}

ShaderProg::~ShaderProg(void)
{
	for(auto &shader : shader_type_names)
	{
		if(0 != program && 0 != shader.second)
			glDetachShader(program, shader.second);
		if(shader.second != 0)
			glDeleteShader(shader.second);
	}
	if(program != 0)
		glDeleteProgram(program);
}

const char* getShaderStringFromCode(GLenum shaderType)
{
	switch(shaderType)
	{
	case GL_VERTEX_SHADER:			return "vert"; break;
	case GL_GEOMETRY_SHADER:		return "geom"; break;
	case GL_TESS_CONTROL_SHADER:	return "tesc"; break;
	case GL_TESS_EVALUATION_SHADER: return "tese"; break;
	case GL_FRAGMENT_SHADER:		return "frag"; break;
	default: return "Not-A-Valid-"; break;
	}
}

bool ShaderProg::compileSourceCode(const SourceCode &code, GLuint loadedShader, GLenum shaderType)
{
	assert(loadedShader != 0);
	glShaderSource(loadedShader, code.size(), code.data(), nullptr);
	glCompileShader(loadedShader);
	GLint result = GL_FALSE;
	glGetShaderiv(loadedShader, GL_COMPILE_STATUS, &result);
	if(GL_FALSE == result)
	{
		int infoLogLength;
		glGetShaderiv(loadedShader, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> shaderErrorMessage(infoLogLength);
		glGetShaderInfoLog(loadedShader, infoLogLength, NULL, &shaderErrorMessage[0]);
		std::string sType_str = getShaderStringFromCode(shaderType);
		out << sType_str << " shader error message:\n" << &shaderErrorMessage[0] << std::endl;
		std::cout << sType_str << " shader error message:\n" << &shaderErrorMessage[0] << std::endl;
		for(size_t i = 0; i < code.size(); ++i)
		{
			std::string path = "log/error" + std::to_string(loadedShader) + "_part_" + std::to_string(i)  + "." + sType_str;
			FileIO::SaveTo(path.c_str(), code[i], 0, &out);
		}
	}
	return result == GL_TRUE;
}

bool ShaderProg::AttachShader(GLenum _shaderType, const SourceCode & code)
{
	if(program == 0)
	{
		program = glCreateProgram();
		if(program == 0 && m_verbose)
		{
			out << "Hiba a shader program létrehozásakor" << std::endl;
			std::cout << "Hiba a shader program létrehozásakor" << std::endl;
			return false;
		}
	}
	if(shader_type_names.count(_shaderType) != 0)
		glDetachShader(program, shader_type_names[_shaderType]);
	else
		shader_type_names[_shaderType] = glCreateShader(_shaderType);
	if(!compileSourceCode(code, shader_type_names[_shaderType], _shaderType))
		return false;
	glAttachShader(program, shader_type_names[_shaderType]);
	return true;
}

bool ShaderProg::Link()
{
	glLinkProgram(program);

	GLint log_len = 0, result = 0;

	glGetProgramiv(program, GL_LINK_STATUS, &result);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_len);
	if(0 != log_len || GL_FALSE == result)
	{
		std::vector<char> msg(log_len);
		glGetProgramInfoLog(program, log_len, nullptr, &msg[0]);
		out << "Link Error Message:\n" << msg.data() << std::endl;
		std::cout << "Link Error Message:\n" << msg.data() << std::endl;
	}
	return result == GL_TRUE;
}

void ShaderProg::BindAttribLoc(int _index, const char* _uniform)
{
	glBindAttribLocation(program, _index, _uniform);
}

void ShaderProg::BindFragDataLoc(int _index, const char* _uniform)
{
	glBindFragDataLocation(program, _index, _uniform);
}

void ShaderProg::SetUniform(const char* _uniform, const glm::vec2& _vec)
{
	GLint loc = getLocation(_uniform);
	glUniform2fv( loc, 1, &_vec[0] );
}

void ShaderProg::SetUniform(const char* _uniform, const glm::vec3& _vec)
{
	GLint loc = getLocation(_uniform);
	glUniform3fv( loc, 1, &_vec[0] );
}

void ShaderProg::SetUniform(const char* _uniform, const glm::vec4& _vec)
{
	GLint loc = getLocation(_uniform);
	glUniform4fv(loc, 1, &_vec[0]);
}

void ShaderProg::SetUniform(const char* _uniform, const glm::mat4& _mat)
{
	GLint loc = getLocation(_uniform);
	glUniformMatrix4fv(loc, 1, GL_FALSE, &(_mat[0][0]));
}

void ShaderProg::SetUniform(const char* _uniform, int _i)
{
	GLint loc = getLocation(_uniform);
	glUniform1i( loc, _i );
}

void ShaderProg::SetUniform(const char* _uniform, float _f)
{
	GLint loc = getLocation(_uniform);
	glUniform1f( loc, _f );
}

void ShaderProg::SetUniform(const char* _uniform, float _a, float _b)
{
	GLint loc = getLocation(_uniform);
	glUniform2f( loc, _a, _b );
}

void ShaderProg::SetUniform(const char* _uniform, float _a, float _b, float _c)
{
	GLint loc = getLocation(_uniform);
	glUniform3f( loc, _a, _b, _c );
}

void ShaderProg::SetUniform(const char* _uniform, float _a, float _b, float _c, float _d)
{
	GLint loc = getLocation(_uniform);
	glUniform4f( loc, _a, _b, _c, _d );
}

void ShaderProg::SetTexture(const char* _uniform, int _sampler, GLuint _textureID)
{
	glActiveTexture(GL_TEXTURE0 + _sampler);
	glBindTexture(GL_TEXTURE_2D, _textureID);
	SetUniform(_uniform, _sampler);
}

void ShaderProg::SetCubeTexture(const char* _uniform, int _sampler, GLuint _textureID)
{
	glActiveTexture(GL_TEXTURE0 + _sampler);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _textureID);
	SetUniform(_uniform, _sampler);
}

inline GLuint	ShaderProg::getLocation(const char* _uniform)
{
	std::map< std::string, int >::iterator loc_it = uniform_locations.find(_uniform);
	if ( loc_it == uniform_locations.end() )
	{
		GLint my_loc = glGetUniformLocation(program, _uniform);
		uniform_locations[_uniform] = my_loc;
		return my_loc;
	}
	else
		return loc_it->second;
}
