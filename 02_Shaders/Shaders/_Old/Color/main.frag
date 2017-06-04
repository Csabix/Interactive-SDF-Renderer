out vec3 out_color;	//display color
out vec4 out_aoswn; //shadow, ambient and comp normal
out vec3 out_depth; //pixel cone dist, nearest dist, past surface dist

/*
//Alternative:
out vec4 out_sw_ao_norm; //shadow, ambient and compressed normal
vec2 compressNorm(vec3 n)
{
	return vec2(atan(n.xy),n.z);
}
vec3 expandNorm(vec2 cn)
{
	return vec3(cos(n.x), sin(n.y), n.z);
}
*/

uniform sampler2D in_color;
uniform sampler2D in_sw_ao;
uniform sampler2D in_depth;
uniform sampler2D in_norm;

subroutine float Refine(Ray r, vec2 range);
subroutine uniform Refine refine;

subroutine vec2 Antialias(inout Ray r, in const vec3 ranges);
subroutine uniform Antialias antialias;

subroutine vec3 Norm(Ray r);
subroutine uniform Norm norm;

subroutine float Ambient(Ray r, vec3 n);
subroutine uniform Ambient ambient;

subroutine float Shadow(Ray r, vec3 n);
subroutine uniform Shadow shadow;

void main()
{
	vec3 ranges = texture(in_depth, coords());
	vec2 range = antialias(camera(), ranges);
	float t = refine(r, range);
	float ao = 1, sw = 1; vec2 cl;
	if(t < 0)
		cl = background(r);
	else
	{
		r.p += r.c*t;
		vec3 n = norm(r.p);
		ao = ambient(r, n);
		sw = shadow(r, n);
		cl = shade(r, n, a, s);
	}
	out_color.xyz = cl;
	out_sw_ao.xy = vec2(ao, sw);
}
	