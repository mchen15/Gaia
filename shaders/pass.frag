#version 400

in vec2 v_Texcoord;
uniform sampler2D u_heightMap;
out vec4 fragColor;

void main(void)
{
	//vec2 tex = v_Texcoord + vec2(0.5,0.5);
	vec4 color = texture2D(u_heightMap, v_Texcoord);
   	//fragColor = vec4(1.0,0.0,0.0,1.0);

	
	//vec4 color = vec4(v_Texcoord, 0.0, 1.0);
	//if (v_Texcoord == vec2(0,0))
	//	color = vec4(1,1,1,1);
	//else
	//	color = vec4(1,0,0,1);
	
	fragColor = color;
	fragColor = vec4(1,0,0,1);
}
