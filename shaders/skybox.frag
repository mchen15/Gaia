#version 400

in vec2 v_Texcoord;
uniform sampler2D u_cubemap;
out vec4 fragColor;

void main(void)
{
	vec4 color = texture2D(u_heightMap, v_Texcoord);
}
