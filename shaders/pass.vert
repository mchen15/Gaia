#version 330


uniform mat4x4 u_Model;
uniform mat4x4 u_View;
uniform mat4x4 u_Persp;

in vec3 Position;
in vec3 Texcoords;


void main(void) {
    vec4 world = u_Model * vec4(Position, 1.0);
    vec4 camera = u_View * world;
    gl_Position = u_Persp * camera;
}
