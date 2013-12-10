#version 330

in vec2 v_Texcoord;
uniform sampler2D u_heightMap;
uniform sampler2D u_normalMap;
out vec4 fragColor;

void main(void)
{
	//fragColor = texture(u_heightMap, v_Texcoord);
	fragColor = vec4(1,1,1,1);
}
