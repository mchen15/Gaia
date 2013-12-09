#version 330

uniform u_pvm;

in vec3 Position;
out vec3 v_Texcoord;

void main(void) {
	gl_Position = u_Persp * u_View * u_Model *  vec4(Position, 1.0, 1.0);

	// using gl_texture_cube_map, so I can use the unit positions as 3D texture coordinates.
	v_Texcoord = Position;
}
