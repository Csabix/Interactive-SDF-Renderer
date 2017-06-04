//#define NEXT_R(r_0, r_1, d) (- r_1 * (d - r_0 + r_1)/(-d - r_0 + r_1))
//#define NEXT_R(r_0, r_1, d) 0.99*(- r_1 * (r_0 - r_1 - d)/(r_0 - r_1 + d))
/*vec2 trace_ray_30(const in Ray r, const in float stepsize, const in int stepcount)
{	//-(r_1 * (d-r_0+r_1))/(-d-r_0+r_1)
	float t = 0, ft = sdf(RAY(r,t)), pft = 0, dt = 0, nft;
	for(int i = 0; i < stepcount; ++i)
	{
		dt = ft + NEXT_R(pft, ft, dt);
		nft = sdf(RAY(r,t + dt));
		if(dt < ft + nft)
		{
			ft = nft;
			t += dt;
		}
		else
		{
			t += dt = ft;
			ft = sdf(RAY(r,t));
		}
		pft = ft;
	}
	return vec2(t, t + stepsize);
}*/
#define STEP_SIZE_REDUCTION 0.87

subroutine(Raytracer)
vec2 raytracer_30(const in Ray r, const in float stepsize, const in int stepcount)
{	//-(r_1 * (d-r_0+r_1))/(-d-r_0+r_1)
	float t = 0, ft = sdf(RAY(r,t)), pft = 0, dt = 0, nft;
	for(int i = 0; i < stepcount; ++i)
	{
		dt = ft * (1 - STEP_SIZE_REDUCTION * (pft - ft - dt)/(pft - ft + dt));
		//dt = ft + NEXT_R(pft, ft, dt);
		nft = sdf(RAY(r,t + dt));
		if(dt < ft + nft)
		{
			ft = nft;
			t += dt;
		}
		else
		{
			t += dt = ft;
			ft = sdf(RAY(r,t));
		}
		pft = ft;
	}
	return vec2(t, t + stepsize);
}