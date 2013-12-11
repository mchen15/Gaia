#version 400

in vec2 texcoord;
in float depth;
out vec4 fragment;

uniform sampler2D u_heightMap;
uniform sampler2D u_normalMap;
uniform sampler2D u_diffuseMap;
uniform float u_heightScale; 
uniform mat4 u_mvInvTrans;
uniform vec2 u_numPatches;
uniform float u_gridSpacing;
uniform vec4 u_lightColor;
uniform vec3 u_lightDirection;
uniform int u_toggleNormal;

//vec3 incident = normalize(vec3(1.0, 5.2, 4.5));
//vec4 light = vec4(1.0, 0.95, 0.9, 1.0) * 1.1;

float sampleHeight(vec2 coord)
{
	return u_heightScale*texture(u_heightMap, coord).r;
}

vec3 sampleNormal(vec2 coord)
{
	return texture(u_normalMap, coord).xyz;
}

vec3 sampleDiffuse(vec2 coord)
{
	return texture(u_diffuseMap, coord).rgb;
}

vec3 getNormalSobel()
{
	const ivec3 off = ivec3(-1.0,0.0,1.0);
	vec2 tSize = 1.0/textureSize(u_heightMap,0);
	float topLeft = sampleHeight( vec2( texcoord + tSize*off.xz));
	float top = sampleHeight( vec2 (texcoord + tSize*off.yz));
	float topRight = sampleHeight( vec2( texcoord + tSize*off.zz));
	float left = sampleHeight( vec2( texcoord + tSize*off.xy));
	float right = sampleHeight( vec2( texcoord + tSize*off.zy));
	float bottomLeft = sampleHeight( vec2( texcoord + tSize*off.xx));
	float bottom = sampleHeight( vec2( texcoord + tSize*off.yx));
	float bottomRight = sampleHeight( vec2( texcoord + tSize*off.zx));


	float dx = (topLeft + 2.0*left+ bottomLeft) - (topRight + 2.0*right+bottomRight) ;
	float dy = (topLeft + 2.0*top+ topRight) - (bottomLeft + 2.0*bottom+bottomRight);
	float dz = 4.0;

	vec3 normal = normalize ( vec3(dx,dy,dz));
	return normal;

}

void main(){
    //vec3 normal = normalize(texture(terrain, texcoord).xyz);
    //vec4 color = texture(diffuse, texcoord);
    //float noise_factor = texture(noise_tile, texcoord*32).r+0.1;

    //float dot_surface_incident = max(0, dot(normal, incident));

    //color = color * light * noise_factor * (max(0.1, dot_surface_incident)+0.05)*1.5;
    //fragment = mix(color, color*0.5+vec4(0.5, 0.5, 0.5, 1.0), depth*2.0);

	// compute normal

	//vec2 stepSize = 1.0 / u_numPatches;
	vec2 stepSize = 1.0 / textureSize(u_heightMap, 0);

	float h21 = sampleHeight(vec2(texcoord.s + stepSize.s, texcoord.t));
	float h01 = sampleHeight(vec2(texcoord.s - stepSize.s, texcoord.t));
	float h12 = sampleHeight(vec2(texcoord.s, texcoord.t + stepSize.t));
	float h10 = sampleHeight(vec2(texcoord.s, texcoord.t - stepSize.t));

	//h21 = h01;
	//h12 = h10;

	float diff1 = h21 - h01;
	float diff2 = h12 - h10;

	vec3 slopeX = vec3(
		2.0 * u_gridSpacing,
		0.0,
		h21 - h01
	);

	vec3 slopeY = vec3(
		0.0,
		2.0 * u_gridSpacing,
		h12 - h10
	);

	vec3 normal  = normalize(u_mvInvTrans * vec4(normalize(cross(slopeX, slopeY)), 0.0)).xyz;
	
	// using normal map

	if (u_toggleNormal == 0)
		normal = getNormalSobel();
	else
		normal = sampleNormal(texcoord);
	//vec3 color = sampleDiffuse(texcoord);


	
	vec3 color = mix( vec3(0.54,0.27,0), vec3(0,0,1), texture(u_heightMap, texcoord).g);
	
	float intensity = max(dot(u_lightDirection, normal), 0.0);
	color = color * intensity * u_lightColor.xyz;

	float avgSlope = (diff1 + diff2) / 2.0;


	const float EPSILON = 0.01;
	//boundary conditions
	if( texcoord.s > 0 - EPSILON && texcoord.s < 0 + EPSILON || 
	

	(texcoord.s > 1 - EPSILON && texcoord.s < 1 + EPSILON) || 
		

	(texcoord.t > 0 - EPSILON && texcoord.t < 0 + EPSILON) || 
		

	(texcoord.t > 1 - EPSILON && texcoord.t < 1 + EPSILON) )
		fragment =  vec4( 0,0,0,0);
	else

	fragment = vec4(color,1.0);

	//fragment = vec4(diff1, 0, 0, 1.0);
	//fragment = vec4(diff2, 0, 0, 1.0);
	//fragment = vec4(avgSlope, 0, 0, 1.0);

	//fragment = vec4(slopeY, 1.0);
	//fragment = vec4(normal, 1.0);
	//fragment = vec4(1,1,1,1);
}