//depth main shader

uniform sampler2D in_depth;

subroutine vec3 Raytracer(const in Ray r, const in float stepsize, const in int stepcount);
subroutine uniform Raytracer raytrace;
//TODO: update raytrace functions!!

subroutine float GetDepth();
subroutine uniform GetDepth getDepth;

uniform int stepcount;
uniform float stepsize;

out vec3 out_depth;

void main()
{
	Ray r = camera();
	r.p += r.v * getDepth();
	out_depth = raytrace(r, stepsize, stepcount);
}