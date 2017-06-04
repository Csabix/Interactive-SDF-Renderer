#pragma once
#include <GL/glew.h>
#include <vector>

class FBO
{
protected:
	std::vector<GLenum> buffs;
	static GLuint bound_fbo;
	GLuint fbo = bound_fbo;
public:
	FBO();
	~FBO();

	FBO(const FBO&) = default;
	FBO& operator = (const FBO&) = default;

	void addAttachement(const GLuint im);
	inline void clear() { buffs.clear(); }
	void finalize();
	void check() const ;
	inline void bind() const
	{
		if(bound_fbo != fbo)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			bound_fbo = fbo;
		}
	}
	static inline void Default()
	{
		if(bound_fbo != 0)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			bound_fbo = 0;
		}
	}
};