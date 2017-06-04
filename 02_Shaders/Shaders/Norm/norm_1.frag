layout(index = 400 + 1) subroutine(Norm)
vec3 norm_1(const in vec3 p, const in vec3 n0, const in float epszilon)
{
	const vec3 a = vec3( 1, 1, 1);
	const vec3 b = vec3( 1,-1,-1);
	const vec3 c = vec3(-1, 1,-1);
	const vec3 d = vec3(-1,-1, 1);
	vec3 n	= a * sdf(a * epszilon + p, vec3(0))
			+ b * sdf(b * epszilon + p, vec3(0))
			+ c * sdf(c * epszilon + p, vec3(0))
			+ d * sdf(d * epszilon + p, vec3(0));
	//return normalize(n);
	n = normalize(n);
	//return mix(n, n0, n_mix_coeff);
	return ( isnan(n.x) || isnan(n.y) || isnan(n.z) ? n0 : mix(n, n0, n_mix_coeff));
}