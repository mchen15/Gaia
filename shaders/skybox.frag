#version 330

in vec3 v_Texcoord;
uniform samplerCube u_cubemap;
out vec4 fragColor;

void main(void)
{
	fragColor = texture(u_cubemap, v_Texcoord);
}
