#include "GPUState.h"

void GPUState::set(GLsizei w, GLsizei h)
{	//All Image2D has type RGBA32F
	nearest_0.set(w, h, 4, 32, true);	//nearest
	linear_1.set(w, h, 4, 32, false);	//linear
	linear_2.set(w, h, 4, 32, false);	//linear
	linear_3.set(w, h, 4, 32, false);	//linear
	fbo.clear();
	fbo.addAttachement(nearest_0.get());
	fbo.addAttachement(linear_1.get());
	fbo.addAttachement(linear_2.get());
	fbo.addAttachement(linear_3.get());
	fbo.finalize();
	fbo.check();
}

void GPUState::bindTextures()
{
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, nearest_0.get());
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, linear_1.get());
	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, linear_2.get());
	glActiveTexture(GL_TEXTURE0 + 3);
	glBindTexture(GL_TEXTURE_2D, linear_3.get());
}
