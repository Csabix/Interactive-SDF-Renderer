layout(index = 500 + 2) subroutine(Ambient)
float ambient_2(const in vec3 p, const in vec3 n, const in float ao)
{
	return 1 - ao_strength * 2 * (ao_radius - sdf(p + n * ao_radius, vec3(0)));
}
