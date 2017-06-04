#define STEP_SIZE_REDUCTION 0.87

layout(index = 0 + 3*3+2) subroutine(SphereTrace)
vec3 spheretrace_32(const in Ray r, float t, float ft)
{	//-(r_1 * (d-r_0+r_1))/(-d-r_0+r_1)
	float pft = 0, dt = 0, nft, msp = st_stepsize;
	for(int i = 0; i < st_stepcount; ++i)
	{
		msp += st_stepincr;

		dt = ft * (1 - STEP_SIZE_REDUCTION * (pft - ft - dt) / (pft - ft + dt)) + msp;
		nft = sdf(RAY(r, t + dt), r.v);
		if(dt < ft + nft + msp * 1.3)
		{
			ft = nft;
			t += dt;
		}
		else
		{
			dt = ft + msp;
			t += dt;
			ft = sdf(RAY(r,t), r.v);
		}
		if(ft < t * cam_pixel_growth) return vec3(t - dt, sdf(RAY(r,t - dt), r.v), i);
		pft = ft;
	}
	return vec3(t, ft, st_stepcount);
}