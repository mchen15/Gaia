#version 330

in vec2 Position;
in vec2 Texcoord;

uniform sampler2D u_heightMap;
uniform mat4x4 u_pvm;

out vec2 v_Texcoord;
out vec3 v_Position;

void main(void) {

	float height = texture(u_heightMap, Texcoord).r;
	gl_Position = u_pvm * vec4(Position, height, 1);

	// using gl_texture_cube_map, so I can use the unit positions as 3D texture coordinates.
	v_Texcoord = Texcoord;
	v_Position = gl_Position.xyz;
}
