//IN OUT
noperspective in vec2 vs_tex;
noperspective in vec2 vs_cam_coeffs;

layout(location = 0) out vec4 out_nearest_0;
layout(location = 1) out vec4 out_linear_1;
layout(location = 2) out vec4 out_linear_2;

struct Ray{ vec3 v, p; };
#define RAY(r,t) (r.v*(t)+r.p)	//point on a ray

//SUBROUTINES
subroutine vec3 SphereTrace(const in Ray r, in float t, in float ft);
subroutine vec3  Antialias(const in Ray r, const in vec4 depth);
subroutine vec3  Norm(const in vec3 p, const in vec3 n0, const in float epszilon);
subroutine float Ambient(const in vec3 p, const in vec3 n, const in float ao);
//subroutine float Shadow(const in vec3 p, const in vec3 n, const in float sw);

//returns vec2(shadow multipier, travelled distance)
subroutine vec2 Shadow(const in Ray r, const in vec3 n, const in vec3 depth,
					   const in vec2 prevstate, const in float pixelsize, const in bool changed);

//UNIFORMS
layout(binding = 0) uniform sampler2D in_nearest_0; //0
layout(binding = 1) uniform sampler2D in_linear_1;	//1
layout(binding = 2) uniform sampler2D in_linear_2;	//2

layout(location = 0) subroutine uniform SphereTrace	spheretrace;	//index 0xy
layout(location = 1) subroutine uniform Antialias	antialias;  	//index 3xy
layout(location = 2) subroutine uniform Norm		norm;       	//index 4xy
layout(location = 3) subroutine uniform Ambient		ambient;    	//index 5xy
layout(location = 4) subroutine uniform Shadow		shadow;     	//index 6xy

uniform int		st_stepcount	= 10;
uniform float	st_stepsize 	= 0.01;
uniform float	st_stepincr 	= 0.01;

uniform float	ao_radius   	= 0.01;
uniform float	ao_strength 	= 0.01;
uniform int		ao_iternum		= 1;
uniform float	ao_mix_coeff	= 0;

uniform float	sw_mix_coeff = 0;
uniform float	n_mix_coeff = 0;
uniform float 	n_epsz_mult = 1.0;
uniform float	c_mix_coeff = 0;

uniform vec3  cam_eye = vec3(2);
uniform vec3  cam_at  = vec3(0);
uniform vec3  cam_up  = vec3(0 , 0 , 1);
uniform float cam_pixel_growth = SQRT2 / length(vec2(640 , 480)) ;

uniform int sw_stepcount = 10;
uniform vec3 light_pos = vec3(30,30,30);
uniform float light_size = 1;
uniform float light_strength = 2.0;
uniform float sw_step_multipier = 0.65;

uniform float mat_ambient_mult = 0.3;
uniform float mat_diffuse_mult = 0.7;
uniform float mat_specular_mult = 0.95;
uniform float mat_specular_power = 200;
uniform float mat_light_decay = 10.f;