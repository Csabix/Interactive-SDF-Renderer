vec3 color(in vec3 p, in vec3 v)
{
	return vec3(1);
}

float gizmo(in vec3 p, vec2 t, float k)
{
	float f = 	 torus(p.xyz,t);
	f = smin(f, torus(p.yzx,t),k);
	f = smin(f, torus(p.zxy,t),k);
	return f;
}

float dice(in vec3 p, in float r)
{
	float f = sphere(p, r*SQRT2);
	f = smax(f, box(p, vec3(r)), 0.035*r);
	return f;
}

float plus(in vec3 p, in vec2 r)
{
	float f = 	cylinder(p.xyz, r.x);
	f = smin(f, cylinder(p.yzx, r.x), r.y);
	f = smin(f, cylinder(p.zxy, r.x), r.y);
	return f;
}

float testcube(in vec3 p, in vec3 r)
{
	return  smax(dice(p, r.x), -plus(p, r.yz), r.z);
}

float viviani(in vec3 p, in float r)
{
	return max(sphere(p, r), cylinder(p + vec3(r*0.5,0,0), r*0.5)); 
}

float sdf(in vec3 p, in vec3 v)
{
	float pl = planeXY(p, v);
	float g = gizmo(p - vec3(0,0,8), vec2(8,1.25), 0.75);
	float cs = testcube(p - vec3(10,10,8), vec3(8,6,0.5));
	float vi = viviani(p - vec3(-10,-10,8), 8);
	
	return min(min(pl,vi),min(g, cs));
}
