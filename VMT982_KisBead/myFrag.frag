#version 130

in vec3 vs_out_col;
in vec3 vs_out_pos;
out vec4 fs_out_col;

uniform int colorFade;
uniform float alpha;

void main()
{
	if(colorFade == 0)
		fs_out_col = vec4(vs_out_col,1);
	else if(colorFade == 1)
		fs_out_col = mix(vec4(vs_out_col, 1),vec4(1,0,0,1), alpha);
	else if(colorFade == 2)
		fs_out_col = mix(vec4(vs_out_col, 1),vec4(0,1,0,1), alpha);
	else if(colorFade == 3)
		fs_out_col = mix(vec4(vs_out_col, 1),vec4(0,0,1,1), alpha);

}