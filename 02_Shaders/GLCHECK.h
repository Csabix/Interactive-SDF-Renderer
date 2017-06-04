#pragma once
#ifdef _DEBUG
#include <iostream>
#include <GL/glew.h>
static int GL_CHECK_max_errors = 0;
#define glcheck(strmsg) gl_checkfun(strmsg, __LINE__, __FILE__)
#define glmessage(msg) std::cout << msg << std::endl
#define glassert(expr) if(!(expr)) gl_assertfun(#expr, __LINE__, __FILE__)

static const char* getErrorCode(GLenum error)
{
	switch(error)
	{
	case GL_INVALID_ENUM: return "GL_INVALID_ENUM"; break;
	case GL_INVALID_VALUE: return "GL_INVALID_VALUE"; break;
	case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION"; break;
	case GL_STACK_OVERFLOW: return "GL_STACK_OVERFLOW"; break;
	case GL_STACK_UNDERFLOW: return "GL_STACK_UNDERFLOW"; break;
	case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY"; break;
	case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
		//case GL_CONTEXT_LOST: return "GL_CONTEXT_LOST"; break;
		//case GL_TABLE_TOO_LARGE1: return "GL_TABLE_TOO_LARGE1"; break;
	case GL_NO_ERROR: return "No error"; break;
	default: return "Unknown error"; break;
	}
}

inline void gl_checkfun(char * __restrict msg, int line, char * __restrict file)
{
	GLenum error = glGetError(); \
		if(error != GL_NO_ERROR)\
		{
			std::cout << "glcheck : " << msg << " : " << getErrorCode(error)
				<< " (" << error << ") at " << line << ":" << file << std::endl;
			if(--GL_CHECK_max_errors <= 0)\
				throw std::exception(msg);
		}
}
inline void gl_assertfun(char * __restrict msg, int line, char * __restrict file)
{
	std::cout << "glassert : " << msg << " failed at " << line << " : " << file << std::endl;
}

#else
#define glcheck(str)
#define glassert(expr) (expr)
#define glmessage(msg)
#endif