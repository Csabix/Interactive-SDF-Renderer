subroutine(Raytracer)
vec2 raytracer_12(const in Ray r, const in float stepsize, const in int stepcount)
{	//sphere trace + faster raymarch
	float t = 0, ft, msp = stepsize;
	for(int i = 0; i < stepcount; ++i)
	{
		ft = sdf(RAY(r,t));
		msp += TRACE_STEPINCR;
		if (ft <= 0) return vec2(t-msp, t);
		t += ft + msp;
	}
	return vec2(t-msp,t);
}