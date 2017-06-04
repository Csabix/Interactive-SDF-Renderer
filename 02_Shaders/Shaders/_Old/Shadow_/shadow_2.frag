layout(index = 600 + 2) subroutine(Shadow)
float shadow_2(const in vec3 p, const in vec3 n, const in float sw)
{
	vec3 l = light(p);
	if(dot(l,n) > 0)
	{
		float t = 0.1;
		Ray r  = {l, p + n*0.1 };
		for(int i = 0; i < st_stepcount; ++i)
		{
			float ft = sdf(RAY(r,t));
			if(ft < 0.1) return 0;
			t += ft;
		}
		return 1;
	}
	return 0;
}