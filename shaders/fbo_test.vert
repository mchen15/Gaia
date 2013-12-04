#version 400

in vec3 Position;
in vec2 Texcoord;

out vec2 fs_Texcoord;

void main(void) 
{
	gl_Position = vec4(Position.xyz, 1.0);
	fs_Texcoord = Texcoord;
}
