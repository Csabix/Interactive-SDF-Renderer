layout(index = 500 + 1) subroutine(Ambient)
float ambient_1(const in vec3 p, const in vec3 n, const in float ao)
{
	float radius = ao_iternum * ao_radius;
	return ao + pow(0.5, ao_iternum) * (radius - sdf(p + n*radius, vec3(0)));
}
