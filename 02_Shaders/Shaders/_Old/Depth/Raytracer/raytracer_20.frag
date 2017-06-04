subroutine(Raytracer)
vec2 raytracer_20(const in Ray r, const in float stepsize, const in int stepcount)
{	//relaxed sphere trace (if low step count glassy artifact instead)
	float t = 0, ft, pft = 0;
	const float or = TRACE_OR;
	for(int i = 0; i < stepcount; ++i)
	{
		ft = sdf(RAY(r,t));
		if(ft < pft*(or-1))
		{
			t -= (or-1)*pft;
			ft = sdf(RAY(r,t));
		}
		t += ft*or;
		pft = ft;
	}
	return vec2(t, t + stepsize);
}