#version 330

uniform mat4x4 u_pvm;
in vec2 Position;
in vec2 Texcoord;
out vec2 v_Texcoord;

void main(void) {
	gl_Position = u_pvm * vec4(Position,0,1);

	// using gl_texture_cube_map, so I can use the unit positions as 3D texture coordinates.
	v_Texcoord = Texcoord;
}
