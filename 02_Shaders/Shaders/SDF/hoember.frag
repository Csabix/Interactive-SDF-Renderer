vec3 color(in vec3 p, in vec3 v)
{
	return vec3(1);
}

float sdf(in vec3 p, in vec3 v)
{	
	float fej = sphere(p+vec3(0,0,-6),v, 0.9);
	float szem1 = sphere(p+vec3(0.2,-0.8,-6.3), v,0.1);
	float szem2 = sphere(p+vec3(-0.2,-0.8,-6.3), v,0.1);
	float orr = sphere(p+vec3(0,-0.9,-6.0), v, 0.15);
	float alakzat1 = max(min(fej, orr), -min(szem1, szem2));
	float alakzat2 = sphere(p+vec3(0,0,-4.2), v, 1.4);
	float alakzat3 = sphere(p+vec3(0,0,-1.5), v, 2);
	return min(min(alakzat1, planeXY(p,v)), min(alakzat2, alakzat3));
}