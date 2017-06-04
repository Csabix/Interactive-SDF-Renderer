#include "Image2D.h"
#include "GLCHECK.h"

void setImageFormats(int channels, int bytesPerChannel, GLuint &i, GLuint &f)
{
	switch(channels)
	{
	case 1: f = GL_RED;
		switch(bytesPerChannel)
		{
		case 8:  i = GL_R8; break;
		case 16: i = GL_R16F; break;
		case 32: i = GL_R32F; break;
		default: glmessage("Invalid bytes for a one channel image."); break;
		}
		break;
	case 2: f = GL_RG;
		switch(bytesPerChannel)
		{
		case 8:  i = GL_RG8; break;
		case 16: i = GL_RG16F; break;
		case 32: i = GL_RG32F; break;
		default: glmessage("Invalid bytes for a two channel image."); break;
		}
		break;
	case 3: f = GL_RGB;
		switch(bytesPerChannel)
		{
		case 3:  i = GL_R3_G3_B2; break;
		case 4:  i = GL_RGB4; break;
		case 5:  i = GL_RGB5; break;
		case 8:  i = GL_RGB8; break;
		case 9:  i = GL_RGB9_E5; break;
		case 10: i = GL_RGB10; break;
		case 11: i = GL_R11F_G11F_B10F; break;
		case 12: i = GL_RGB12; break;
		case 16: i = GL_RGB16F; break;
		case 32: i = GL_RGB32F; break;
		default: glmessage("Invalid bytes for a two channel image."); break;
		}
		break;
	case 4: f = GL_RGBA;
		switch(bytesPerChannel)
		{
		case 3:  i = GL_R3_G3_B2; break;
		case 4:  i = GL_RGBA4; break;
		case 5:  i = GL_RGB5_A1; break;
		case 8:  i = GL_RGBA8; break;
		case 10: i = GL_RGB10_A2; break;
		case 12: i = GL_RGBA12; break;
		case 16: i = GL_RGBA16F; break;
		case 32: i = GL_RGBA32F; break;
		default: glmessage("Invalid bytes for a two channel image."); break;
		}
		break;
	default: glmessage("Invalid number of channels"); break;
	}
}

void Image2D::set(GLsizei wi, GLsizei he, int channels, int bytesPerChannel, bool nearest_filter, const GLvoid * data)
{
	w = wi; h = he;
	if(0 != w && 0 != h)
	{
		glDeleteTextures(1, &id);
		glGenTextures(1, &id);
	}
	setImageFormats(channels, bytesPerChannel, i, f);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, i, w, h, 0, f, GL_FLOAT, data);
	filter = (nearest_filter ? GL_NEAREST : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

/*void Image2D::update(const GLvoid * data, GLsizei wi, GLsizei hi, GLsizei x, GLsizei y)
{
	glTextureSubImage2D(id, 0, x, y, (0 == wi ? w - x : wi), (0 == hi ? h - y : hi), f, GL_FLOAT, data);
	glcheck("Error updating texture.");
}*/
