layout(index = 0) subroutine(SphereTrace)
vec3 spheretrace_00(const in Ray r, in float t, in float ft)
{
	return vec3(t, ft, 0);
}