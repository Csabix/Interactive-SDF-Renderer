#include "Uniforms.h"
#include "GLCHECK.h"
#include "DebugWindow.h"

ShaderProg & operator<<(ShaderProg & prog, const Uniforms & uniforms)
{	//TODO: optimize to a few ogl calls (UBO?)
	prog.SetUniform("st_stepcount", uniforms.sphere_trace.stepcount);
	prog.SetUniform("st_stepsize" , uniforms.sphere_trace.stepsize);
	prog.SetUniform("st_stepincr" , uniforms.sphere_trace.stepincr);
	prog.SetUniform("ao_radius"   , uniforms.ambient_occolution.radius);
	prog.SetUniform("ao_strength" , uniforms.ambient_occolution.strength);

	prog.SetUniform("ao_mix_coeff", uniforms.ambient_occolution.mix_coeff);
	prog.SetUniform("sw_mix_coeff", uniforms.shadows.mix_coeff);
	prog.SetUniform("n_mix_coeff", uniforms.normal.mix_coeff);
	prog.SetUniform("n_epsz_mult", uniforms.normal.epsz_mult);
	prog.SetUniform("c_mix_coeff", uniforms.color_mix_coeff);

	prog.SetUniform("cam_eye", uniforms.camera.eye);
	prog.SetUniform("cam_at", uniforms.camera.at);
	prog.SetUniform("cam_up", uniforms.camera.up);
	prog.SetUniform("cam_pixel_growth", uniforms.camera.pixel_growth);
	prog.SetUniform("cam_normalized_size", uniforms.camera.normalized_size);
	prog.SetUniform("cam_res_mult", uniforms.resolution_multipiers.camera);
	prog.SetUniform("tex_res_mult", uniforms.resolution_multipiers.texture);

	prog.SetUniform("sw_stepcount", uniforms.shadows.stepcount);
	prog.SetUniform("light_pos", uniforms.shadows.light_pos);
	prog.SetUniform("light_size", uniforms.shadows.light_radius);
	prog.SetUniform("sw_step_multipier", uniforms.shadows.step_multipier);
	prog.SetUniform("ao_iternum", uniforms.ambient_occolution.ao_iternum);

	prog.SetUniform("mat_ambient_mult", uniforms.material.ambient);
	prog.SetUniform("mat_diffuse_mult", uniforms.material.diffuse);
	prog.SetUniform("mat_specular_mult", uniforms.material.specular);
	prog.SetUniform("mat_specular_power", uniforms.material.power);
	prog.SetUniform("mat_light_decay", uniforms.material.light_decay);
	prog.SetUniform("light_strength", uniforms.material.light_strength);
	prog.SetUniform("user_itercount", (int)uniforms.user_itercount);

	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 5, uniforms.subroutines);
	return prog;
}

Uniforms::Uniforms(GUI::DebugWindow & debug) : debug(debug)
{
	algorithms.spheretrace = ST_3;
	algorithms.antialias = ANTIALIAS_0;
	algorithms.norm = NORM_1;
	algorithms.ambient = AMBIENT_1;
	algorithms.shadow = SHADOW_4;
}

void Uniforms::setShowUniformsToShaderProg(ShaderProg & prog) const
{
	if(debug.original_size)
		prog.SetUniform("cam_res_mult", glm::vec2(1.0));
	else
		prog.SetUniform("cam_res_mult", (debug.show_currentstate ? resolution_multipiers.camera : resolution_multipiers.texture));
	//prog.SetUniform("tex_res_mult", tex_res_mult);
	prog.On();
	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &debug.view.type);
}

void Uniforms::Iteration(const int it)
{
	resolution_multipiers.texture = resolution_multipiers.camera;	//next texture resoltuion is the last camera resolution
	const float t = (float) it;

	float m = debug.functions.resolution_multipier(t);
	camera.pixel_growth = 1.41421356237f / glm::length(glm::vec2(w*m, h*m))*2.f;
	resolution_multipiers.camera= glm::vec2(m);
	camera.normalized_size = glm::vec2(w*m, h*m);
	camera.normalized_size = glm::normalize(camera.normalized_size) * debug.view.fow_mult;

	sphere_trace.stepcount = (int)debug.functions.spheretrace_stepcount(t);
	shadows.stepcount = (int)debug.functions.shadow_stepcount(t);
	user_itercount = (int)debug.functions.user_itercount(t);

	ambient_occolution.ao_iternum = it - debug.ambient.start_iternum;
	algorithms.ambient = (it <= debug.ambient.start_iternum ? Uniforms::AMBIENT_0 : Uniforms::AMBIENT_0 + debug.ambient.algorithm);
}