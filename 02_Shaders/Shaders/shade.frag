vec3 light(const in vec3 p)
{
	return normalize(light_pos - p);
}

vec3 skylight(const in vec3 p, const in vec3 v)
{
	vec3 sky =  mix(vec3(0.046,0.2,0.047), vec3(0.41,0.51,1.1),v.z*0.5 + 0.5);
	//float x = dot(v, normalize(light_pos))*0.5+0.5;
	//vec3 sun = mix(vec3(0.1,0.05,0.05), vec3(1, 0.9, 0.5),  pow(x,1000*light_size));
	return sky;
}

vec3 shade(const in Ray r, const in vec3 n, const in float ao, const in float sw)
{
	vec3 col = color(r.p, r.v);
	vec3 l = light(r.p);
	float lightness = sw * light_strength /(1 + mat_light_decay*distance(r.p, light_pos));
	float a = clamp(ao * mat_ambient_mult, 0, 1);
	float d = clamp(dot(l,n)*mat_diffuse_mult*lightness,0,1);
	float s = clamp(pow(clamp(dot(normalize(l-r.v), n),0,1),mat_specular_power/light_size) * mat_specular_mult*lightness, 0,1);
	vec3 sky = 0.5 * skylight(r.p, n) + vec3(0.5);
	return d*col + a * mix(sky * ao , col, 0.5) + vec3(s);
}