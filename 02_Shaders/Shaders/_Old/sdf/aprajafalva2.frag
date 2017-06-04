vec3 color(vec3 p)
{
	return vec3(1);
}

float sdf(in vec3 p)
{
	p.xy = repeat(p.xy, vec2(21,16));
	float haztest = box(p + vec3(0,0,-2), vec3(5,4,2.5));
	float hazbelso = box(p + vec3(0,0,-2), vec3(4,3,2));
	float ajto = box(p + vec3(4.5,0.5,-2), vec3(1,1,2));
	float ablak1 = box(p + vec3(2,0,-2.8), vec3(0.7,10,1));
	float ablak2 = box(p + vec3(-2,0,-2.8), vec3(0.7,10,1));
	float ablakok = min(ablak1, ablak2);
	float haz = max(max(max(haztest, -hazbelso), -ajto), -ablakok);
	vec3 p1 = p + vec3(0,0,-4.5);
	float teto = max(sphere(p1, 6), -planeXY(p1));
	return min(min(haz, teto), planeXY(p));
}