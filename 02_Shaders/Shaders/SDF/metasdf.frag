vec3 color(vec3 p, in vec3 v)
{
	float a = metasdf(box(p,vec3(0.2,0.2,0.07)), 0.3);
	float b = metasdf(torus(p + vec3(0.5,0.5,0), vec2(0.5,0.04)), 0.5);
	float c = metasdf(sphere(p - vec3(0, 1.1, 0),0.01), 1);
	float d = metasdf(sphere(p - vec3(1, 0.5, 0),0.01), 1);
	float e = metasdf(p.z+0.4,0.3);
	float sum = a*a + b*b + c*c + d*d + sqrt(e);
	return vec3(0,1,0)*(a*a)/sum +
	       vec3(1,0,1)*(b*b)/sum +
	       vec3(0,0,1)*(c*c)/sum +
		   vec3(1,0,0)*(d*d)/sum +
	       vec3(1,1,1)*(sqrt(e))/sum;
}

float sdf(in vec3 p, in vec3 v)
{
	float m = 0;
	m += metasdf(box(p, v, vec3(0.2,0.2,0.07)), 0.3);
	m += metasdf(torus(p + vec3(0.5,0.5,0), vec2(0.5,0.04)), 0.5);
	m += metasdf(sphere(p - vec3(0, 1.1, 0), v,0.01), 1);
	m += metasdf(sphere(p - vec3(1, 0.5, 0), v,0.01), 1);
	return smin((0.5- m)*0.25, planeXY(p + vec3(0,0,0.2), v), 0.1);
}