#version 400

in vec2 globalUV;
in vec3 tePosition;

out vec4 fragColor;
uniform sampler2D u_heightMap;
uniform float u_texScale;
uniform mat3 u_mvInvTrans;
uniform vec3 u_lightPosWorld;
uniform float u_heightScale;
uniform float u_gridSpacing;

void main(void)
{
	vec2 texSize = textureSize(u_heightMap,0);
	float texelUnit = 1.0/ (texSize*u_texScale);

	vec3 ddx = vec3( 
			2.0*u_gridSpacing,
			0, 
			texture(u_heightMap, vec2(globalUV.s+texelUnit, globalUV.t)).r*u_heightScale - 
				texture(u_heightMap, vec2(globalUV.s-texelUnit,globalUV.t)).r*u_heightScale
		);
	
	vec3 ddy = 
		vec3 (
			0, 
			2.0*u_gridSpacing, 
			texture(u_heightMap, vec2(globalUV.s, globalUV.t+texelUnit)).r*u_heightScale - 
				texture(u_heightMap,vec2(globalUV.s, globalUV.t - texelUnit)).r*u_heightScale
		);
	
	vec3 normal = u_mvInvTrans*normalize(cross(ddx,ddy));
	vec3 lightVec = u_lightPosWorld - tePosition;
	float diffuse = max(dot(lightVec,normal),0.0);
	vec3 color = vec3(1.0,0.0,0.0);

	color = color*diffuse + vec3(0.2,0.2,0.2);
	fragColor = vec4(color.xyz,1.0);
}
