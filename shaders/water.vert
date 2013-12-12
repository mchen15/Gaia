#version 330

in vec2 Position;
in vec2 Texcoord;

uniform sampler2D u_heightMap;
uniform mat4x4 u_Model;
uniform mat4x4 u_View;
uniform mat4x4 u_pvm;

out vec2 v_Texcoord;
out vec3 v_Position;

void main(void) {

	float height = texture(u_heightMap, Texcoord).r;
	gl_Position = u_pvm * vec4(Position, height, 1);

	v_Texcoord = Texcoord;
	//v_Position = gl_Position.xyz;

	vec4 pos = u_View * u_Model * vec4(Position, height, 1.0);
	v_Position = pos.xyz;
}
