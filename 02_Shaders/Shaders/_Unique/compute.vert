#define SQRT2 1.4142135623

in vec2 pos;
noperspective out vec2 vs_tex;
noperspective out vec2 vs_cam_coeffs;

uniform vec2 cam_res_mult = vec2(1.0);
uniform vec2 tex_res_mult = vec2(1.0);
uniform float cam_pixel_growth; //SQRT2 / length(vec2(640 , 480)) ;	
uniform vec2 cam_normalized_size; // vec2(640,480)/length(vec2(640,480));

void main()
{
	vec2 pos01 = (pos * 0.5 + vec2(0.5));
	gl_Position = vec4(pos * cam_res_mult + vec2(cam_res_mult-1),0,1);
	vs_cam_coeffs = pos * cam_normalized_size;
	vs_tex = pos01 * tex_res_mult;
}