#version 400
in vec2 Position;
out vec2 vPosition;

// debug
//uniform sampler2D u_heightMap;
//uniform float u_texScale;
//uniform mat4 pvm;

void main(void) {
	// debug
	//vec2 t = textureSize(u_heightMap, 0) * u_texScale;
	//gl_Position = pvm*vec4(Position.xy, 1, 0);
	// end debug
	vPosition = Position.xy;
}
