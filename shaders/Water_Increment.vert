#version 400

in vec3 Position;
in vec2 Texcoord;



void main(void) 
{
	gl_Position = vec4(Position, 1.0);	
}
