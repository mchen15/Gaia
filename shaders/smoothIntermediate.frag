#version 400
/////////////////////////////////////
// Uniforms, Attributes, and Outputs
////////////////////////////////////
uniform sampler2D u_InputTex;

uniform int u_ScreenWidth = 1280;
uniform int u_ScreenHeight= 720;

in vec2 v_Texcoord;

out vec4 out_Color;
uniform int u_kernelX = 1;

/////////////////////////////////////
// UTILITY FUNCTIONS
/////////////////////////////////////

//Helper function to automicatlly sample and unpack positions
vec3 sampleCol(vec2 texcoords) {
    return texture(u_InputTex,texcoords).xyz;
}

///////////////////////////////////
// MAIN
//////////////////////////////////
void main() {
    vec3 color = sampleCol(v_Texcoord);

        int kxHalf = u_kernelX/2;
        int count = 0;
        float delX = 1.0/u_ScreenWidth;

        vec3 sumColor = vec3(0.0);

        for(int i=-kxHalf; i<=kxHalf; ++i)
        {
                vec2 texCoord = vec2(v_Texcoord.s+i*delX, v_Texcoord.t);
                vec3 color = sampleCol(texCoord);
                
                sumColor += color;
                count++;
        }

        sumColor = 1.0/count * sumColor;

        out_Color = vec4(sumColor,1.0);

		//out_Color = vec4(0,1,0,1);
        //out_Color = vec4(sampleCol(v_Texcoord),1.0);
}