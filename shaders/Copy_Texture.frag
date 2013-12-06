#version 400


uniform sampler2D u_sourceTex;

out vec4 out_destTex;


void main (void)
{
	out_destTex = texture(u_sourceTex,v_Texcoord).rgba;
}