#version 400

in vec3 Position;
out vec3 fs_Position;

void main(void) 
{
	fs_Position = Position;
	gl_Position = vec4(Position, 1.0);	
}
