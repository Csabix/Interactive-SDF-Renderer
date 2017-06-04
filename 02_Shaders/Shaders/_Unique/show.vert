in vec2 pos;
noperspective out vec2 vs_tex;
noperspective out vec2 vs_pos;

uniform vec2 cam_res_mult = vec2(1.0);

void main()
{
	gl_Position = vec4(pos,0,1);
	vs_pos = pos;
	vs_tex = (pos.xy * 0.5 + vec2(0.5)) * cam_res_mult;
}