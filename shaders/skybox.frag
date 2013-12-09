#version 330

in vec2 v_Texcoord;
uniform samplerCube u_cubemap;
out vec4 fragColor;

void main(void)
{
	vec4 color = texture(u_heightMap, v_Texcoord);
}
