#define STEP_SIZE_REDUCTION 0.95
//#define STEP_SIZE_REDUCTION 0.87

//Performs a sphere-step
// r a ray on wich along the tracing happens
// c0, c1 are the previous cones
// the result is the next cone
// A
/*
vec2 sphere_step(const in Ray r, in vec2 c0, in vec2 c1)
{
	float dt = c0.y * (1 - STEP_SIZE_REDUCTION * (c1.y - c0.y - (c1.x - c0.x))/(c1.y - c0.y + (c1.x - c0.x)));
	float ft = sdf(RAY(r, c0.x + dt));
	if(dt < c0.y + c1.y)
		return vec2();
}*/

#define NEXT_R(r_0, r_1, d) STEP_SIZE_REDUCTION*(- r_1 * (r_0 - r_1 - d)/(r_0 - r_1 + d))

layout(index = 0 + 3) subroutine(SphereTrace)
vec3 spheretrace_3(const in Ray r, in float t, float ft)
{	//-(r_1 * (d-r_0+r_1))/(-d-r_0+r_1)
	float pft = 0, dt = 0, nft;
	for(int i = 0; i < st_stepcount; ++i)
	{
		//dt = ft * (1 - STEP_SIZE_REDUCTION * (pft - ft - dt)/(pft - ft + dt));
		dt = ft + NEXT_R(pft, ft, dt);
		nft = sdf(RAY(r,t + dt), r.v);
		if(dt < ft + nft)
		{
			ft = nft;
			t += dt;
		}
		else
		{
			dt = ft;
			t += dt;
			ft = sdf(RAY(r,t), r.v);
		}
		if(ft < t * cam_pixel_growth)
			return vec3(t - dt, sdf(RAY(r, t - dt), r.v), float(i));
		pft = ft;
	}
	return vec3(t, ft, float(st_stepcount));
}