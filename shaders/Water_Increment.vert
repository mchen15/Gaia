#version 400

in vec3 Position;
in vec2 Texcoord;

out vec2 v_Texcoord;

void main(void) {

	v_Texcoord = Texcoord;
	v_Texcoord.y = 1-Texcoord.y;


	gl_Position = vec4(Position.xyz,1.0);

}

