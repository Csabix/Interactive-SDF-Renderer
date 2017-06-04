layout(index = 400 + 2) subroutine(Norm)
vec3 norm_2(const in vec3 p, const in vec3 n0, const in float epszilon)
{
	vec3 plus = vec3(sdf(p + vec3(epszilon,0,0), vec3(0)),
					 sdf(p + vec3(0,epszilon,0), vec3(0)),
					 sdf(p + vec3(0,0,epszilon), vec3(0)));
	vec3 mins = vec3(sdf(p - vec3(epszilon,0,0), vec3(0)),
					 sdf(p - vec3(0,epszilon,0), vec3(0)),
					 sdf(p - vec3(0,0,epszilon), vec3(0)));
	vec3 n =  normalize(plus-mins);
	return ( isnan(n.x) || isnan(n.y) || isnan(n.z) ? n0 : mix(n, n0, n_mix_coeff));
}