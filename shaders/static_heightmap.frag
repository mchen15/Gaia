#version 400

in vec2 teGlobalUV;
in vec3 tePosition;

out vec4 fragColor;
uniform sampler2D u_heightMap;
uniform float u_texScale;
uniform mat3 u_mvInvTrans;
uniform vec3 u_lightPosWorld;
uniform float u_heightScale;
uniform float u_gridSpacing;


float height( float u, float v)
{
	return texture(u_heightMap, vec2(u,v)).r*u_heightScale;
}

void main(void)
{

	float delta = 1.0/ (textureSize(u_heightMap,0).x*u_texScale);

	vec3 ddx = vec3(2.0*u_gridSpacing,0.0, height(teGlobalUV.s + delta, teGlobalUV.t) - height(teGlobalUV.s - delta, teGlobalUV.t));
	vec3 ddy = vec3(0.0,2.0*u_gridSpacing, height(teGlobalUV.s + delta, teGlobalUV.t) - height(teGlobalUV.s - delta, teGlobalUV.t));

	vec3 normal = normalize( u_mvInvTrans*cross(ddx,ddy));

	//vec2 texSize = textureSize(u_heightMap,0);
	//float texelUnit = 1.0/ (texSize*u_texScale);

	//vec3 ddx = vec3( 
	//		2.0*u_gridSpacing,
	//		0, 
	//		texture(u_heightMap, vec2(teGlobalUV.s+texelUnit, teGlobalUV.t)).r*u_heightScale - 
	//			texture(u_heightMap, vec2(teGlobalUV.s-texelUnit,teGlobalUV.t)).r*u_heightScale
	//	);
	//
	//vec3 ddy = 
	//	vec3 (
	//		0, 
	//		2.0*u_gridSpacing, 
	//		texture(u_heightMap, vec2(teGlobalUV.s, teGlobalUV.t+texelUnit)).r*u_heightScale - 
	//			texture(u_heightMap,vec2(teGlobalUV.s, teGlobalUV.t - texelUnit)).r*u_heightScale
	//	);
	//
	//vec3 normal = u_mvInvTrans*normalize(cross(ddx,ddy));
	vec3 lightVec = u_lightPosWorld - tePosition;
	float diffuse = max(dot(lightVec,normal),0.0);
	vec3 color = vec3(1.0,0.0,0.0);

	color = color*diffuse + vec3(0.2,0.2,0.2);
	fragColor = vec4(color.xyz,1.0);
}
