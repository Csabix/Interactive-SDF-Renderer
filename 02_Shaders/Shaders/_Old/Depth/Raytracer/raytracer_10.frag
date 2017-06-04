subroutine(Raytracer)
vec2 raytracer_10(const in Ray r, const in float stepsize, const in int stepcount)
{	//pure sphere trace (if low step count, bubbley artifact)
	float t = 0;
	for(int i = 0; i < stepcount; ++i)
		t += sdf(RAY(r,t));
	return vec2(t, t+stepsize);
}