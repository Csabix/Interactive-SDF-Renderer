#define PI 3.14159265359
//uniform float size_multipier = 1.0;
noperspective in vec2 vs_tex;
noperspective in vec2 vs_pos;
out vec4 fs_out;

subroutine vec4 Colorize();
layout(location = 0)	subroutine	uniform Colorize colorize;

layout(location = 0, binding = 2)	uniform sampler2D samplers[4];

layout(location = 4) uniform vec2 cam_res_mult			= vec2(1.0);
layout(location = 5) uniform vec4 color_offset[4]		= {vec4(0),vec4(0),vec4(0),vec4(0)};
layout(location = 9) uniform mat4 color_multipiers[4]	= {mat4(1),mat4(1),mat4(1),mat4(1)};

layout(index = 0) subroutine(Colorize)
vec4 colorize_0()
{
	return texture2D(samplers[0], vs_tex);
}

bool isBL() {return vs_pos.x < 0 && vs_pos.y < 0;}
bool isBR() {return vs_pos.x > 0 && vs_pos.y < 0;}
bool isTL() {return vs_pos.x < 0 && vs_pos.y > 0;}
bool isTR() {return vs_pos.x > 0 && vs_pos.y > 0;}

vec2 getCorner()
{	//TODO inline
	if (isTL()) return (vs_pos + vec2(1,0))*cam_res_mult;
	if (isTR()) return (vs_pos + vec2(0,0))*cam_res_mult;
	if (isBL()) return (vs_pos + vec2(1,1))*cam_res_mult;
	if (isBR()) return (vs_pos + vec2(0,1))*cam_res_mult;
}

layout(index = 1) subroutine(Colorize)
vec4 colorize_1()
{
	return color_multipiers[0]*texture2D(samplers[0], vs_tex) + color_offset[0];
}

layout(index = 2) subroutine(Colorize)
vec4 colorize_2() //négy részre bontja
{	//No dynamicly uniform texture lookup! (repeat tel-sem jó, pont az optimalizáció miatt)
	vec4 col = vec4(0);
	if (isTL()) col = color_multipiers[0]*texture2D(samplers[0], getCorner()) + color_offset[0];
	if (isTR()) col = color_multipiers[1]*texture2D(samplers[1], getCorner()) + color_offset[1];
	if (isBL()) col = color_multipiers[2]*texture2D(samplers[2], getCorner()) + color_offset[2];
	if (isBR()) col = color_multipiers[3]*texture2D(samplers[3], getCorner()) + color_offset[3];
	return col;
}

void main()
{
	fs_out = colorize();
}