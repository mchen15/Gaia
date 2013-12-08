#version 400

in vec3 Position;
in vec2 Texcoord;

out vec3 fs_Position;
out vec2 fs_Texcoord;

void main(void) 
{
	fs_Position = Position;
	fs_Texcoord = Texcoord;

	gl_Position = vec4(Position, 1.0);	
}
