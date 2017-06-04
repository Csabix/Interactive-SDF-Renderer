subroutine(Raytracer)
vec2 raytracer_01(const in Ray r, const in float stepsize, const in int stepcount)
{	//pure raymarch
	float t = 0;
	for(int i = 0; i < stepcount; ++i)
	{
		if(sdf(RAY(r,t)) <= 0) return vec2(t-stepsize,t);
		t+=stepsize;
	}
	return vec2(t-stepsize,t);
}