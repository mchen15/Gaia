#version 330

uniform float u_Far;
uniform vec4 u_Color;

in vec3 fs_Normal;
in vec4 fs_Position;

void main(void)
{
    //out_Normal = vec4(normalize(fs_Normal),0.0f);
    //out_Position = vec4(fs_Position.xyz,1.0f); //Tuck position into 0 1 range
    //out_Color = vec4(1.0,1.0,0.0,1.0);//u_Color;

	gl_FragColor = vec4(1.0,0.0,0.0,1.0);
}
