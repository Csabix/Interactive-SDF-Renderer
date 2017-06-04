vec3 hsv2rgb(const in vec3 c)
{
    const vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec3 color(vec3 p)
{
	if(abs(p.y) < 0.01*length(p) || abs(p.z-0.2) < 0.001*length(p)) return vec3(1,1,1);
	else return hsv2rgb(vec3(length(p)*2, 0.5, 0.5));
}

float sdf(in vec3 p)
{	//Mandelbulb from http://blog.hvidtfeldts.net/index.php/2011/09/distance-estimated-3d-fractals-v-the-mandelbulb-different-de-approximations/
	const float Power = 8.0;
	vec3 z = p;
	float dr = 1.0, r = 0.0;
	for (int i = 0; i < user_itercount ; i++) {
		r = length(z);
		if (r>=2) break;
		float theta = acos(z.z/r), phi = atan(z.y,z.x);
		dr =  pow( r, Power-1.0)*Power*dr + 1.0;
		float zr = pow( r, Power);
		theta = theta*Power;
		phi = phi*Power;
		z = zr*vec3(sin(theta)*cos(phi), sin(phi)*sin(theta), cos(theta));
		z+=p;
	}
	return min(0.5*log(r)*r/dr, min(p.z,p.y));
}