layout(index = 600 + 1) subroutine(Shadow)
vec2 shadow_1(const in Ray r, const in vec3 n, const in vec3 depth,
			  const in vec2 prevstate, const in float pixelsize, const in bool changed)
{	//HARD SHADOW
	float dist = distance(light_pos, r.p);
	if (sw_stepcount == 0) return vec2(1,0);
	if (!changed && (prevstate.y >= dist || prevstate.x < 0.01)) return prevstate;
	
	vec3 l = (light_pos - r.p)/dist;	
	if(dot(l, n) < 0) return vec2(0,dist);
	
	float t = (changed ? depth.y : prevstate.y);
	
	Ray r2  = {l, r.p + n*pixelsize };
	for(int i = 0; i < sw_stepcount; ++i)
	{
		if(t >= dist)	return vec2(1, t);
		float ft = sdf(RAY(r2,t), r.v);
		if(ft < pixelsize) return vec2(0, t);
		t += ft;
	}
	return vec2(1,t);	//0.3 to check wich pixels get to the light_pos
}