#pragma once
#include <GL/glew.h>

class Image2D
{
	GLuint id;				//name
	GLsizei w, h;			//width, height
	GLuint i, f, filter;	//internalFormat, format
public:
	Image2D() : w(0), h(0) { glGenTextures(1, &id); }
	~Image2D() { glDeleteTextures(1, &id); }

	void set(GLsizei w, GLsizei h, int channels = 4, int bytesPerChannel = 32,
			bool nearest_filter = false, const GLvoid * data = nullptr);
	inline GLsizei GetW() const { return w; };
	inline GLsizei GetH() const { return h; };

	//void update(const GLvoid *data, GLsizei wi = 0, GLsizei hi = 0, GLsizei x = 0, GLsizei y = 0);
	inline GLuint get() const { return id; }

	Image2D(const Image2D&) = delete;
	Image2D& operator = (const Image2D&) = delete;
};

