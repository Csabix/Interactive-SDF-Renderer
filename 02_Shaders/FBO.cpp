#include "FBO.h"
#include "GLCHECK.h"
#include <iostream>

GLuint FBO::bound_fbo = 0;

FBO::FBO()
{
	glGenFramebuffers(1, &fbo);
}
FBO::~FBO()
{
	glDeleteFramebuffers(1, &fbo);
}
void FBO::addAttachement(const GLuint im)
{
	bind();
	GLenum color_attachment_i = GL_COLOR_ATTACHMENT0 + buffs.size();
		//glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, color_attachment_i, GL_TEXTURE_2D, im, 0);
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, color_attachment_i, im, 0);
	buffs.push_back(color_attachment_i);
}
void FBO::finalize()
{
	glDrawBuffers(buffs.size(), buffs.data());
}
void FBO::check() const
{
	switch(glCheckFramebufferStatus(GL_FRAMEBUFFER))
	{
#define E "Incomplete framebuffer : "
	case GL_FRAMEBUFFER_COMPLETE: break; //OK
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: std::cout << (E "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n"); exit(1); break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: std::cout << (E "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n"); exit(1); break;
	case GL_FRAMEBUFFER_UNSUPPORTED: std::cout << (E "GL_FRAMEBUFFER_UNSUPPORTED\n"); exit(1); break;
	default: std::cout << (E "Unknown FBO Error\n"); exit(1); break;
#undef E
	}
}