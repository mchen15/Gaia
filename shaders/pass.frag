#version 400

uniform float u_Far;
uniform vec4 u_Color;

in vec3 fs_Normal;
in vec4 fs_Position;

out vec4 fragColor;

void main(void)
{
   	fragColor = vec4(1.0,0.0,0.0,1.0);
}
