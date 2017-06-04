#define TRACE_OR 1.6

layout(index = 0 + 2) subroutine(SphereTrace)
vec3 spheretrace_2(const in Ray r, in float t, float ft)
{
	float pft = 0, dt = 0, nft;
	for(int i = 0; i < st_stepcount; ++i)
	{
		dt = ft * TRACE_OR;
		nft = sdf(RAY(r,t + dt), r.v);
		if(dt < ft + nft)
		{
			ft = nft;
			t += dt;
		}
		else
		{
			dt = ft;
			t += dt;
			ft = sdf(RAY(r,t), r.v);
		}
		if(ft < t * cam_pixel_growth)
			return vec3(t-dt, sdf(RAY(r, t-dt), r.v), float(i));
		pft = ft;
	}
	return vec3(t, ft, float(st_stepcount));
}