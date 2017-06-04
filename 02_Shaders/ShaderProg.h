#pragma once
#include <GL/glew.h>
#include <vector>
#include <map>
#include <glm\glm.hpp>
#include <iostream>
//typedef std::vector<std::string> SourceCode;

typedef std::vector<const char*> SourceCode;

class ShaderProg
{
public:
	ShaderProg(std::ostream &out = std::cout);
	~ShaderProg(void);

	ShaderProg(const ShaderProg&) = default;
	ShaderProg& operator = (const ShaderProg&) = default;


	bool AttachShader(GLenum _shaderType, const SourceCode &code);
	inline bool AttachShader(GLenum _shaderType, const char* code) { return AttachShader(_shaderType, SourceCode({code})); }
	inline bool AttachShader(GLenum _shaderType, const std::string &code) { return AttachShader(_shaderType, { code.c_str() }); }
	bool Link();

	void BindAttribLoc(int _index, const char* name);
	void BindFragDataLoc(int _index, const char* name);

	inline void SetVerbose(bool _val) { m_verbose = _val; }
	inline void On() const	//optimized for multiple updates :)
	{
		if(used_program != program)
		{
			glUseProgram(program);
			used_program = program;
		}
	}
	inline void Off() const { glUseProgram(0); };

	void SetUniform(const char* _uniform, const glm::vec2& _vec);
	void SetUniform(const char* _uniform, const glm::vec3& _vec);
	void SetUniform(const char* _uniform, const glm::vec4& _vec);
	void SetUniform(const char* _uniform, const glm::mat4& _mat);
	void SetUniform(const char* _uniform, int _i);
	void SetUniform(const char* _uniform, float _f);
	void SetUniform(const char* _uniform, float _a, float _b, float _c, float _d);
	void SetUniform(const char* _uniform, float _a, float _b, float _c);
	void SetUniform(const char* _uniform, float _a, float _b);
	void SetTexture(const char* _uniform, int _sampler, GLuint _textureID);
	void SetCubeTexture(const char* _uniform, int _sampler, GLuint _textureID);

	GLuint program;
protected:
	static GLuint used_program;
	bool compileSourceCode(const SourceCode & code, GLuint loadedShader, GLenum shaderType);
	GLuint	getLocation(const char* _uniform);
	std::map<std::string, GLint> uniform_locations;
	std::map<GLenum, GLuint> shader_type_names;
	bool m_verbose;
	std::ostream &out;
};

//Possible Improovements:
// - A new class for ShaderStage
		// ==> Easier creation of ShaderPrograms
		// ==> Subroutine support
// - UBO support : UBO class..