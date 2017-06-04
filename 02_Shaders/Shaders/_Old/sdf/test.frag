vec3 color(vec3 p)
{
	return vec3(1);
}

float sdf(in vec3 p)
{
	float m = 10000;
	for (int i=0; i < 10; ++i)
	{
		float t = 0.1*i*3.1415*2.0;
		vec3 d = 7*vec3(cos(t), sin(t), 0);
		m = min(m, cylinder(p-d, 1));
	}
	return smin(planeXY(p), m, 1);
}