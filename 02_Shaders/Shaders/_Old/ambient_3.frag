layout(index = 500 + 3) subroutine(Ambient)
float ambient_3(const in vec3 p, const in vec3 n, const in float ao)
{
	float sum = 0;
	sum += 2.000 * (1*ao_radius - sdf(p+n*1*ao_radius, vec3(0)));
	sum += 1.000 * (2*ao_radius - sdf(p+n*2*ao_radius, vec3(0)));
	sum += 0.500 * (3*ao_radius - sdf(p+n*3*ao_radius, vec3(0)));
	sum += 0.250 * (4*ao_radius - sdf(p+n*4*ao_radius, vec3(0)));
	sum += 0.125 * (5*ao_radius - sdf(p+n*5*ao_radius, vec3(0)));
	return clamp(1 - ao_strength * sum, 0, 1);
}