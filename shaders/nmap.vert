#version 400

in vec3 Position;
in vec2 Texcoord;

out vec2 v_Texcoord;

void main(void) {
    //vec4 world = u_Model * vec4(Position, 1.0);
    //vec4 camera = u_View * world;
	//vPositionCamSpace = camera.xyz;
	//vPosition = Position.xyz;
	//v_Texcoord = Texcoord;
	v_Texcoord = Position.xy*0.5 + 0.5;
	v_Texcoord.y = 1-v_Texcoord.y;
	gl_Position = vec4(Position.xyz,1.0);

}
