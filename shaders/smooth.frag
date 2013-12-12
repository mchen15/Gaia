#version 400

uniform sampler2D u_SmoothPass1tex;

in vec2 v_Texcoord;
out vec4 out_Color;

uniform int u_kernelY = 1;

vec3 sampleSmoothPass1(vec2 texcoords) {
    return texture(u_SmoothPass1tex,texcoords).xyz;
}

void main() {

	vec3 smoothColor = vec3(0.0);
	int kyHalf = u_kernelY/2;
	int count = 0;
	float delY = 1.0/textureSize(u_SmoothPass1tex,0).y;
	for(int i=-kyHalf; i<=kyHalf; ++i)
	{
			vec2 texCoord = vec2(v_Texcoord.s, v_Texcoord.t+i*delY);
			vec3 color = sampleSmoothPass1(texCoord);
			smoothColor += color;
			count++;
	}
        
	smoothColor = 1.0/count * smoothColor;
	
	out_Color = vec4(smoothColor, 1.0);

	//out_Color = vec4(sampleSmoothPass1(v_Texcoord),1.0);

	//out_Color = vec4(1,0,0,1);
}