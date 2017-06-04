subroutine(Raytracer)
vec2 raytracer_02(const in Ray r, const in float stepsize, const in int stepcount)
{	//faster raymarch (about same as pure on 1/2 TRACE_STEPSIZE)
	float t = 0, msp = stepsize;
	for(int i = 0; i < stepcount; ++i)
	{
		msp += TRACE_STEPINCR;
		if(sdf(RAY(r,t)) <= 0) return vec2(t-msp,t);
		t += msp;
	}
	return vec2(t-msp,t);
}