#define SQRT2 1.4142135623
#define PI 3.14159265359
#define EPSZILON 1e-4
#define no_konvex_opt

//repeat-s
#define repeat(p,r) (mod(p+r*0.5,r)-r*0.5)
#define repeatXY(p,r) vec3(repeat((p).xy,r),(p).z)
#define repeatYZ(p,r) vec3(repeat((p).yz,r),(p).x)
#define repeatXZ(p,r) vec3(repeat((p).xz,r),(p).y)

//deprected stuff:

float cylinder(const in vec3 p,  float r )
{
	return length(p.xy)-r;
}

float sphere(const in vec3 p, const in float r)
{
	return length(p)-r;
}
float box(const in vec3 p, const in vec3 size)
{
	vec3 d = abs(p) - size;
	return min(max(d.x,max(d.y,d.z)),0.0) + length(max(d,0.0));
}
float cone( vec3 p, vec2 c )
{    // c must be normalized
    float q = length(p.xy);
    return dot(c,vec2(q,p.z));
}

float torus( vec3 p, vec2 t )
{
  vec2 q = vec2(length(p.xy)-t.x, p.z);
  return length(q)-t.y;
}

#define konvex_optimize(f, vdf) f	//no optimalization
//#define konvex_optimize(f, vdf) (abs(vdf) < 0.01 ? f : (vdf < 0 && f > 0 ? 2000 : f/abs(vdf*1.1) ))

//planes

float plane(in vec3 p, in vec3 v, in vec3 n)
{	//length(n) must be 1
	float f = dot(p, n);
	float vf = dot(-v, n);
	return f;
	return konvex_optimize(f, vf);
}
#define planeYZ(p, v) plane(p,v, vec3(1,0,0))
#define planeXZ(p, v) plane(p,v, vec3(0,1,0))
#define planeXY(p, v) plane(p,v, vec3(0,0,1))

//convex stuff:

float sphere(const in vec3 p, const in vec3 v, const in float r)
{
	float f = length(p)-r;
	float vf = dot(v, normalize(p));
	return konvex_optimize(f, vf);
}
float box(const in vec3 p, const in vec3 v, const in vec3 size)
{
	vec3 d = abs(p) - size;
	return min(max(d.x,max(d.y,d.z)),0.0) + length(max(d,0.0));
}
float cone( const in vec3 p, const in vec3 v, const in vec2 c )
{    // c must be normalized
    float q = length(p.xy);
    return dot(c,vec2(q,p.z));
}
float cylinder(const in vec3 p, const in vec3 v, const in float r )
{
	return length(p.xy)-r;
}

//combinators:

float smin( float a, float b, float k )
{	// polynomial smooth min (k = 0.1);
    float h = clamp( 0.5+0.5*(b - a)/k, 0.0, 1.0 );
    return mix( b, a, h ) - k*h*(1.0-h);
}
float smax( float a, float b, float k )
{	// polynomial smooth min (k = 0.1);
    float h = clamp( 0.5+0.5*(a - b)/k, 0.0, 1.0 );
    return mix( b, a, h ) + k*h*(1.0-h);
}
float metasdf(const in float dist, const in float r)
{
	return smoothstep(r,0,dist)*2.0/3.0;
}

//user uniforms

layout(location = 26) uniform int user_itercount = 1;

//eof
