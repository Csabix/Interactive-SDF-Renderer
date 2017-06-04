subroutine(Raytracer)
vec2 raytracer_11(const in Ray r, const in float stepsize, const in int stepcount)
{	//sphere trace + raymarch
	float t = 0, ft;
	for(int i = 0; i < stepcount; ++i)
	{
		ft = sdf(RAY(r,t));
		if (ft <= 0) return vec2(t-stepsize, t);
		t += ft + stepsize;
	}
	return vec2(t-stepsize, t);
}