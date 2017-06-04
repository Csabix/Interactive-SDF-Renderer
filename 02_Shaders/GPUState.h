#pragma once
#include "FBO.h"
#include "Image2D.h"

class GPUState
{
protected:
	FBO fbo;
public:
	Image2D nearest_0, linear_1, linear_2, linear_3;
	GPUState() {}
	void set(GLsizei w, GLsizei h);
	inline void on() const { fbo.bind(); }
	friend class Uniforms;
	GPUState(const GPUState&) = delete;
	GPUState& operator = (const GPUState&) = delete;
	void bindTextures();
};