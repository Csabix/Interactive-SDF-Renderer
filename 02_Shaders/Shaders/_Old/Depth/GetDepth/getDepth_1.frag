subroutine(GetDepth)
float getDepth_1()
{
	return texture2D(in_depth, inDepthCoords(vs_out_pos.xy*0.5+vec2(0.5)));
}