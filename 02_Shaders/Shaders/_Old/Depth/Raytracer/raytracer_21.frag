subroutine(Raytracer)
vec2 raytracer_21(const in Ray r, const in float stepsize, const in int stepcount)
{	//relaxed sphere trace with raymarch
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
		if(ft <= 0) return vec2(t - stepsize,t);
		t += ft*or + stepsize;
		pft = ft;
	}
	return vec2(t -stepsize,t);
}