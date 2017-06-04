layout(index = 0 + 1) subroutine(SphereTrace)
vec3 spheretrace_1(const in Ray r, in float t, float ft)
{	//pure sphere trace (low step count causes bubbley artifact)
	for(int i = 0; i < st_stepcount; ++i)
	{
		ft = sdf(RAY(r,t), r.v);
		if(ft < t * cam_pixel_growth) return vec3(t - ft, sdf(RAY(r, t-ft), r.v), float(i));
		t += ft;
	}
	return vec3(t, ft, float(st_stepcount));
}