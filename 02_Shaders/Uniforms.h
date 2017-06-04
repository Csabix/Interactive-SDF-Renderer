#pragma once
#include <GL/glew.h>
#include "GPUState.h"
#include "ShaderProg.h"
#include "gCamera.h"
#include <glm/glm.hpp>
//#include "DebugWindow.h"

namespace GUI{	class DebugWindow;}	//fwd

class Uniforms
{
	friend ShaderProg& operator << (ShaderProg& prog, const Uniforms &uniforms);
	friend class GUI::DebugWindow;
public:
	enum ALGORITHM
	{
		/*ST_0 = 0,	*/		ST_1 = 1,			ST_2 = 2,			ST_3 = 3,
		REFINE_0 = 200,		REFINE_1 = 201,		REFINE_2 = 202,		REFINE_3 = 203,
		ANTIALIAS_0 = 300,
		NORM_0 = 400,		NORM_1 = 401,		NORM_2 = 402,
		AMBIENT_0 = 500,	AMBIENT_1 = 501,	AMBIENT_2 = 502,	AMBIENT_3 = 503,
		SHADOW_0 = 600,	SHADOW_1 = 601,			SHADOW_2 = 602,		SHADOW_3 = 603,		SHADOW_4 = 604,
	};
public:
	GUI::DebugWindow &debug;

	float	color_mix_coeff = 0.f;
	int user_itercount = 1;

	struct
	{
		int		stepcount = 10;
		float	stepsize	 = 0.1f;
		float	stepincr  = 0.1f;
	} sphere_trace;
	
	struct
	{
		float	radius = 0.1f;
		float	strength = 0.9f;
		float	mix_coeff = 0.f;
		int ao_iternum = 1;
	} ambient_occolution;
	
	struct
	{
		float	mix_coeff = 0.f;
		float	epsz_mult = 0.5f;
	} normal;
	struct
	{
		glm::vec3 eye = glm::vec3(2.f);
		glm::vec3 at = glm::vec3(0.f);
		glm::vec3 up = glm::vec3(0.f, 0.f, 1.f);

		float pixel_growth = 1.41421356237f / glm::length(glm::vec2(1280.f, 720.f));
		glm::vec2 normalized_size = glm::vec2(1280.f,720.f) / glm::length(glm::vec2(1280.f, 720.f));
	} camera;

	struct
	{
		glm::vec3 light_pos = glm::vec3(18.6f, -0.7f, 3.3f);
		float light_radius = 0.5f;
		int stepcount = 10;
		float step_multipier = 0.6f;
		float mix_coeff = 0.f;
	} shadows;

	union
	{
		GLuint subroutines[5];
		struct
		{
			GLuint spheretrace;
			GLuint antialias;
			GLuint norm;
			GLuint ambient;
			GLuint shadow;
		} algorithms;
	};

	struct
	{
		glm::vec2 camera = glm::vec2(1.0);
		glm::vec2 texture = glm::vec2(1.0);
	} resolution_multipiers;

	struct
	{
		float ambient = 0.18f;
		float diffuse = 0.76f;
		float specular = 0.98f;
		float power = 200.f;
		float light_decay = 0.1f;
		float light_strength = 2.3f;
	}material;

	int w = 1280, h = 720;

public:
	Uniforms(GUI::DebugWindow &debug);
	inline float GetCamResMultX() const { return resolution_multipiers.camera.x; }
	inline void setWH(int wi, int hi)
	{
		w = wi; h = hi;
		camera.pixel_growth = static_cast<float>(1.41421356237 / glm::length(glm::dvec2(wi,hi)));
		camera.normalized_size = glm::vec2(wi, hi);
		camera.normalized_size = glm::normalize(camera.normalized_size);
	}
	inline Uniforms& operator << (const gCamera &cam)
	{
		camera.eye = cam.GetEye();	camera.at = cam.GetAt();	camera.up = cam.GetUp();
		return *this;
	}
	void setShowUniformsToShaderProg(ShaderProg &prog) const;
	void Iteration(const int it);
};
ShaderProg& operator << (ShaderProg& prog, const Uniforms &uniforms);


// Possible improovemenst:
//	- Implement an abstact version of UBO
//	- May need multiple UBO-s to update content in different rate (per frame, per iteration)

