vec3 color(vec3 p)
{
	return vec3(1);
}

float sdf(in vec3 p)
{
	return sphere(repeat(p, vec3(5,5,5)), 1);
	/*float sik = planeXY(p + vec3(0,0,0.7));
	float gomb1 = sphere(repeatXY(p, vec2(2,5)), 1);
	float gomb2 = sphere(repeatXY(p, vec2(3,6)) - vec3(0,0,1.5), 0.5);
	//float gomb3 = sphere(repeatXY(p, vec2(13,11)) - vec3(0,0,14), 3);
	return min(min(gomb1, gomb2), sik);*/
}