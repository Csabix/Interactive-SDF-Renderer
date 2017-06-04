#define STEP_SIZE_REDUCTION 0.87

//kb ugyanolyan jó mint a 22
subroutine(Raytracer)
vec2 raytracer_32(const in Ray r, const in float stepsize, const in int stepcount)
{	//-(r_1 * (d-r_0+r_1))/(-d-r_0+r_1)
	float t = 0, ft = sdf(RAY(r,t)), pft = 0, dt = 0, nft, msp = stepsize;
	for(int i = 0; i < stepcount; ++i)
	{
		msp += TRACE_STEPINCR;

		dt = ft * (1 - STEP_SIZE_REDUCTION * (pft - ft - dt)/(pft - ft + dt)) + msp;
		nft = sdf(RAY(r,t + dt));
		if(dt < ft + nft + msp*1.3)
		{
			ft = nft;
			t += dt;
		}
		else
		{
			t += dt = ft + msp;
			ft = sdf(RAY(r,t));
		}
		if(ft < 0) return vec2(t - msp,t);
		pft = ft;
	}
	return vec2(t - stepsize,t);
}