layout(index = 600 + 2) subroutine(Shadow)
vec2 shadow_2(const in Ray r, const in vec3 n, const in vec3 depth,
			  const in vec2 prevstate, const in float pixelsize, const in bool changed)
{	//SOFT SHADOW
	float dist = distance(light_pos, r.p);
	if (sw_stepcount == 0) return vec2(1,0);
	if (!changed && (prevstate.y >= dist - light_size || prevstate.x <= 0.01)) return prevstate;
	
	vec3 l = normalize(light_pos - r.p);	
	if(dot(l, n) < 0) return vec2(0, prevstate.y);
	
	float t = (changed ? 0 : prevstate.y);
	float cone = (light_size - pixelsize)/dist;
	
	Ray r2  = {l, r.p};
	float m = (changed ? 1 : prevstate.x);
	for(int i = 0; i < sw_stepcount; ++i)
	{
		if(t >= dist - light_size) return vec2(m, t);
		float rt = t*cone + pixelsize;
		float ft = sdf(RAY(r2,t), r2.v) + rt;
		float newm = smoothstep(0,1,0.5*ft/rt);
		if(newm < m)
		{
			if(newm < 0.01) return vec2(newm,t);
			m = newm;
			t -= ft*sw_step_multipier*0.5; //step back to find smaller values
		}
		t += ft*sw_step_multipier;
	}
	return vec2(m,t);
}